import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2

import Musec 1.0

QQC2.Dial {
    id: control
    opacity: enabled? 1: 0.5
    background: Rectangle {
        width: Math.min(control.width, control.height)
        height: width
        radius: width / 2
        x: (control.width - width) / 2
        y: (parent.height - height) / 2
        color: Constants.backgroundColor
        border.width: 1
        border.color: Constants.borderColor
    }
    handle: Rectangle {
        id: handleItem
        x: control.background.x + (control.background.width - width) / 2
        y: control.background.y + (control.background.height - height) / 2
        width: 4
        height: control.background.height / 4
        color: Constants.contentColor2
        antialiasing: true
        transform: [
            Translate {
                y: (handleItem.height - control.background.height) / 2
            },
            Rotation {
                angle: control.angle
                origin.x: handleItem.width / 2
                origin.y: handleItem.height / 2
            }
        ]
    }
}
