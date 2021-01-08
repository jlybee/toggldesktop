// Copyright 2014 Toggl Desktop developers.

#ifndef SRC_IDLE_H_
#define SRC_IDLE_H_

#include <sstream>

#include "model/settings.h"
#include "model/user.h"
#include "util/logger.h"

#include <Poco/Types.h>

namespace toggl {

class GUI;

class TOGGL_INTERNAL_EXPORT Idle {
 public:
    explicit Idle(GUI *ui);
    virtual ~Idle() {}

    void SetIdleSeconds(
        const Poco::Int64 idle_seconds,
        User *current_user);

    void SetSettings(const Settings &settings) {
        settings_ = settings;
    }

    void SetSystemLocked() {
        if (!last_locked_started_)
            last_locked_started_ = time(nullptr);
    }

    void SetSystemUnlocked(User *current_user) {
        if (last_locked_started_) {
            Poco::Int64 slept_seconds = time(nullptr) - last_locked_started_;
            if (slept_seconds > 0) {
                SetIdleSeconds(slept_seconds, current_user);
            }
        }

        last_locked_started_ = 0;
    }

 private:
    void computeIdleState(const Poco::Int64 idle_seconds,
                          User *current_user);

    Logger logger { "idle" };

    // Idle detection related values
    Poco::Int64 last_idle_seconds_reading_;
    Poco::Int64 last_idle_started_;
    time_t last_locked_started_;

    Settings settings_;
    GUI *ui_;
};

}  // namespace toggl

#endif  // SRC_IDLE_H_
