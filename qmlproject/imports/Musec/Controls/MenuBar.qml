import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import Musec 1.0

QQC2.MenuBar {
    id: menuBar
    width: contentItem.contentWidth
    height: 20
    property color backgroundColor: "#000000"
    property bool oneMenuOpened: false
    delegate: QQC2.MenuBarItem {
        id: menuBarItem
        font.family: Constants.font
        font.pointSize: 10
        height: 20
        // Content of menu item
        contentItem: Text {
            anchors.centerIn: menuBarBackground
            text: menuBarItem.text.indexOf('&')>=0?
                menuBarItem.text.split('&')[0] + menuBarItem.text.split('&')[1].substring(0, 1) + menuBarItem.text.split('&')[1].substring(1, menuBarItem.text.split('&')[1].length):
                menuBarItem.text
            font: menuBarItem.font
            opacity: enabled ? 1.0 : 0.3
            color: menuBarItem.highlighted ? Constants.contentColor1 : Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        // Background of `MenuBar`
        background: Rectangle {
            id: menuBarBackground
            implicitWidth: contentItem.contentWidth + 10
            height: 20
            opacity: enabled ? 1 : 0.3
            color: menuBarItem.highlighted? Constants.menuHighlightBackgroundColor: menuBar.background.color
            border.width: 1
            border.color: color
        }
        onPressed: {
            menu.open();
        }
    }
    background: Rectangle {
        implicitWidth: contentItem.contentWidth + 10
        height: 20
        color: menuBar.backgroundColor
    }
}
