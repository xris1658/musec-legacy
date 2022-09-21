import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as QQC2

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    flags: Qt.Dialog
    id: root
    title: qsTr("Third-party applications used by Musec")
    color: Constants.backgroundColor
    modality: Qt.WindowModal
    visible: true
    width: 500
    height: 500
    Component.onCompleted: {
        buttonOK.forceActiveFocus();
    }
    Item {
        width: root.width
        anchors.top: parent.top
        anchors.bottom: buttons.top
        Rectangle {
            anchors.fill: parent
            anchors.margins: 10
            color: Constants.backgroundColor
            border.color: Constants.borderColor
            clip: true
            ListView {
                id: supportList
                anchors.fill: parent
                anchors.margins: parent.border.width
                model: Constants.technicalSupport
                interactive: false
                delegate: Rectangle {
                    width: supportList.width
                    height: 20
                    color: index % 2? Constants.backgroundColor: Constants.backgroundColor2
                    Row {
                        width: parent.width
                        height: parent.height
                        Text {
                            width: parent.width * 0.4
                            text: "<a href=\"%1\">%2</a>".arg(homepage).arg(name)
                            anchors.verticalCenter: parent.verticalCenter
                            padding: 5
                            font.family: Constants.font
                            color: Constants.contentColor1
                            linkColor: "#0080FF"
                            QQC2.Control {
                                id: hover
                                opacity: 0
                                x: 5
                                width: parent.contentWidth
                                height: parent.contentHeight
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            MCtrl.ToolTip {
                                text: qsTr("<p><b>Click to visit the following link:</b></p>%1")
                                       .arg(homepage)
                                visible: hover.hovered
                            }
                            onLinkActivated: (link) => {
                                Qt.openUrlExternally(link);
                            }
                        }
                        Text {
                            width: parent.width * 0.6
                            text: description
                            anchors.verticalCenter: parent.verticalCenter
                            padding: 5
                            font.family: Constants.font
                            color: Constants.contentColor1
                        }
                    }
                }
            }
        }
    }
    Rectangle {
        id: buttons
        width: root.width
        anchors.bottom: parent.bottom
        height: 40
        color: Constants.backgroundColor2
        Grid {
            rows: 1
            spacing: 5
            anchors.right: parent.right
            anchors.rightMargin: spacing
            anchors.verticalCenter: parent.verticalCenter
            horizontalItemAlignment: Grid.AlignRight
            verticalItemAlignment: Grid.AlignVCenter
            MCtrl.Button {
                id: buttonOK
                text: Constants.okTextWithMnemonic
                width: 75
                height: 30
                font.family: Constants.font
                onClicked: {
                    close();
                }
            }
        }
    }
}
