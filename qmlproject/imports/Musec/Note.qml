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
    property int startTime: 0
    property int duration: 96
    width: 100
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
        font.family: Constants.myMonoFont.name
        font.bold: true
        font.pixelSize: control.height - 2
        color: "#000000"
    }
    MouseArea {
        id: noteMouseArea
        anchors.fill: parent
        property int initialX: 0
        property int initialY: 0
        property bool settingVelocity: false
        onPressed: (mouse) => {
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
        onMouseXChanged: (mouse) => {
            if(pressed) {
                if(settingVelocity) {
                    var delta = mouse.y - initialY;
                    control.velocity -= delta;
                    if(control.velocity > Constants.maxVelocity) {
                        control.velocity = Constants.maxVelocity;
                    }
                    else if(control.velocity < 0) {
                        control.velocity = 0;
                    }
                }
                else {
                    var contentAreaCoordinate = control.mapToItem(control.parent, mouse.x, mouse.y);
                    var delta = contentAreaCoordinate.x - initialX;
                    control.x += delta;
                    if(control.x < 0) {
                        control.x = 0;
                    }
                    initialX = contentAreaCoordinate.x;
                }
            }
        }
        onMouseYChanged: (mouse) => {
            if(pressed) {
                if(settingVelocity) {
                    control.velocity -= mouse.y - initialY;
                    if(control.velocity > Constants.maxVelocity) {
                        control.velocity = Constants.maxVelocity;
                    }
                    else if(control.velocity < 0) {
                        control.velocity = 0;
                    }
                }
                else {
                    if(mouse.y > control.height) {
                        var down = Math.floor(mouse.y / (control.height + 1));
                        if(control.noteMidiNum - down < 0) {
                            down = control.noteMidiNum;
                        }
                        control.y += (down * (control.height + 1));
                        control.noteMidiNum -= down;
                    }
                    if(mouse.y < 0) {
                        var up = Math.floor((1 - mouse.y + control.height) / (control.height + 1));
                        if(control.noteMidiNum + up > Constants.midiValueRange - 1) {
                            up = Constants.midiValueRange - 1 - control.noteMidiNum;
                        }
                        control.y -= (up * (control.height + 1));
                        control.noteMidiNum += up;
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
        onPressed: (mouse) => {
            mouseInitialX = mouse.x;
        }
        onMouseXChanged: (mouse) => {
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
        onClicked: (mouse) => {
            noteMouseArea.clicked(mouse);
        }
        onDoubleClicked: (mouse) => {
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
        cursorShape: containsMouse || pressed? Qt.SizeHorCursor: Qt.ArrowCursor
        onPressed: (mouse) => {
            mouseInitialX = mouse.x;
        }
        onMouseXChanged: (mouse) => {
            if(pressed) {
                mouseDeltaX = mouse.x - mouseInitialX;
                control.width += mouseDeltaX;
                mouseInitialX = 0;
            }
        }
        onClicked: (mouse) => {
            noteMouseArea.clicked(mouse);
        }
        onDoubleClicked: (mouse) => {
            noteMouseArea.doubleClicked(mouse);
        }
        z: 2
    }
}
