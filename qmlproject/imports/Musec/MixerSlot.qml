import QtQuick 2.15
import QtQuick.Controls 2.15

import Musec.Controls 1.0 as MCtrl

MCtrl.Button {
    id: root
    height: 20
    property bool slotEnabled: true
    property string name: "Sample Text"
    property bool sidechainExist: true
    property bool sidechainEnabled: false
    property bool editorVisible: false
    implicitWidth: 100
    signal entered(drag: var)
    signal dropped(drop: var)
    signal exited()
    signal positionChanged(drag: var)
    color: editorVisible? Constants.deviceEnabledColor: Constants.backgroundColor
    Row {
        Item {
            id: enabledIndicatorArea
            width: height
            height: root.height
            Button {
                anchors.fill: parent
                anchors.margins: 3
                background: Rectangle {
                    radius: width / 2
                    border.color: Constants.borderColor
                    color: root.slotEnabled? Constants.deviceEnabledColor: Constants.deviceDisabledColor
                }
            }
        }
        Text {
            width: root.width - enabledIndicatorArea.width - (root.sidechainExist? sidechainIndicatorArea.width: 0)
            anchors.verticalCenter: parent.verticalCenter
            text: root.name
            color: root.editorVisible? Constants.backgroundColor: Constants.contentColor1
            font.family: Constants.font
        }
        Item {
            id: sidechainIndicatorArea
            width: height
            height: root.height
            visible: root.sidechainExist
            Text {
                anchors.centerIn: parent
                font.family: "Noto Sans Mono"
                font.styleName: "Condensed SemiBold"
                color: root.sidechainEnabled? Constants.contentColor1: Constants.contentColor2
                text: "SC"
            }
        }
    }
    DropArea {
        id: dropArea
        anchors.fill: parent
        onEntered: {
            root.entered(drag);
        }
        onExited: {
            root.exited();
        }
        onDropped: {
            root.dropped(drop);
        }
        onPositionChanged: {
            root.positionChanged(drag);
        }
    }
}
