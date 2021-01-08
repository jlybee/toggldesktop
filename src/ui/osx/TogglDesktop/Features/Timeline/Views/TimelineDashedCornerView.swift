//
//  TimelineDashedCornerView.swift
//  TogglDesktop
//
//  Created by Nghia Tran on 7/2/19.
//  Copyright © 2019 Alari. All rights reserved.
//

import Foundation

/// Helper View to draw a dash rounded rectangle
/// It tends for the Empty Time Entry
final class TimelineDashedCornerView: NSView {

    // MARK: Variables

    var cornerRadius: CGFloat = 10.0
    private lazy var color: NSColor = Color.timelineDashedBoxBorder.color

    // MARK: Overriden

    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
        let path = NSBezierPath(roundedRect: bounds, xRadius: cornerRadius, yRadius: cornerRadius)
        path.lineWidth = 3

        let dashes: [CGFloat] = [3.0, 3.0]
        path.setLineDash(dashes, count: dashes.count, phase: 0.0)
        path.lineCapStyle = .butt
        path.lineJoinStyle = .bevel

        // Draw
        color.setStroke()
        path.stroke()
    }
}
