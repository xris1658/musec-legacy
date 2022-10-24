import QtQuick 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

import Musec.Models 1.0 as MModel

MCtrl.Slider {
    id: control
    orientation: Qt.Vertical
    property MModel.VolumeFaderScaleModel model
    Rectangle {
        width: control.handle.width - 4
        height: 2
        parent: control.handle
        anchors.centerIn: parent
        color: Constants.borderColor
    }
    Item {
        anchors.right: parent.right
        width: parent.width
        height: 1
        y: control.topPadding + control.handle.height / 2
        Rectangle {
            anchors.left: parent.left
            width: parent.width / 4
            height: 1
            color: Constants.borderColor
        }
        Rectangle {
            anchors.right: parent.right
            width: parent.width / 4
            height: 1
            color: Constants.borderColor
        }
    }
    Repeater {
        model: control.model
        Item {
            parent: control
            anchors.right: parent.right
            width: parent.width
            height: 1
            y: control.topPadding + control.handle.height / 2 + (1 - position) * (control.height - control.topPadding - control.bottomPadding - control.handle.height)
            property bool isZero: decibel == 0.0
            Rectangle {
                anchors.left: parent.left
                width: parent.isZero? parent.width / 3: parent.width / 4
                height: 1
                color: Constants.borderColor
            }
            Rectangle {
                anchors.right: parent.right
                width: parent.isZero? parent.width / 3: parent.width / 4
                height: 1
                color: Constants.borderColor
            }
        }
    }
    Item {
        anchors.right: parent.right
        width: parent.width
        height: 1
        y: control.topPadding + control.handle.height / 2 + control.height - control.topPadding - control.bottomPadding - control.handle.height
        Rectangle {
            anchors.left: parent.left
            width: parent.width / 4
            height: 1
            color: Constants.borderColor
        }
        Rectangle {
            anchors.right: parent.right
            width: parent.width / 4
            height: 1
            color: Constants.borderColor
        }
    }
}
