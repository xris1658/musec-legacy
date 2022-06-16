import QtQuick 2.15
import QtQuick.Controls 2.15
//import Musec 1.0

Rectangle {
    color: Constants.backgroundColor
    clip: true
    Rectangle {
        anchors.fill: parent
        color: Constants.backgroundColor
    }
    Column {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 20
        Column {
            Text {
                text: qsTr("Name")
                font.family: Constants.font
                font.pointSize: 12
                color: Constants.contentColor1
            }
            Text {
                text: qsTr("Properties")
                font.family: Constants.font
                font.pointSize: 10
                color: Constants.contentColor2
            }
        }
        Column {
            Text {
                text: qsTr("Custom Text")
                font.family: Constants.font
                font.pointSize: 12
                color: Constants.contentColor1
            }
            Text {
                text: qsTr("Double click here to add custom information.")
                font.family: Constants.font
                font.pointSize: 10
                color: Constants.contentColor2
            }
        }
    }
}
