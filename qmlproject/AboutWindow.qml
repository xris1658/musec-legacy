import QtQuick 2.15
import QtQuick.Window 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    flags: Qt.Dialog
    id: root
    title: qsTr("About Musec")
    color: Constants.backgroundColor
    modality: Qt.WindowModal
    visible: true
    width: 600
    height: 450
    Component.onCompleted: {
        buttonOK.forceActiveFocus();
    }
    Column {
        Row {
            id: mainRow
            Item {
                id: iconRow
                width: 100
                height: root.height - buttons.height
                Item {
                    id: icon
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: (parent.width - width) / 2
                    width: 64
                    height: 64
                    Image {
                        source: "images/Musec-image-2.ico"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        layer.enabled: true
                        layer.smooth: true
                        layer.textureSize: Qt.size(width * 2, height * 2)
                    }
                }
            }
            Item {
                id: textRow
                width: root.width - iconRow.width - 10
                height: root.height - buttons.height
                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    topPadding: 10
                    bottomPadding: 10
                    spacing: 5
                    Text {
                        text: Strings.applicationName
                        color: Constants.contentColor1
                        font.family: Constants.myFont.name
                        font.bold: true
                        font.pixelSize: 20
                    }
                    Text {
                        text: "Development Version"
                        color: Constants.contentColor1
                        font.family: Constants.font
                    }
                    Text {
                        text: "Copyright (c) 2021-2023 xris1658. All rights reserved."
                        color: Constants.contentColor1
                        font.family: Constants.font
                    }
                    Text {
                        text: qsTr("Musec is a digital audio workstation (DAW) software under development.")
                        color: Constants.contentColor1
                        font.family: Constants.font
                    }
                }
            }
        }
        Rectangle {
            id: buttons
            width: root.width
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
                MCtrl.Button {
                    id: buttonTechnicalSupport
                    text: qsTr("&Third Party Software")
                    height: 30
                    font.family: Constants.font
                    onClicked: {
                        var component = Qt.createComponent("TechnicalSupportWindow.qml");
                        if(component.status == Component.Ready) {
                            var aboutWindow = component.createObject(root);
                            EventBridge.setWindowIcon(aboutWindow);
                            aboutWindow.showNormal();
                        }
                    }
                }
            }
        }
    }
}
