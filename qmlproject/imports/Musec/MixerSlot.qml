import QtQuick 2.15
import QtQuick.Controls 2.15

import Musec.Controls 1.0 as MCtrl

MCtrl.Button {
    id: root
    height: 20
    property bool slotEnabled: true
    property string name: "Sample Text abcdefghjiijklmn"
    property bool sidechainExist: true
    property bool sidechainEnabled: false
    property bool editorVisible: false
    implicitWidth: 100
    signal entered(drag: var)
    signal dropped(drop: var)
    signal exited()
    signal positionChanged(drag: var)
    color: hovered? Qt.lighter(editorVisible? Constants.currentElementColor: Constants.backgroundColor):
                    editorVisible? Constants.currentElementColor: Constants.backgroundColor
    function getFullWidth() {
        var ret = enabledIndicatorArea.width + pluginNameText.contentWidth;
        console.log(ret);
        return ret;
    }

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
    Rectangle {
        anchors.left: enabledIndicatorArea.right
        width: pluginNameText.width
        height: root.height - root.border.width * 2
        anchors.verticalCenter: parent.verticalCenter
        color: root.hovered? root.color: "transparent"
        Text {
            id: pluginNameText
            width: root.hovered? contentWidth + root.height - contentHeight: root.width - enabledIndicatorArea.width - (root.sidechainExist? sidechainIndicatorArea.width: 0)
            anchors.verticalCenter: parent.verticalCenter
            text: root.name
            color: root.editorVisible? Constants.backgroundColor: Constants.contentColor1
            font.family: Constants.font
            elide: root.hovered? Text.ElideNone: Text.ElideRight
        }
    }

    Item {
        anchors.right: parent.right
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
