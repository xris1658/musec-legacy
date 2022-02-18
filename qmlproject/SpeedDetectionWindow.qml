import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    property double bpm: 127.5
    flags: Qt.Dialog
    id: root
    title: qsTr("测速")
    width: 200
    height: 260
    color: Constants.backgroundColor
    modality: Qt.WindowModal
    visible: true
    Grid {
        id: grid
        anchors.fill: parent
        anchors.margins: 10
        columns: 1
        columnSpacing: 10
        MCtrl.Button {
            text: qsTr("TAP")
            width: root.width - 10 * 2
            height: width
        }
        Item {
            width: root.width - 10 * 2
            height: bpmText.contentHeight
            Text {
                id: bpmText
                anchors.centerIn: parent
                text: bpm
                font.family: "Noto Sans Mono"
                font.styleName: "Condensed"
                font.bold: true
                font.pointSize: Constants.font.pointSize * 2
                color: Constants.contentColor1
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Grid {
//            anchors.horizontalCenter: parent.horizontalCenter
            rows: 1
            spacing: 10
            verticalItemAlignment: Grid.AlignVCenter
            horizontalItemAlignment: Grid.AlignHCenter
            MCtrl.Button {
                text: qsTr("取整(&R)")
                onClicked: {
                    bpm = Math.round(bpm);
                }
            }
            MCtrl.Button {
                text: qsTr("应用速度并关闭(&A)")
                onClicked: {
                    bpm = Math.round(bpm);
                }
            }
        }
    }
}
