import QtQuick 2.15
import QtQuick.Shapes 1.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Rectangle {
    property alias numerator: bigClock.numerator
    property alias demoninator: bigClock.demoninator
    width: parent.width
    height: 70
    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#333333"
        }
        GradientStop {
            position: 1
            color: "#727272"
        }
    }

    //大号时钟
    BigClockBackup {
        id: bigClock
        anchors.centerIn: parent
    }

    //走带按钮
    Row {
        id: transportButtons
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: bigClock.left
        anchors.rightMargin: 10
        height: parent.height - 10 * 2
        spacing: 8
        Gradient {
            id: transportButtonsGradient
            GradientStop {
                position: 0
                color: "#1a1a1a"
            }
            GradientStop {
                position: 1
                color: "#414141"
            }
        }
        MCtrl.Button {
            id: playButton
            anchors.left: Row.left
            background: Rectangle {
                gradient: transportButtonsGradient
                radius: 5
            }
            width: 56
            height: 36
            z: 1
            Shape {
                id: playButtonIcon
                width: 18
                height: 16
                anchors.centerIn: parent
                layer.enabled: true
                layer.samples: 4
                antialiasing: true
                smooth: true
                ShapePath {
                    strokeWidth: 2
                    strokeColor: playing? "#00CC33": "#FFFFFF"
                    startX: 4; startY: 2
                    joinStyle: ShapePath.RoundJoin
                    capStyle: ShapePath.RoundCap
                    PathLine { x: 15; y: 8 }
                    PathLine { x: 4;  y: 14}
                    PathLine { x: 4;  y: 2 }
                    fillColor: strokeColor
                    fillRule: ShapePath.WindingFill
                }
                z: 2
            }
            onClicked: {
                playing = !playing;
            }
        }
        Rectangle {
            id: stopButton
            gradient: transportButtonsGradient
            width: 36
            height: 36
            radius: 5
            z: 1
            Rectangle {
                id: stopButtonIcon
                anchors.centerIn: parent
                width: 14
                height: 14
                color: "#FFFFFF"
            }
        }
        Rectangle {
            id: recordButton
            gradient: transportButtonsGradient
            width: 36
            height: 36
            radius: 5
            z: 1
            Rectangle {
                width: 16
                height: 16
                anchors.centerIn: parent
                radius: 8
                color: "#FF0000"
                z: 2
            }
        }
        Rectangle {
            id: loopButton
            gradient: transportButtonsGradient
            width: 36
            height: 36
            radius: 5
            z: 1
            Shape {
                id: shapeLoop
                property color color: actionLoop.checked?
                                          "#FFFFFF":
                                          Constants.currentElementColor
                width: 16
                height: 18
                anchors.centerIn: parent
                ShapePath {
                    strokeWidth: -1
                    startX: 4; startY: 0
                    PathLine { x: 12; y: 0 }
                    PathLine { x: 16; y: 4 }
                    PathLine { x: 12; y: 8 }
                    PathLine { x: 12; y: 6 }
                    PathLine { x: 11; y: 6 }
                    PathLine { x: 4 ; y: 6 }
                    PathLine { x: 4 ; y: 14}
                    PathLine { x: 12; y: 14}
                    PathLine { x: 12; y: 10}
                    PathLine { x: 16; y: 10}
                    PathLine { x: 16; y: 18}
                    PathLine { x: 0 ; y: 18}
                    PathLine { x: 0 ; y: 2 }
                    PathLine { x: 11; y: 2 }
                    PathLine { x: 11; y: 0 }
                    PathLine { x: 12; y: 0 }
                    fillColor: shapeLoop.color
                }
                z: 2
            }
        }
        Rectangle {
            id: setInPoint
            gradient: transportButtonsGradient
            width: 24
            height: 36
            radius: 5
            z: 1
            Item {
                width: 12
                height: 16
                anchors.centerIn: parent
                Rectangle {
                    width: 12
                    height: 8
                    x: 0
                    y: 0
                    color: "#727272"
                }
                Rectangle {
                    width: 1
                    height: 8
                    anchors.left: parent.left
                    y: 8
                    color: "#727272"
                }
            }
        }
        Rectangle {
            id: setOutPoint
            gradient: transportButtonsGradient
            width: 24
            height: 36
            radius: 5
            z: 1
            Item {
                width: 12
                height: 16
                anchors.centerIn: parent
                Rectangle {
                    width: 12
                    height: 8
                    x: 0
                    y: 0
                    color: "#727272"
                }
                Rectangle {
                    width: 1
                    height: 8
                    anchors.right: parent.right
                    y: 8
                    color: "#727272"
                }
            }
        }
    }
    //工具按钮
    Row {
        id: toolButtons
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: bigClock.right
        anchors.leftMargin: 10
        height: parent.height - 10 * 2
        spacing: 8
        Rectangle {
            id: metronomeButton
            width: 36
            height: 36
            radius: 5
            gradient: transportButtonsGradient
            Shape {
                width: 16
                height: 16
                anchors.centerIn: parent
                layer.enabled: true
                layer.samples: 2
                antialiasing: true
                layer.smooth: true
                layer.textureSize: Qt.size(width * 2, height * 2)
                smooth: true
                ShapePath {
                    startX: startPoint.x
                    startY: startPoint.y
                    PathLine {
                        x: 2
                        y: 14
                    }
                    PathLine {
                        x: 14
                        y: 14
                    }
                    PathLine {
                        x: 11
                        y: 0
                    }
                    PathLine { x: 10; y: 0 }
                    PathLine { x: 10; y: 10 }
                    PathLine { x: 6; y: 10 }
                    PathLine { x: 6; y: 0 }
                    PathLine {
                        id: startPoint
                        x: 5
                        y: 0
                    }
                    fillColor: menuItemMetronome.checked? "#FFFFFF": "#727272"
                    strokeColor: fillColor
                }
            }
        }
        Rectangle {
            id: enableKeyboardButton
            width: 36
            height: 36
            radius: 5
            gradient: transportButtonsGradient
            Item {
                id: shapeEnableKeyboard
                property color color: actionEnableKeyboard.checked?
                                          "#FFFFFF":
                                          Constants.currentElementColor
                width: 16
                height: 12
                anchors.centerIn: parent
                Rectangle {
                    x: 0
                    y: 0
                    width: 2
                    height: 10
                    color: shapeEnableKeyboard.color
                }
                Rectangle {
                    x: 6
                    y: 0
                    width: 2
                    height: 10
                    color: shapeEnableKeyboard.color
                }
                Rectangle {
                    x: 12
                    y: 0
                    width: 2
                    height: 10
                    color: shapeEnableKeyboard.color
                }
                Rectangle {
                    x: 0
                    y: 8
                    width: 4
                    height: 4
                    color: shapeEnableKeyboard.color
                }
                Rectangle {
                    x: 5
                    y: 8
                    width: 4
                    height: 4
                    color: shapeEnableKeyboard.color
                }
                Rectangle {
                    x: 10
                    y: 8
                    width: 4
                    height: 4
                    color: shapeEnableKeyboard.color
                }
            }
        }
        Rectangle {
            id: followButton
            width: 36
            height: 36
            radius: 5
            gradient: transportButtonsGradient
            Item {
                id: shapeFollow
                property color color: actionFollow.checked?
                                          "#FFFFFF":
                                          Constants.currentElementColor
                width: 16
                height: 16
                anchors.centerIn: parent
                Shape {
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 6
                    height: 3
                    layer.enabled: true
                    layer.samples: 4
                    antialiasing: true
                    smooth: true
                    ShapePath {
                        fillColor: shapeFollow.color
                        fillRule: ShapePath.WindingFill
                        strokeColor: fillColor
                        joinStyle: ShapePath.MiterJoin
                        capStyle: ShapePath.RoundCap
                        strokeWidth: 1
                        startX: 0; startY: 0
                        PathLine { x: 6; y: 0 }
                        PathLine { x: 3; y: 3 }
                        PathLine { x: 0; y: 0 }
                    }
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 2
                    height: 12
                    color: shapeFollow.color
                }
                Rectangle {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    width: 6
                    height: 2
                    color: shapeFollow.color
                }
                Rectangle {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: 6
                    height: 2
                    color: shapeFollow.color
                }
                Shape {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: 3
                    height: 6
                    layer.enabled: true
                    layer.samples: 4
                    antialiasing: true
                    smooth: true
                    ShapePath {
                        fillColor: shapeFollow.color
                        strokeColor: fillColor
                        strokeWidth: 0
                        fillRule: ShapePath.WindingFill
                        startX: 0; startY: 0
                        PathLine { x: 3; y: 3 }
                        PathLine { x: 0; y: 6 }
                        PathLine { x: 0; y: 0 }
                    }
                }
            }
        }
        Rectangle {
            id: snapButton
            width: 36
            height: 36
            radius: 5
            gradient: transportButtonsGradient
            Item {
                id: shapeSnap
                property color color: actionSnap.checked? "#FFFFFF": Constants.currentElementColor
                width: 16
                height: 16
                anchors.centerIn: parent
                Shape {
                    width: parent.width
                    height: parent.height
                    layer.enabled: true
                    layer.samples: 4
                    antialiasing: true
                    smooth: true
                    ShapePath {
                        startX: 8
                        startY: 0
                        PathArc {
                            x: 16
                            y: 8
                            radiusX: 8
                            radiusY: 8
                            direction: PathArc.Clockwise
                            useLargeArc: false
                        }
                        PathLine {
                            x: 16
                            y: 16
                        }
                        PathLine {
                            x: 12
                            y: 16
                        }
                        PathLine {
                            x: 12
                            y: 8
                        }
                        PathArc {
                            x: 8
                            y: 4
                            radiusX: 4
                            radiusY: 4
                            direction: PathArc.Counterclockwise
                            useLargeArc: false
                        }
                        PathLine {
                            x: 8
                            y: 0
                        }
                        fillColor: shapeSnap.color
                        fillRule: ShapePath.WindingFill
                        strokeColor: fillColor
                        joinStyle: ShapePath.MiterJoin
                    }
                }
                Shape {
                    width: parent.width
                    height: parent.height
                    layer.enabled: true
                    layer.samples: 4
                    antialiasing: true
                    smooth: true
                    ShapePath {
                        startX: 8
                        startY: 0
                        PathArc {
                            x: 0
                            y: 8
                            radiusX: 8
                            radiusY: 8
                            direction: PathArc.Counterclockwise
                            useLargeArc: false
                        }
                        PathLine {
                            x: 0
                            y: 16
                        }
                        PathLine {
                            x: 4
                            y: 16
                        }
                        PathLine {
                            x: 4
                            y: 8
                        }
                        PathArc {
                            x: 8
                            y: 4
                            radiusX: 4
                            radiusY: 4
                            direction: PathArc.Clockwise
                            useLargeArc: false
                        }
                        PathLine {
                            x: 8
                            y: 0
                        }
                        fillColor: "transparent"
                        fillRule: ShapePath.WindingFill
                        strokeColor: shapeSnap.color
                        strokeWidth: 1.5
                        joinStyle: ShapePath.MiterJoin
                    }
                }
            }
        }
    }
}
