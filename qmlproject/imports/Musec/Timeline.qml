import QtQuick 2.15
//import Musec 1.0

Rectangle {
    id: root
    property int fullNoteWidth: 80
    property int barWidth: 80
    property int barCount: 20
    property int numerator: 4
    property int denominator: 4
    property double start: 0
    property double length: 4
    property bool loop: true
    width: barWidth * barCount
    height: 25
    color: "#000000"
    Row {
        anchors.fill: parent
        z: 1
        Repeater {
            model: root.barCount
            Rectangle {
                id: barRect
                width: root.barWidth
                height: parent.height
                color: "#000000"
                Row {
                    anchors.fill: parent
                    Repeater {
                        model: numerator
                        Rectangle {
                            property int ind: index
                            anchors.bottom: parent.bottom
                            width: root.barWidth / numerator
                            height: parent.height
                            color: "#000000"
                            Rectangle {
                                anchors.left: parent.left
                                anchors.bottom: parent.bottom
                                width: 1
                                height: (parent.ind == 0)? 15 : 10
                                color: Constants.borderColor
                            }
                        }
                    }
                }
                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 0
                    text: index + 1
                    font.family: "Noto Sans Mono Condensed"
                    color: Constants.borderColor
                }
            }
        }
    }
    Rectangle {
        anchors.top: parent.top
        z: 2
        x: start * root.barWidth / numerator
        width: length * root.barWidth / numerator
        height: 10
        border.color: Constants.mouseOverElementColor
        color: loop? border.color: "transparent"
    }
}
