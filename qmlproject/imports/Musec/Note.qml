import QtQuick 2.15
import QtQuick.Shapes 1.15
//import Musec 1.0

Item {
    id: control
//    property color noteBackgroundColor: noteMouseArea.pressed? Qt.darker(Constants.noteBackgroundColor, 1.1): Constants.noteBackgroundColor
    property int noteNum
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
        visible: rightResizeArea.hovered | rightResizeArea.pressed
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
        visible: leftResizeArea.hovered | leftResizeArea.pressed
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
        text: qsTr(noteName)
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
            if(mouse.modifiers & Qt.AltModifier) {
                settingVelocity = true;
            }
            else {
                var contentAreaCoordinate = control.mapToItem(control.parent, mouseX, mouseY);
                initialX = contentAreaCoordinate.x;
            }
        }
        onReleased: {
            settingVelocity = false;
        }
        onMouseXChanged: {
            if(pressed && (!settingVelocity)) {
                var contentAreaCoordinate = control.mapToItem(control.parent, mouseX, mouseY);
                var delta = contentAreaCoordinate.x - initialX;
                control.x += delta;
                if(control.x < 0) {
                    control.x = 0;
                }
                initialX = contentAreaCoordinate.x;
            }
        }
        onMouseYChanged: {
            if(pressed && settingVelocity) {
                control.velocity -= mouseY - initialY;
                if(control.velocity > Constants.maxVelocity) {
                    control.velocity = Constants.maxVelocity;
                }
                else if(control.velocity < 0) {
                    control.velocity = 0;
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
        property bool hovered: false
        anchors.left: parent.left
        width: 5
        height: parent.height
        hoverEnabled: true
        cursorShape: hovered || pressed? Qt.SizeHorCursor: Qt.ArrowCursor
        onEntered: {
            hovered = true;
        }
        onExited: {
            hovered = false;
        }
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
        property bool hovered: false
        anchors.right: parent.right
        width: 5
        hoverEnabled: true
        height: parent.height
        // 鼠标离开 MouseArea 后，即使当前是 pressed，光标也会变回箭头。原因未知。
        cursorShape: hovered || pressed? Qt.SizeHorCursor: Qt.ArrowCursor

        onEntered: {
            hovered = true;
        }
        onExited: {
            hovered = false;
        }
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
