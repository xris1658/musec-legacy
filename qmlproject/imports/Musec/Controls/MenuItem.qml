import QtQuick 2.15
import QtQuick.Shapes 1.15
import QtQuick.Controls 2.15 as QQC2

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

QQC2.MenuItem {
    id: menuItemDelegate
    implicitWidth: 50
    height: 20
    font.family: Constants.font
    property alias shortcut: action.shortcut
    action: MCtrl.Action {
        id: action
    }
    function mainTextWidth(): int {
        return actionText.contentWidth;
    }
    function keyTextWidth(): int {
        return actionShortcut.contentWidth;
    }
    function indicatorWidth(): int {
        return indicator.width;
    }
    function arrowWidth(): int {
        return arrow.width;
    }
    // Cascade menu indicator at right
    arrow: Item {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: height
        visible: menuItemDelegate.subMenu
        Shape {
            anchors.fill: parent
            opacity: menuItemDelegate.subMenu? 1: 0;
            ShapePath {
                strokeWidth: 0
                strokeColor: fillColor
                startX: 8; startY: 6
                PathLine {x: 12; y: 10}
                PathLine {x: 8; y: 14}
                fillColor: menuItemDelegate.highlighted? Constants.menuHighlightContentColor:
                                                         Constants.menuContentColor
                fillRule: ShapePath.WindingFill
            }
        }
    }
    // Menu item checked indicator at left
    indicator: Item {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 3
        width: height
        Rectangle {
            width: 12
            height: 12
            anchors.verticalCenter: parent.verticalCenter
            visible: menuItemDelegate.checkable
            border.width: 1
            border.color: menuItemDelegate.highlighted? Constants.menuHighlightContentColor:
                                                        Constants.menuContentColor
            color: menuItemDelegate.highlighted? Constants.menuHighlightBackgroundColor:
                                                 Constants.menuBackgroundColor
            Shape {
                width: 8
                height: 8
                anchors.centerIn: parent
                visible: menuItemDelegate.checked
                layer.enabled: true
                layer.samples: 4
                antialiasing: true
                smooth: true
                ShapePath {
                    strokeWidth: 1
                    strokeColor: menuItemDelegate.highlighted? Constants.menuHighlightContentColor:
                                                               Constants.menuContentColor
                    startX: 1; startY: 3
                    PathLine { x: 3; y: 5 }
                    PathLine { x: 7; y: 1 }
                    PathLine { x: 7; y: 3 }
                    PathLine { x: 3; y: 7 }
                    PathLine { x: 1; y: 5 }
                    PathLine { x: 1; y: 3 }
                    fillColor: menuItemDelegate.highlighted? Constants.menuHighlightContentColor:
                                                             Constants.menuContentColor
                    fillRule: ShapePath.WindingFill
                }
            }
        }
    }
    // Text of menu item
    contentItem: Item {
        height: 20
        Text {
            id: actionText
            leftPadding: menuItemDelegate.indicator.width - 5
            rightPadding: menuItemDelegate.arrow.width
            anchors.verticalCenter: parent.verticalCenter
            text: menuItemDelegate.text.indexOf('&') >= 0?
                      menuItemDelegate.text.split('&')[0] + "<u>" + menuItemDelegate.text.split('&')[1].substring(0, 1) + "</u>" + menuItemDelegate.text.split('&')[1].substring(1, menuItemDelegate.text.split('&')[1].length)
                    : menuItemDelegate.text
            font: menuItemDelegate.font
            opacity: enabled? 1.0 : 0.3
            color: menuItemDelegate.highlighted? Constants.menuHighlightContentColor:
                                                 Constants.menuContentColor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            id: actionShortcut
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: menuItemDelegate.action.shortcut
            font: menuItemDelegate.font
            opacity: enabled? 1.0 : 0.3
            color: menuItemDelegate.highlighted? Constants.menuHighlightContentColor:
                                                 Constants.menuContentColor
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }
   // Background of menu item
    background: Rectangle {
        width: menuItemDelegate.width - 2
        height: menuItemDelegate.height - 2
        anchors.centerIn: parent
        opacity: 1
        color: menuItemDelegate.highlighted? Constants.menuHighlightBackgroundColor:
                                             Constants.menuBackgroundColor
    }
}
