import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    property bool exporting: false
    flags: Qt.Dialog
    id: root
    title: qsTr("导出并轨")
    width: 450
    height: 600
    color: Constants.backgroundColor
    modality: Qt.WindowModal
    visible: true
    Column {
        Item {
            width: root.width
            height: root.height - buttons.height
            Item {
                id: mixdownSettingContent
                anchors.fill: parent
                anchors.margins: 5
                Grid {
                    columns: 2
                    columnSpacing: 10
                    rowSpacing: 5
                    verticalItemAlignment: Grid.AlignVCenter
                    horizontalItemAlignment: Grid.AlignLeft
                    Text {
                        width: 100
                        height: 20
                        text: qsTr("文件")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: 320
                        height: 20
                    }
                    Text {
                        width: 100
                        text: qsTr("文件名:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        width: 320
                        height: 20
                        color: Constants.contentColor1
                        placeholderText: qsTr("键入文件名")
                        placeholderTextColor: Constants.currentElementColor
                        topPadding: 0
                        bottomPadding: 0
                        leftPadding: 5
                        rightPadding: 5
                        background: Rectangle {
                            color: "transparent"
                            border.color: Constants.borderColor
                            clip: true
                        }
                        font {
                            family: Constants.font
                        }
                    }
                    Text {
                        width: 100
                        text: qsTr("目录:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        id: filePathSelector
                        width: 320
                        height: 20
                        Row {
                            id: filePathSelectorRow
                            spacing: 5
                            TextField {
                                id: filePathSelectorTextInput
                                width: filePathSelector.width - browseButton.width - filePathSelectorRow.spacing
                                height: 20
                                color: Constants.contentColor1
                                placeholderText: qsTr("键入路径或选择目录")
                                placeholderTextColor: Constants.currentElementColor
                                topPadding: 0
                                bottomPadding: 0
                                leftPadding: 5
                                rightPadding: 5
                                background: Rectangle {
                                    color: "transparent"
                                    border.color: Constants.borderColor
                                    clip: true
                                }
                                font {
                                    family: Constants.font
                                }
                            }
                            MCtrl.Button {
                                id: browseButton
                                width: 75
                                height: 20
                                text: qsTr("浏览(&B)...")
                                font.family: Constants.font
                                FolderDialog {
                                    id: folderDialog
                                    onAccepted: {
                                        filePathSelectorTextInput.text = folder;
                                    }
                                }
                                onClicked: {
                                    folderDialog.open();
                                }
                            }
                        }

                    }
                    Text {
                        width: 100
                        height: 20
                        text: qsTr("导出无损音频")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: 320
                        height: 20
                    }
                    Text {
                        width: 100
                        text: qsTr("格式: ")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: 100
                        height: 20
                        model: ["WAV", "FLAC", "AIFF"]
                        currentIndex: 0
                        font.family: Constants.font
                    }
                    Text {
                        width: 100
                        text: qsTr("采样率:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Row {
                        spacing: 5
                        MCtrl.ComboBox {
                            width: 100
                            height: 20
                            model: ["44100", "48000", "88200", "96000", "176400", "192000"]
                            currentIndex: 0
                            font.family: Constants.font
                        }
                        Text {
                            text: qsTr("Hz")
                            font.family: Constants.font
                            color: Constants.contentColor1
                        }
                    }
                    Text {
                        width: 100
                        text: qsTr("位深度:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: 100
                        height: 20
                        model: ["16位整型", "24位整型", "32位浮点", "64位浮点"]
                        currentIndex: 0
                        font.family: Constants.font
                        onCurrentIndexChanged: {
                            checkBoxDithering.enabled = currentIndex < 2;
                            if(!checkBoxDithering.enabled) {
                                checkBoxDithering.checked = false;
                            }
                        }
                    }
                    Text {
                        width: 100
                        text: qsTr("抖动:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Row {
                        id: rowDithering
                        spacing: 5
                        MCtrl.CheckBox {
                            id: checkBoxDithering
                            checked: false
                        }
                        MCtrl.ComboBox {
                            width: 100 - checkBoxDithering.width - rowDithering.spacing
                            height: 20
                            enabled: checkBoxDithering.checked
                            model: ["POW-r 1", "POW-r 2", "POW-r 3", "方波", "三角波"]
                            currentIndex: 0
                            font.family: Constants.font
                        }
                    }
                    Text {
                        opacity: checkBoxDithering.checked? 1: 0
                        width: 100
                        height: checkBoxDithering.checked? 40: 0
                        text: qsTr("警告:")
                        font.family: Constants.font
                        color: Constants.warningTextColor
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignTop
                    }
                    Text {
                        opacity: checkBoxDithering.checked? 1: 0
                        width: 320
                        height: checkBoxDithering.checked? 40: 0
                        text: qsTr("抖动操作只应在不对文件进行后续操作时使用。\n如果总线已经添加了抖动操作，请取消勾选此选项。")
                        font.family: Constants.font
                        color: Constants.warningTextColor
                        wrapMode: Text.Wrap
                    }

                    Text {
                        width: 100
                        height: 20
                        text: qsTr("高级设置")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: 320
                        height: 20
                    }
                    Text {
                        width: 100
                        text: qsTr("导出为单声道:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.CheckBox {
                        checked: false
                    }
                    Text {
                        width: 100
                        text: qsTr("标准化:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: 100
                        height: 20
                        model: ["无", "0dB", "-0.1dB", "-3dB"]
                        currentIndex: 0
                        font.family: Constants.font
                    }
                    Text {
                        width: 100
                        text: qsTr("尾部处理方式:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: 100
                        height: 20
                        model: ["不保留", "保留（置于末尾）", "保留（置于开头）"]
                        currentIndex: 0
                        font.family: Constants.font
                    }
                    Text {
                        width: 100
                        height: 20
                        text: qsTr("导出有损压缩音频")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: 320
                        height: 20
                    }
                    Text {
                        width: 100
                        text: qsTr("文件格式:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: 100
                        height: 20
                        model: ["AAC", "MP3", "M4A", "OGG", "WMA"]
                        currentIndex: 0
                        font.family: Constants.font
                    }
                    Text {
                        width: 100
                        height: 20
                        text: qsTr("导出完成后操作")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: 320
                        height: 20
                    }
                    Text {
                        width: 100
                        text: qsTr("关闭此窗口:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.CheckBox {
                        checked: false
                    }
                    Text {
                        width: 100
                        text: qsTr("打开导出目录:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.CheckBox {
                        checked: false
                    }
                    Text {
                        width: 100
                        text: qsTr("执行命令:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        width: 320
                        height: 20
                        color: Constants.contentColor1
                        placeholderText: qsTr("键入命令")
                        placeholderTextColor: Constants.currentElementColor
                        topPadding: 0
                        bottomPadding: 0
                        leftPadding: 5
                        rightPadding: 5
                        background: Rectangle {
                            color: "transparent"
                            border.color: Constants.borderColor
                            clip: true
                        }
                        font {
                            family: Constants.font
                        }
                    }
                    Text {
                        width: 100
                        height: 80
                        text: qsTr("提示:")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        verticalAlignment: Text.AlignTop
                        horizontalAlignment: Text.AlignRight
                    }
                    Text {
                        width: 320
                        height: 80
                        textFormat: Text.RichText
                        text: qsTr("用%path作为导出位置；用%filename作为文件名（不含扩展名），用%format作为无损音频文件的扩展名。<br>例：导出的无损音频文件的完整路径为<br>%path\\%filename.%format")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        wrapMode: Text.Wrap
                    }
                }
            }
            MCtrl.ProgressBar {
                id: exportProgress
                opacity: root.exporting? 1.0: 0.0
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                height: 20
                SequentialAnimation on value {
                    id: seqAnimation
                    running: false
                    NumberAnimation {
                        id: demo
                        from: 0
                        to: 1
                        easing.type: Easing.Linear
                        duration: 10000
                    }
                    onFinished: {
                        root.close();
                        root.destroy();
                    }
                }
            }
        }
        Item {
            id: buttons
            width: root.width
            height: 40
            Rectangle {
                anchors.fill: parent
                color: Constants.backgroundColor2
            }
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
                    text: qsTr("导出(&E)")
                    width: 75
                    height: 30
                    font.family: Constants.font
                    onClicked: {
                        exporting = true;
                        exportProgress.value = 0.0;
                        seqAnimation.start();
                    }
                }
                MCtrl.Button {
                    id: buttonCancel
                    text: qsTr("取消(&C)")
                    width: 75
                    height: 30
                    font.family: Constants.font
                    onClicked: {
                        root.close();
                        root.destroy();
                    }
                }
            }
        }

    }
}
