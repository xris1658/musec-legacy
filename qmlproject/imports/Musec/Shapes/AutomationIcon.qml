import QtQuick 2.15
import QtQuick.Shapes 1.15

import Musec 1.0

Item {
    id: root
    property color fillColor
    width: 12
    height: 12
    Rectangle {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 5
        height: width
        radius: width / 2
        color: root.fillColor
    }
    Rectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        width: 5
        height: width
        radius: width / 2
        color: root.fillColor
    }
    Shape {
        width: parent.width
        height: parent.height
        antialiasing: true
        smooth: true
        ShapePath {
            startX: 3
            startY: 9
            PathLine {
                x: 9
                y: 3
            }
            strokeWidth: 1.5
            strokeColor: fillColor
            fillColor: root.fillColor
        }
    }
}