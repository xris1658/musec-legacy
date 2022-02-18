import QtQuick 2.15
import QtQuick.Window 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    flags: Qt.Dialog
    id: root
    title: qsTr("关于 Qt")
    color: Constants.backgroundColor
    modality: Qt.WindowModal
    visible: true
    width: 600
    height: mainRow.height + buttons.height
    Column {
        Row {
            id: mainRow
            spacing: 10
            anchors.margins: 10
            height: texts.height - 10
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
                        source: "images/qtlogo-64.png"
                        anchors.fill: parent
                    }
                }
            }
            Item {
                id: textRow
                width: root.width - iconRow.width - 10
                height: root.height - buttons.height
                Column {
                    id: texts
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    topPadding: 10
                    bottomPadding: 10
                    spacing: 5
                    Text {
                        text: qsTr("关于 Qt")
                        color: Constants.contentColor1
                        font.family: "Noto Sans CJK SC Bold"
                        font.pixelSize: 20
                    }
                    Text {
                        text: qsTr("This program uses Qt version %1.").arg("5.15.2")
                        color: Constants.contentColor1
                        font: Constants.font
                        anchors.left: parent.left
                        anchors.right: parent.right
                        wrapMode: Text.Wrap
                    }
                    Text {
                        text: qsTr(
                            "<p>Qt is a C++ toolkit for cross-platform application development.</p>
                             <p>Qt provides single-source portability across all major desktop operating systems. It is also available for embedded Linux and other embedded and mobile operating systems.</p>
                             <p>Qt is available under multiple licensing options designed to accommodate the needs of our various users.</p>
                             <p>Qt licensed under our commercial license agreement is appropriate for development of proprietary/commercial software where you do not want to share any source code with third parties or otherwise cannot comply with the terms of GNU (L)GPL.</p>
                             <p>Qt licensed under GNU (L)GPL is appropriate for the development of Qt&nbsp;applications provided you can comply with the terms and conditions of the respective licenses.</p>
                             <p>Please see <a href=\"http://%2/\">%2</a> for an overview of Qt licensing.</p>
                             <p>Copyright (C) %1 The Qt Company Ltd and other contributors.</p><p>Qt and the Qt logo are trademarks of The Qt Company Ltd.</p>
                             <p>Qt is The Qt Company Ltd product developed as an open source project. See <a href=\"http://%3/\">%3</a> for more information.</p>"
                        ).arg("2020").arg("qt.io/licensing").arg("qt.io")
                        color: Constants.contentColor1
                        linkColor: "#0080FF"
                        font: Constants.font
                        anchors.left: parent.left
                        anchors.right: parent.right
                        wrapMode: Text.Wrap
                        onLinkActivated: {
                            Qt.openUrlExternally(link);
                        }
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
                    text: qsTr("确定(&O)")
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
}
