import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Shapes 1.15

import Musec 1.0

QQC2.Switch {
    id: control
    opacity: enabled? 1.0: 0.5
    implicitWidth: 30
    implicitHeight: 20
    hoverEnabled: true
    indicator: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        color: control.pressed? Constants.backgroundColor2:
               control.hovered? Constants.mouseOverElementColor:
                                Constants.backgroundColor
        border.color: Constants.borderColor
        border.width :control.activeFocus? 2: 1
        Rectangle {
            // Seems like I can't use anchors to indicate the state of the switch.
            x: control.checked? parent.width - width - 4: 4
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width / 2 - 4
            height: parent.height - 8
        }
    }
    contentItem: Text {
        anchors.left: indicator.right
        anchors.leftMargin: control.indicatorAndContentSpacing
        anchors.verticalCenter: indicator.verticalCenter
        text: control.text
        color: Constants.contentColor1
        font.family: Constants.font
        verticalAlignment: Text.AlignVCenter
    }
}