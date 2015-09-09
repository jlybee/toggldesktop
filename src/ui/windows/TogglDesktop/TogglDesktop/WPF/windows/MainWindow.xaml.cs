﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using TogglDesktop.Diagnostics;

namespace TogglDesktop.WPF
{
    public partial class MainWindow
    {
        #region fields

        private readonly KeyboardHook startHook = new KeyboardHook();
        private readonly KeyboardHook showHook = new KeyboardHook();

        private readonly WindowInteropHelper interopHelper;
        private readonly UserControl[] views;
        private Window[] childWindows;

        private AboutWindow aboutWindow;
        private FeedbackWindow feedbackWindow;
        private EditViewPopup editPopup;

        private bool remainOnTop;

        private UserControl activeView;
        private bool isInManualMode;
        private bool isTracking;
        private bool isResizingWithHandle;

        #endregion

        public MainWindow()
        {
            this.DataContext = this;
            this.InitializeComponent();

            this.interopHelper = new WindowInteropHelper(this);

            this.views = new UserControl[] {this.loginView, this.timerEntryListView};

            this.hideAllViews();

            this.initializeWindows();
            this.initializeContextMenu();
            this.initializeTaskbarIcon();
            this.initializeEvents();

            this.startHook.KeyPressed += this.onGlobalStartKeyPressed;
            this.showHook.KeyPressed += this.onGlobalShowKeyPressed;
            this.IsVisibleChanged += this.onIsVisibleChanged;

            this.finalInitialisation();
        }

        #region setup

        private void initializeContextMenu()
        {
            foreach (var item in this.ContextMenu.Items)
            {
                var asMenuItem = item as MenuItem;
                if (asMenuItem != null)
                {
                    asMenuItem.CommandTarget = this;
                }
            }
        }

        private void hideAllViews()
        {
            foreach (var view in this.views)
            {
                view.Visibility = Visibility.Collapsed;
            }
        }

        private void initializeWindows()
        {
            this.childWindows = new Window[]{
                this.editPopup = new EditViewPopup(),
                this.aboutWindow = new AboutWindow(),
                this.feedbackWindow = new FeedbackWindow(),
                new PreferencesWindow(),
                new IdleNotificationWindow(),
            };

            this.timerEntryListView.SetEditPopup(this.editPopup.EditView);

            this.editPopup.IsVisibleChanged += this.editPopupVisibleChanged;
            this.editPopup.SizeChanged += (sender, args) => this.updateEntriesListWidth();

            this.IsVisibleChanged += this.ownChildWindows;
        }

        private void initializeTaskbarIcon()
        {
            this.taskbarIcon.ContextMenu = this.mainContextMenu;
        }

        private void ownChildWindows(object sender, DependencyPropertyChangedEventArgs args)
        {
            foreach (var window in this.childWindows)
            {
                window.Owner = this;
            }

            this.IsVisibleChanged -= this.ownChildWindows;
        }

        private void initializeEvents()
        {
            Toggl.OnApp += this.onApp;
            Toggl.OnError += this.onError;
            Toggl.OnLogin += this.onLogin;
            Toggl.OnTimeEntryList += this.onTimeEntryList;
            Toggl.OnOnlineState += this.onOnlineState;
            Toggl.OnReminder += this.onReminder;
            Toggl.OnURL += this.onURL;
            Toggl.OnRunningTimerState += this.onRunningTimerState;
            Toggl.OnStoppedTimerState += this.onStoppedTimerState;
            Toggl.OnSettings += this.onSettings;
        }

        private void finalInitialisation()
        {
            if (!Toggl.StartUI(TogglDesktop.Program.Version()))
            {
                MessageBox.Show("Missing callback. See the log file for details");
                this.shutdown(1);
            }

            Utils.LoadWindowLocation(this, this.editPopup);

            this.aboutWindow.UpdateReleaseChannel();

            this.runScriptAsync();
        }

        private async void runScriptAsync()
        {
            await Task.Delay(TimeSpan.FromSeconds(1));

            if (Toggl.ScriptPath == null)
            {
                return;
            }

            ThreadPool.QueueUserWorkItem(args => this.runScript(), null);
        }

        private void runScript()
        {
            if (!File.Exists(Toggl.ScriptPath))
            {
                Toggl.Debug("Script file does not exist: " + Toggl.ScriptPath);
                this.shutdown(0);
            }

            var script = File.ReadAllText(Toggl.ScriptPath);

            long errorCode = 0;
            var result = Toggl.RunScript(script, ref errorCode);
            if (errorCode != 0)
            {
                Toggl.Debug(string.Format("Failed to run script, err = {0}", errorCode));
            }
            Toggl.Debug(result);

            if (errorCode == 0)
            {
                this.shutdown(0);
            }
        }

        #endregion

        #region toggl events

        private void onStoppedTimerState()
        {
            if (this.TryBeginInvoke(this.onStoppedTimerState))
                return;

            this.updateTracking(null);
        }

        private void onRunningTimerState(Toggl.TogglTimeEntryView te)
        {
            if (this.TryBeginInvoke(this.onRunningTimerState, te))
                return;

            this.updateTracking(te);
        }

        private void onURL(string url)
        {
            Process.Start(url);
        }

        private void onReminder(string title, string informativeText)
        {
            if (this.TryBeginInvoke(this.onReminder, title, informativeText))
                return;

            this.taskbarIcon.ShowBalloonTip(title, informativeText, Properties.Resources.toggl);
        }

        private void onOnlineState(long state)
        {
            if (this.TryBeginInvoke(this.onOnlineState, state))
                return;

            this.updateStatusIcons(state == 0);
        }

        private void onTimeEntryList(bool open, List<Toggl.TogglTimeEntryView> list)
        {
            if (this.TryBeginInvoke(this.onTimeEntryList, open, list))
                return;

            if (open)
            {
                this.setActiveView(this.timerEntryListView);
            }
        }

        private void onLogin(bool open, ulong userID)
        {
            if (this.TryBeginInvoke(this.onLogin, open, userID))
                return;

            if (open)
            {
                this.setActiveView(this.loginView);
            }

            if (open || userID == 0)
            {
                this.emailAddressMenuItem.Header = "Logged out";
            }
            else
            {
                this.emailAddressMenuItem.Header = Toggl.UserEmail();
            }
            this.updateTracking(null);
        }

        private void onError(string errmsg, bool userError)
        {
        }

        private void onApp(bool open)
        {
            if (this.TryBeginInvoke(this.onApp, open))
                return;

            if (open)
            {
                this.Show();
            }
        }

        private void onSettings(bool open, Toggl.TogglSettingsView settings)
        {
            this.setGlobalShortcutsFromSettings();
            this.remainOnTop = settings.OnTop;
            this.setWindowOnTop();
        }

        #endregion

        #region ui events

        private void onGlobalShowKeyPressed(object sender, KeyPressedEventArgs e)
        {
            this.togglVisibility();
        }

        private void onGlobalStartKeyPressed(object sender, KeyPressedEventArgs e)
        {
            if (this.isTracking)
            {
                using (Performance.Measure("stopping time entry from global short cut", this.isInManualMode))
                {
                    Toggl.Stop();
                }
            }
            else
            {
                using (Performance.Measure("starting time entry from global short cut, manual mode: {0}", this.isInManualMode))
                {
                    this.startTimeEntry();
                }
            }
        }

        private void editPopupVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            this.updateEditPopupLocation();
            this.updateEntriesListWidth();
        }

        protected override void onCloseButtonClick(object sender, RoutedEventArgs e)
        {
            this.minimizeToTray();
        }

        protected override void onCogButtonClick(object sender, RoutedEventArgs e)
        {
            this.mainContextMenu.PlacementTarget = (FrameworkElement)sender;
            this.mainContextMenu.Placement = PlacementMode.Bottom;
            this.mainContextMenu.HorizontalOffset = 0;
            this.mainContextMenu.VerticalOffset = 0;

            this.mainContextMenu.IsOpen = true;
        }

        protected override void OnLocationChanged(EventArgs e)
        {
            this.updateEditPopupLocation();

            base.OnLocationChanged(e);
        }

        protected override void OnRenderSizeChanged(SizeChangedInfo sizeInfo)
        {
            this.updateEditPopupLocation();

            base.OnRenderSizeChanged(sizeInfo);
        }

        private void onTaskbarLeftMouseUp(object sender, RoutedEventArgs e)
        {
            this.togglVisibility();
        }

        private void onResizeHandleLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            const int htBottomRight = 17;

            Mouse.Capture(null);

            Win32.SendMessage(this.interopHelper.Handle,
                Win32.wmNcLButtonDown,
                htBottomRight,
                0);

            this.resizeHandle.CaptureMouse();
            this.isResizingWithHandle = true;
        }

        private void onResizeHandleLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            this.endHandleResizing();
        }

        private void onWindowMouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Released)
            {
                this.endHandleResizing();
            }
        }

        private void onIsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (this.IsVisible)
            {
                this.setWindowOnTop();
            }
        }

        #endregion

        #region command events

        private void onNewCommand(object sender, RoutedEventArgs e)
        {
            using (Performance.Measure("starting time entry from menu, manual mode: {0}", this.isInManualMode))
            {
                this.startTimeEntry();
            }
        }

        private void onContinueCommand(object sender, RoutedEventArgs e)
        {
            using (Performance.Measure("continuing time entry from menu"))
            {
                Toggl.ContinueLatest();
            }
        }
        
        private void onStopCommand(object sender, RoutedEventArgs e)
        {
            using (Performance.Measure("stopping time entry from menu"))
            {
                Toggl.Stop();   
            }
        }
        
        private void onShowCommand(object sender, RoutedEventArgs e)
        {
            this.Show();
            this.Topmost = true;
        }
        
        private void onSyncCommand(object sender, RoutedEventArgs e)
        {
            using (Performance.Measure("syncing from menu"))
            {
                Toggl.Sync();
            }
        }
        
        private void onReportsCommand(object sender, RoutedEventArgs e)
        {
            using (Performance.Measure("opening reports from menu"))
            {
                Toggl.OpenInBrowser();
            }
        }
        
        private void onPreferencesCommand(object sender, RoutedEventArgs e)
        {
            using (Performance.Measure("opening preferences from menu"))
            {
                Toggl.EditPreferences();
            }
        }
        
        private void onToggleManualModeCommand(object sender, RoutedEventArgs e)
        {
            this.isInManualMode = !this.isInManualMode;

            this.togglManualModeMenuItem.Header =
                this.isInManualMode ? "Use timer" : "Use manual mode";

            this.timerEntryListView.SetManualMode(this.isInManualMode);
        }
        
        private void onClearCacheCommand(object sender, RoutedEventArgs e)
        {
            var result = MessageBox.Show(this,
                "This will remove your Toggl user data from this PC and log you out of the Toggl Desktop app. " +
                "Any unsynced data will be lost.\nDo you want to continue?", "Clear Cache",
                MessageBoxButton.YesNo, MessageBoxImage.Question);

            if (result == MessageBoxResult.Yes)
            {
                using (Performance.Measure("clearing cache from menu"))
                {
                    Toggl.ClearCache();
                }
            }
        }
        
        private void onSendFeedbackCommand(object sender, RoutedEventArgs e)
        {
            this.feedbackWindow.Show();
            this.feedbackWindow.Topmost = true;
        }
        
        private void onAboutCommand(object sender, RoutedEventArgs e)
        {
            this.aboutWindow.Show();
            this.aboutWindow.Topmost = true;
        }
        
        private void onLogoutCommand(object sender, RoutedEventArgs e)
        {
            using (Performance.Measure("logging out from menu"))
            {
                Toggl.Logout();
            }
        }
        
        private void onQuitCommand(object sender, RoutedEventArgs e)
        {
            this.shutdown(0);
        }

        #endregion

        #region ui controlling

        private void togglVisibility()
        {
            if (this.IsVisible)
            {
                this.minimizeToTray();
            }
            else
            {
                this.showOnTop();
            }
        }

        private void minimizeToTray()
        {
            this.Hide();
            if (this.editPopup.IsVisible)
            {
                // TODO: consider saving popup open state and restoring when window is shown
                this.editPopup.Hide();
                this.timerEntryListView.DisableHighlight();
            }
        }

        private void setGlobalShortcutsFromSettings()
        {
            try
            {
                this.startHook.ChangeTo(
                    Properties.Settings.Default.StartModifiers,
                    Properties.Settings.Default.StartKey
                    );
            }
            catch (Exception e)
            {
                Toggl.Debug("Could not register start shortcut: " + e);
            }

            try
            {
                this.showHook.ChangeTo(
                    Properties.Settings.Default.ShowModifiers,
                    Properties.Settings.Default.ShowKey
                    );
            }
            catch (Exception e)
            {
                Toggl.Debug("Could not register show hotkey: " + e);
            }
        }

        private void startTimeEntry()
        {
            if (this.isInManualMode)
            {
                var guid = Toggl.Start("", "0", 0, 0, "", "");
                Toggl.Edit(guid, false, Toggl.Duration);
            }
            else
            {
                Toggl.Start("", "", 0, 0, "", "");
            }
        }

        private void showOnTop()
        {
            this.Show();
            this.Topmost = true;
            this.Activate();
        }

        private void shutdown(int exitCode)
        {
            if (this.taskbarIcon != null)
            {
                this.taskbarIcon.Visibility = Visibility.Collapsed;
            }

            if (this.mainContextMenu != null)
            {
                this.mainContextMenu.IsOpen = false;
                this.mainContextMenu.Visibility = Visibility.Collapsed;
            }

            if (this.IsVisible)
            {
                if (exitCode == 0)
                {
                    Utils.SaveWindowLocation(this, this.editPopup);
                }
                this.Hide();
            }

            this.Close();

            this.Dispatcher.BeginInvoke(DispatcherPriority.Background, new Action(() =>
            {
                Program.Shutdown(exitCode);
            }));
        }

        private void updateStatusIcons(bool isOnline)
        {
            string notifyIconName;

            if (this.isTracking)
            {
                notifyIconName = isOnline ? "IconOnlineActive" : "IconOfflineActive";
            }
            else
            {
                notifyIconName = isOnline ? "IconOnlineInactive" : "IconOfflineInactive";
            }

            this.taskbarIcon.IconSource = (BitmapImage)this.FindResource(notifyIconName);
        }

        private void updateTracking(Toggl.TogglTimeEntryView? timeEntry)
        {
            var tracking = timeEntry != null;

            this.isTracking = tracking;

            if (tracking)
            {
                var description = timeEntry.Value.Description;

                if (string.IsNullOrEmpty(description))
                {
                    this.Title = "Toggl Desktop";
                    this.runningMenuItem.Header = "Timer is tracking";
                }
                else
                {
                    this.Title = description + " - Toggl Desktop";
                    this.runningMenuItem.Header = description;
                }

            }
            else
            {
                this.runningMenuItem.Header = "Timer is not tracking";
                this.Title = "Toggl Desktop";
            }

            this.updateContextMenuItems();
            this.updateStatusIcons(true);
            this.SetIconState(tracking);
        }

        private void updateContextMenuItems()
        {
            var loggedIn = TogglDesktop.Program.IsLoggedIn;

            // todo: disable commands (instead)
            this.newMenuItem.IsEnabled = loggedIn;
            this.continueMenuItem.IsEnabled = loggedIn && !this.isTracking;
            this.stopMenuItem.IsEnabled = loggedIn && this.isTracking;
            this.syncMenuItem.IsEnabled = loggedIn;
            this.reportsMenuItem.IsEnabled = loggedIn;
            this.togglManualModeMenuItem.IsEnabled = loggedIn;
            this.clearCacheMenuItem.IsEnabled = loggedIn;
            this.sendFeedbackMenuItem.IsEnabled = loggedIn;
            this.logoutMenuItem.IsEnabled = loggedIn;
        }

        #endregion

        #region window size, position and state handling

        private void setWindowOnTop()
        {
            Win32.SetWindowPos(this.interopHelper.Handle,
                this.remainOnTop ? Win32.HWND_TOPMOST : Win32.HWND_NOTOPMOST,
                0, 0, 0, 0, Win32.SWP_NOMOVE | Win32.SWP_NOSIZE);

            if (this.editPopup != null)
            {
                this.editPopup.SetWindowOnTop(this.remainOnTop);
            }
        }

        private void endHandleResizing()
        {
            if (!this.isResizingWithHandle)
                return;

            Mouse.Capture(null);
            this.isResizingWithHandle = false;
        }

        private void updateEntriesListWidth()
        {
            if (this.WindowState == WindowState.Maximized && this.editPopup.IsVisible)
            {
                this.timerEntryListView.SetListWidth(this.ActualWidth - this.editPopup.ActualWidth);
            }
            else
            {
                this.timerEntryListView.DisableListWidth();
            }
        }

        private void updateEditPopupLocation()
        {
            if (this.editPopup == null || !this.editPopup.IsVisible)
                return;

            if (this.WindowState == WindowState.Maximized)
            {
                Win32.Rectangle bounds;
                Win32.GetWindowRect(this.interopHelper.Handle, out bounds);

                var x = (double)bounds.Left;
                var y = (double)bounds.Top;

                var headerHeight = this.WindowHeaderHeight + this.timerEntryListView.TimerHeight;

                y += headerHeight;
                x += this.ActualWidth;

                this.editPopup.SetPlacement(true, x, y, this.ActualHeight - headerHeight, true);
            }
            else
            {
                var s = this.getCurrentScreen();
                bool left = s.WorkingArea.Right - (this.Left + this.ActualWidth) < this.editPopup.Width;

                var x = this.Left;
                var y = this.Top;

                if (!left)
                {
                    x += this.ActualWidth;
                }

                this.editPopup.SetPlacement(left, x, y, this.Height);
            }

        }

        private void setActiveView(UserControl activeView)
        {
            if (activeView == null)
                throw new ArgumentNullException("activeView");

            if (this.activeView != null)
            {
                this.activeView.Visibility = Visibility.Collapsed;
            }

            this.activeView = activeView;

            activeView.Visibility = Visibility.Visible;
            this.editPopup.Hide();
            this.timerEntryListView.DisableHighlight();
            this.updateContextMenuItems();

            this.updateMinimumSize(activeView);
        }

        private void updateMinimumSize(UserControl activeView)
        {
            this.MinHeight = this.WindowHeaderHeight + activeView.MinHeight;
            this.MinWidth = activeView.MinWidth;
        }

        #endregion
    }
}
