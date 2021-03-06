//
//  TimeDecoratorView.swift
//  TogglDesktop
//
//  Created by Nghia Tran on 2/25/19.
//  Copyright © 2019 Alari. All rights reserved.
//

import Cocoa

final class TimeDecoratorView: NSView {

    @IBOutlet weak var backgroundBox: NSView!

    override func awakeFromNib() {
        super.awakeFromNib()

        wantsLayer = true
        layer?.masksToBounds = false
        backgroundBox.wantsLayer = true
        backgroundBox.layer?.masksToBounds = false
        backgroundBox.shadow = NSShadow()
        backgroundBox.layer?.shadowColor = Color.shadow.cgColor
        backgroundBox.layer?.shadowOpacity = 1.0
        backgroundBox.layer?.shadowOffset = CGSize(width: 0, height: -2)
        backgroundBox.layer?.shadowRadius = 8
        backgroundBox.layer?.cornerRadius = 4
    }

    override func updateLayer() {
        super.updateLayer()
        backgroundBox.layer?.backgroundColor = Color.whiteBackgroundColor.cgColor
    }
}
