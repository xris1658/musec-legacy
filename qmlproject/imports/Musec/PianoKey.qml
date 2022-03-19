import QtQuick 2.15
import QtQuick.Controls 2.15

import Musec 1.0

Item {
    property int keyHeight: 14
    width: 100
    height: keyHeight * Constants.midiValueRange
    Item {
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        z: 2
        clip: true
        Column {
            id: blackKeys
            anchors.fill: parent
            Repeater {
                model: Constants.midiValueRange
                Item {
                    property int num: Constants.midiValueRange - 1 - index
                    width: parent.width
                    height: keyHeight
                    Rectangle {
                        width: parent.width * 0.6
                        height: parent.height
                        opacity: Constants.blackKey(num)? 1 : 0
                        color: "#000000"
                    }
                    Text {
                        anchors.right: parent.right
                        anchors.rightMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        text: (num % 12 == 0) ? "C" + (num - (num % 12)) / 12 : ""
                        font.family: "Noto Sans Mono Condensed"
                        font.bold: true
                    }
                }
            }
        }
    }

    Item {
        anchors.fill: parent
        clip: true
        z: 1
        Column {
            id: whiteKeys
            width: parent.width
            height: keyHeight * 12 * 75 / 7 // 75 是白键的数量
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.fill: parent
            Repeater {
                model: 75
                Rectangle {
                    width: parent.width
                    height: index == 0? keyHeight * 9 / 7 : keyHeight * 12 / 7
                    border.color: "#000000"
                }
            }
        }
    }
}
