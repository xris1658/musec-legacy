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
    title: qsTr("Bounce")
    width: 520
    height: 600
    property int firstColumnWidth: 150
    property int secondColumnWidth: 350
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
                        width: firstColumnWidth
                        height: 20
                        text: qsTr("File")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: secondColumnWidth
                        height: 20
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("File Name:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        width: secondColumnWidth
                        height: 20
                        color: Constants.contentColor1
                        placeholderText: qsTr("Type file name")
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
                        width: firstColumnWidth
                        text: qsTr("Path:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        id: filePathSelector
                        width: secondColumnWidth
                        height: 20
                        Row {
                            id: filePathSelectorRow
                            spacing: 5
                            TextField {
                                id: filePathSelectorTextInput
                                width: filePathSelector.width - browseButton.width - filePathSelectorRow.spacing
                                height: 20
                                color: Constants.contentColor1
                                placeholderText: qsTr("Type path or select a directory")
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
                                text: qsTr("&Browse...")
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
                        width: firstColumnWidth
                        height: 20
                        text: qsTr("Export Lossless Audio File")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: secondColumnWidth
                        height: 20
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("Format: ")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: firstColumnWidth
                        height: 20
                        model: ["WAV", "FLAC", "AIFF"]
                        currentIndex: 0
                        font.family: Constants.font
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("Sample Rate:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Row {
                        spacing: 5
                        MCtrl.ComboBox {
                            width: firstColumnWidth
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
                        width: firstColumnWidth
                        text: qsTr("Bit Depth:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: firstColumnWidth
                        height: 20
                        model: [qsTr("16-Bit Integer"), qsTr("24-Bit Integer"), qsTr("32-Bit Float"), qsTr("64-Bit Integer")]
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
                        width: firstColumnWidth
                        text: qsTr("Dithering:")
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
                            width: firstColumnWidth - checkBoxDithering.width - rowDithering.spacing
                            height: 20
                            enabled: checkBoxDithering.checked
                            model: ["POW-r 1", "POW-r 2", "POW-r 3", qsTr("Square"), qsTr("Triangular")]
                            currentIndex: 0
                            font.family: Constants.font
                        }
                    }
                    Text {
                        opacity: checkBoxDithering.checked? 1: 0
                        width: firstColumnWidth
                        height: checkBoxDithering.checked? 40: 0
                        text: qsTr("Warning:")
                        font.family: Constants.font
                        color: Constants.warningTextColor
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignTop
                    }
                    Text {
                        opacity: checkBoxDithering.checked? 1: 0
                        width: secondColumnWidth
                        height: checkBoxDithering.checked? 40: 0
                        text: qsTr("Use dithering only when the audio file won't be processed afterwards. \nIf you add dithering devices on the master track, please uncheck this option.")
                        font.family: Constants.font
                        color: Constants.warningTextColor
                        wrapMode: Text.Wrap
                    }

                    Text {
                        width: firstColumnWidth
                        height: 20
                        text: qsTr("Advanced")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: secondColumnWidth
                        height: 20
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("Export as Mono:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.CheckBox {
                        checked: false
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("标准化:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: firstColumnWidth
                        height: 20
                        model: [qsTr("N/A"), "0dB", "-0.1dB", "-3dB"]
                        currentIndex: 0
                        font.family: Constants.font
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("Tails:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: firstColumnWidth
                        height: 20
                        model: [qsTr("Cut"), qsTr("Leave"), qsTr("Wrap")]
                        currentIndex: 0
                        font.family: Constants.font
                    }
                    Text {
                        width: firstColumnWidth
                        height: 20
                        text: qsTr("Export Lossy Audio File")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: secondColumnWidth
                        height: 20
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("File Format:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        width: firstColumnWidth
                        height: 20
                        model: ["AAC", "MP3", "M4A", "OGG", "WMA"]
                        currentIndex: 0
                        font.family: Constants.font
                    }
                    Text {
                        width: firstColumnWidth
                        height: 20
                        text: qsTr("After Exporting")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: secondColumnWidth
                        height: 20
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("Close This Window:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.CheckBox {
                        checked: false
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("Open Export Directory:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.CheckBox {
                        checked: false
                    }
                    Text {
                        width: firstColumnWidth
                        text: qsTr("Execute a Command:")
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        width: secondColumnWidth
                        height: 20
                        color: Constants.contentColor1
                        placeholderText: qsTr("Type command")
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
                        width: firstColumnWidth
                        height: 80
                        text: qsTr("Tip:")
                        font.family: Constants.font
                        color: Constants.contentColor2
                        verticalAlignment: Text.AlignTop
                        horizontalAlignment: Text.AlignRight
                    }
                    Text {
                        width: secondColumnWidth
                        height: 80
                        textFormat: Text.RichText
                        text: qsTr("Use %path as export directory; use %filename as file name without the extension，use %format as extension of the lossless audio file.<br>e.g. the complete path of the exported lossless audio file is <br>%path\\%filename.%format")
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
                    text: qsTr("&Export")
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
                    text: Constants.cancelTextWithMnemonic
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
