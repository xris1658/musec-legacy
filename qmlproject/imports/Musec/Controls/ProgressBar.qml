import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Shapes 1.15
import Musec 1.0

QQC2.ProgressBar {
    property bool showPercentage: true
    property bool showAnimation: true
    width: 200
    height: 50
    indeterminate: false
    id: control
    value: 0.40
    background: Rectangle {
        color: Constants.backgroundColor
        border.color: Constants.borderColor
    }
    contentItem: Item {
        anchors.fill: parent
        Rectangle {
            id: foreground
            opacity: control.indeterminate? 0.0: 1.0
            width: (control.width - 2 * background.border.width) * control.value
            height: control.height - 2 * background.border.width
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 1
            color: Qt.rgba(6 / 256, 176 / 256, 37 / 256, 1);

            clip: true
            Rectangle {
                width: 40
                height: parent.height
                x: -width
                y: 0
                gradient: Gradient {
                    orientation: Qt.Horizontal
                    GradientStop { position: 0.0; color: Qt.rgba(foreground.color.r, foreground.color.g, foreground.color.b, 0) }
                    GradientStop { position: 0.3; color: Qt.rgba(foreground.color.r, foreground.color.g, foreground.color.b, 0.5) }
                    GradientStop { position: 0.5; color: Qt.rgba(foreground.color.r * 1.5, foreground.color.g * 1.5, foreground.color.b * 1.5) }
                    GradientStop { position: 0.7; color: Qt.rgba(foreground.color.r, foreground.color.g, foreground.color.b, 0.5) }
                    GradientStop { position: 1.0; color: Qt.rgba(foreground.color.r, foreground.color.g, foreground.color.b, 0) }
                }
                SequentialAnimation on x {
                    running: showAnimation && value < 1
                    loops: Animation.Infinite
                    NumberAnimation {
                        from: -width
                        to: background.width + 2 * width
                        easing.type: Easing.Linear
                        duration: 4000
                    }
                }
            }
        }
        Text {
            opacity: showPercentage? 1.0: 0.0
            anchors.centerIn: parent
            text: (value * 100).toString().substring(0, value >= 1? 3: value>= 0.1? 2: 1)  + "%"
            color: Constants.contentColor1
            font {
                family: Constants.font
            }
        }
    }
}
