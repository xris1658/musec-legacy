import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Shapes 1.15
import Musec 1.0

QQC2.CheckBox {
    id: control
    opacity: enabled? 1: 0.5
    checked: true
    width: 20
    height: 20
    indicator: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        color: Constants.backgroundColor
        border.color: Constants.borderColor
        border.width: control.activeFocus? 2: 1
        Shape {
            width: 80
            height: 80
            scale: parent.width>parent.height? (parent.height - 4) / height: (parent.width - 4) / width
            anchors.centerIn: parent
            visible: control.checked
            layer.enabled: true
            layer.samples: 4
            antialiasing: true
            smooth: true
            ShapePath {
                strokeWidth: 1
                strokeColor: Constants.contentColor1
                startX: 10; startY: 30
                PathLine { x: 30; y: 50 }
                PathLine { x: 70; y: 10 }
                PathLine { x: 70; y: 30 }
                PathLine { x: 30; y: 70 }
                PathLine { x: 10; y: 50 }
                PathLine { x: 10; y: 30 }
                fillColor: Constants.contentColor1
                fillRule: ShapePath.WindingFill
            }
        }
        Rectangle {
            width: parent.width - 4
            height: parent.height - 4
            anchors.centerIn: parent
            color: Constants.contentColor1
            opacity: control.checkState == Qt.PartiallyChecked? 1: 0
        }
    }
}
