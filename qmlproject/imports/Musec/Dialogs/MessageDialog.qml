import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

// Qt 6 中移除了 QtQuick.Dialogs 中的 ColorDialog 和 MessageDialog,
// 此处使用自行实现的 MessageDialog.
// 讲个笑话: Types in Qt.labs modules are not guaranteed to remain
// compatible in future versions. - Qt Documentation
Window {
    flags: Qt.Dialog
    modality: Qt.ApplicationModal
    color: Constants.backgroundColor
    height: icon.height + dialogButtonBox.height + row.padding * 2
    minimumHeight: row.height + dialogButtonBox.height
    width: dialogButtonBox.contentWidth + dialogButtonBox.padding * 2
    minimumWidth: row.width
    property string message: qsTr("对话框消息文字")
    property alias standardButtons: dialogButtonBox.standardButtons
    title: qsTr("对话框示例")
    onVisibleChanged: {
        if(visible) {
//            width = minimumWidth;
        }
    }

    Row {
        id: row
        padding: dialogButtonBox.padding
        spacing: 10
        Rectangle {
            id: icon
            width: 64
            height: 64
            color: "transparent"
            border.color: Constants.borderColor
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: message
            color: Constants.contentColor1
            font: Constants.font
        }
    }
    DialogButtonBox {
        id: dialogButtonBox
        background: Rectangle {
            color: Constants.backgroundColor2
        }
        alignment: Qt.AlignRight
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: parent.width
        spacing: 10
//        width: parent.width
        delegate: MCtrl.Button {}
    }
}
