import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    color: Constants.backgroundColor2
    property ListModel tracks
    clip: true
    Rectangle {
        id: mixerContentIndicator
        width: 25
        height: parent.height
        color: Constants.backgroundColor
        Rectangle {
            anchors.right: parent.right
            anchors.top: parent.top
            width: 1
            height: parent.height
            color: Constants.borderColor
            z: 2
        }
    }
    Rectangle {
        id: mixerContentArea
        anchors.left: mixerContentIndicator.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        clip: true
        color: Constants.backgroundColor2
        MixerChannel {
            id: masterChannel
            channelName: qsTr("Master")
            channelColor: Constants.backgroundColor2
            channelNumber: 0
            width: 120
            height: root.height - scroll.height
        }
        Rectangle {
            id: masterChannelRightBorder
            anchors.left: masterChannel.right
            anchors.top: parent.top
            width: 1
            height: parent.height
            color: Constants.borderColor
            z: 2
        }
        ListView {
            id: trackChannelList
            orientation: Qt.Horizontal
            anchors.left: masterChannelRightBorder.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            clip: true
            model: tracks
            interactive: false
            ScrollBar.horizontal: ScrollBar {
                id: scroll
                parent: mixerContentArea
                width: parent.width - masterChannel.width - masterChannelRightBorder.width
                anchors.right: parent.right
                height: 15
                anchors.bottom: parent.bottom
                background: Rectangle {
                    color: Constants.backgroundColor
                }
                contentItem: Rectangle {
                    radius: 2
                    color: Constants.mouseOverElementColor
                    opacity: trackChannelList.width < trackChannelList.contentWidth? 1: 0
                }
            }
            footer: Item {
                width: 50
                height: root.height - scroll.height
            }
            delegate: Row {
                MixerChannel {
                    channelName: name
                    channelColor: color
                    width: 120
                    height: root.height - scroll.height
                    channelNumber: index + 1
                }
                Rectangle {
                    width: 1
                    height: root.height
                    color: Constants.borderColor
                }
            }
        }
    }
}
