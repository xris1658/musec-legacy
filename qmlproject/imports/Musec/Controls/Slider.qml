import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2

import Musec 1.0

QQC2.Slider {
    id: control
    value: 0.5
    clip: true
//    orientation: Qt.Vertical
    property alias implicitThickness: background.implicitThickness
    property alias implicitLength: background.implicitLength
    property alias handleImplicitThickness: handle.handleImplicitThickness
    property alias handleImplicitLength: handle.handleImplicitLength
    property int lengthPadding: 0
    property int thicknessPadding: 6
    leftPadding: orientation == Qt.Horizontal? lengthPadding: thicknessPadding
    rightPadding: orientation == Qt.Horizontal? lengthPadding: thicknessPadding
    topPadding: orientation == Qt.Horizontal? thicknessPadding: lengthPadding
    bottomPadding: orientation == Qt.Horizontal? thicknessPadding: lengthPadding

    background: Rectangle {
        id: background
        property int implicitThickness: 4
        property int implicitLength: 200
        x: orientation == Qt.Horizontal? control.leftPadding: control.leftPadding + control.availableWidth / 2 - width / 2
        y: orientation == Qt.Horizontal? control.topPadding + control.availableHeight / 2 - height / 2: control.topPadding
        implicitWidth: orientation == Qt.Horizontal? implicitLength: implicitThickness
        implicitHeight: orientation == Qt.Horizontal? implicitThickness: implicitLength
        width: orientation == Qt.Horizontal? control.availableWidth: implicitThickness
        height: orientation == Qt.Horizontal? implicitThickness: control.availableHeight
        color: "transparent"
        border.color: Constants.borderColor
        Rectangle {
            width: orientation == Qt.Horizontal? control.visualPosition * parent.width: parent.width
            y: orientation == Qt.Horizontal? 0: control.visualPosition * parent.height
            height: parent.height - y
//            height: orientation == Qt.Horizontal? parent.height: control.visualPosition * parent.height
            color: Constants.currentElementColor
        }
    }

    handle: Rectangle {
        id: handle
        property int handleImplicitThickness: 10
        property int handleImplicitLength: 15
        x: orientation == Qt.Horizontal? control.leftPadding + control.visualPosition * (control.availableWidth - width): control.leftPadding + control.availableWidth / 2 - width / 2
        y: orientation == Qt.Horizontal? control.topPadding + control.availableHeight / 2 - height / 2: control.topPadding + control.visualPosition * (control.availableHeight - height)
        implicitWidth: orientation == Qt.Horizontal? handleImplicitLength: handleImplicitThickness
        implicitHeight: orientation == Qt.Horizontal? handleImplicitThickness: handleImplicitLength
        radius: (handleImplicitThickness - implicitThickness) / 2
        color: control.pressed? Qt.darker(Constants.contentColor1): control.hovered? Constants.currentElementColor: Constants.contentColor1
        border.color: Constants.borderColor
        border.width: control.activeFocus? 2: 1
    }
}
