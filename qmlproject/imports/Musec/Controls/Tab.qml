import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width: 500
    height: 500
    TabBar {
        id: bar
        width: parent.width
        height: 20
        background: Item {}
        TabButton {
            text: "Tab Button"
            width: implicitWidth
            font.family: Constants.font
            contentItem: Text {
                id: _text
                anchors.centerIn: parent.background
                text: parent.text
                font: parent.font
                color: Constants.contentColor1
            }
            background: Rectangle {
                width: parent.width
                height: 20
                color: Constants.backgroundColor
            }
        }
    }
    StackLayout {
        width: parent.width
        anchors.bottom: parent.bottom
        y: bar.height
        height: parent.height - bar.height
        currentIndex: bar.currentIndex
    }
}

