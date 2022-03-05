import QtQuick 2.15
import QtQuick.Shapes 1.15
//import Musec 1.0

Item {
    id: control
//    property color noteBackgroundColor: noteMouseArea.pressed? Qt.darker(Constants.noteBackgroundColor, 1.1): Constants.noteBackgroundColor
    property int noteMidiNum: 0
    property string noteName
    property real mouseInitialX: 0
    property real mouseDeltaX: 0
    property int velocity: 100
    width: 100
    height: 20
    Rectangle {
        anchors.fill: parent
        color: Constants.noteBackgroundColor
        opacity: (velocity + 20) / (Constants.maxVelocity + 20)
    }
    clip: true
    Rectangle {
        id: rightResizeIndicator
        visible: rightResizeArea.containsMouse | rightResizeArea.pressed
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: 10
        height: parent.height - anchors.margins * 2
        anchors.margins: 2
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                position: 0
                color: "transparent"
            }
            GradientStop {
                position: 1
                color: rightResizeArea.pressed? Qt.darker(Constants.noteBackgroundColor): Qt.lighter(Constants.noteBackgroundColor)
            }
        }
    }
    Rectangle {
        id: leftResizeIndicator
        visible: leftResizeArea.containsMouse | leftResizeArea.pressed
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: 10
        height: parent.height - anchors.margins * 2
        anchors.margins: 2
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                position: 1
                color: "transparent"
            }
            GradientStop {
                position: 0
                color: leftResizeArea.pressed? Qt.darker(Constants.noteBackgroundColor): Qt.lighter(Constants.noteBackgroundColor)
            }
        }
    }
    Text {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 3
        text: (Constants.dynamicKeyName.get(noteMidiNum % 12).sharp) + ((noteMidiNum - noteMidiNum % 12) / 12)
        font.family: "Noto Sans Mono Condensed"
        font.bold: true
        font.pointSize: 9
        color: "#000000"
    }
    MouseArea {
        id: noteMouseArea
        anchors.fill: parent
        property int initialX: 0
        property int initialY: 0
        property bool settingVelocity: false
        onPressed: {
            settingVelocity = mouse.modifiers & Qt.AltModifier;
            if(settingVelocity) {
                initialY = 0;
            }
            else {
                var contentAreaCoordinate = control.mapToItem(control.parent, mouseX, mouseY);
                initialX = contentAreaCoordinate.x;
                initialY = contentAreaCoordinate.y;
            }
        }
        onReleased: {
            settingVelocity = false;
        }
        onMouseXChanged: {
            if(pressed) {
                if(settingVelocity) {
                    var delta = mouseY - initialY;
                    control.velocity -= delta;
                    if(control.velocity > Constants.maxVelocity) {
                        control.velocity = Constants.maxVelocity;
                    }
                    else if(control.velocity < 0) {
                        control.velocity = 0;
                    }
                }
                else {
                    var contentAreaCoordinate = control.mapToItem(control.parent, mouseX, mouseY);
                    var delta = contentAreaCoordinate.x - initialX;
                    control.x += delta;
                    if(control.x < 0) {
                        control.x = 0;
                    }
                    initialX = contentAreaCoordinate.x;
                }
            }
        }
        onMouseYChanged: {
            if(pressed) {
                if(settingVelocity) {
                    control.velocity -= mouseY - initialY;
                    if(control.velocity > Constants.maxVelocity) {
                        control.velocity = Constants.maxVelocity;
                    }
                    else if(control.velocity < 0) {
                        control.velocity = 0;
                    }
                }
                else {
                    var contentAreaCoordinate = control.mapToItem(control.parent, mouseX, mouseY);
                    var deltaY = contentAreaCoordinate.y - initialY;
                    if(deltaY >= control.height && control.noteMidiNum >= 0) {
                        control.y += control.height + 1;
                        control.noteMidiNum -= 1;
                        initialY = contentAreaCoordinate.y;
                    }
                    else if(deltaY <= 0 - control.height && control.noteMidiNum < Constants.midiValueRange - 1) {
                        control.y -= control.height + 1;
                        control.noteMidiNum += 1;
                        initialY = contentAreaCoordinate.y;
                    }
                }
            }
        }
        onDoubleClicked: {
            control.destroy();
        }
        z: 1
    }
    MouseArea {
        id: leftResizeArea
        anchors.left: parent.left
        width: 5
        height: parent.height
        hoverEnabled: true
        cursorShape: containsMouse || pressed? Qt.SizeHorCursor: Qt.ArrowCursor
        onPressed: {
            mouseInitialX = mouse.x;
        }
        onMouseXChanged: {
            if(pressed) {
                mouseDeltaX = mouse.x - mouseInitialX;
                control.x += mouseDeltaX;
                control.width -= mouseDeltaX;
                if(control.x < 0)
                {
                    control.width += control.x;
                    control.x = 0;
                }
                mouseInitialX = 0;
            }
        }
        onClicked: {
            noteMouseArea.clicked(mouse);
        }
        onDoubleClicked: {
            noteMouseArea.doubleClicked(mouse);
        }
        z: 2
    }
    MouseArea {
        id: rightResizeArea
        anchors.right: parent.right
        width: 5
        hoverEnabled: true
        height: parent.height
        // 鼠标离开 MouseArea 后，即使当前是 pressed，光标也会变回箭头。
        cursorShape: containsMouse || pressed? Qt.SizeHorCursor: Qt.ArrowCursor
        onPressed: {
            mouseInitialX = mouse.x;
        }
        onMouseXChanged: {
            if(pressed) {
                mouseDeltaX = mouse.x - mouseInitialX;
                control.width += mouseDeltaX;
                mouseInitialX = 0;
            }
        }
        onClicked: {
            noteMouseArea.clicked(mouse);
        }
        onDoubleClicked: {
            noteMouseArea.doubleClicked(mouse);
        }
        z: 2
    }
}
