import QtQuick 2.15
import QtQuick.Controls 2.15

import Musec 1.0

MenuSeparator {
    height: 5
    background: Rectangle {
        color: Constants.menuBackgroundColor
        Rectangle {
            width: 1
            height: parent.height
            anchors.right: parent.right
            color: Constants.borderColor
        }
        Rectangle {
            width: 1
            height: parent.height
            anchors.left: parent.left
            color: Constants.borderColor
        }
    }
    contentItem: Item {
        height: parent.height
        implicitWidth: 200
        anchors.centerIn: parent
        Rectangle {
            width: parent.width
            height: 1
            anchors.centerIn: parent
            color: Constants.menuSeparatorColor
        }
    }
}
