import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    id: root

    property string fileName
    property string filePath
    property bool newProject

    flags: Qt.Dialog
    title: qsTr("Project Properties")
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
                id: projectPropertiesContent
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
                        text: qsTr("File")
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
                        text: qsTr("File Name") + ":"
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        width: 320
                        height: 20
                        text: fileName
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    Text {
                        width: 100
                        text: qsTr("Path") + ":"
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        width: 320
                        height: 20
                        text: filePath
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    Item {
                        width: 100
                        height: 20
                    }
                    Row {
                        spacing: 5
                        MCtrl.Button {
                            text: qsTr("&Copy Path")
                            font.family: Constants.font
                            height: 20
                            enabled: !newProject
                        }
                        MCtrl.Button {
                            text: qsTr("Sh&ow in Explorer")
                            font.family: Constants.font
                            height: 20
                            enabled: !newProject
                        }
                    }
                    Text {
                        width: 100
                        height: 20
                        text: qsTr("&Metadata")
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
                        text: qsTr("Title") + ":"
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        width: 320
                        height: 20
                        color: Constants.contentColor1
                        placeholderText: qsTr("Type Project Title")
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
                        font.family: Constants.font
                        selectByMouse: true
                        selectionColor: Constants.currentElementColor
                        selectedTextColor: Constants.backgroundColor
                    }
                    Text {
                        width: 100
                        text: qsTr("Author") + ":"
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        width: 320
                        height: 20
                        color: Constants.contentColor1
                        placeholderText: qsTr("Type author")
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
                        font.family: Constants.font
                        selectByMouse: true
                        selectionColor: Constants.currentElementColor
                        selectedTextColor: Constants.backgroundColor
                    }
                    Text {
                        width: 100
                        text: qsTr("Company") + ":"
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    TextField {
                        width: 320
                        height: 20
                        color: Constants.contentColor1
                        placeholderText: qsTr("Type company or organization")
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
                        font.family: Constants.font
                        selectByMouse: true
                        selectionColor: Constants.currentElementColor
                        selectedTextColor: Constants.backgroundColor
                    }
                    Text {
                        width: 100
                        text: qsTr("Genre") + ":"
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        width: 320
                        height: 20
                        TextField {
                            id: textFieldGenre
                            width: parent.width - parent.height
                            height: parent.height
                            color: Constants.contentColor1
                            placeholderText: qsTr("Type or select genre")
                            placeholderTextColor: Constants.currentElementColor
                            topPadding: 0
                            bottomPadding: 0
                            leftPadding: 5
                            rightPadding: 5
                            background: Rectangle {
                                color: comboBoxGenre.background.color
                                border.color: Constants.borderColor
                                clip: true
                            }
                            font.family: Constants.font
                            selectByMouse: true
                            selectionColor: Constants.currentElementColor
                            selectedTextColor: Constants.backgroundColor
                            z: 2
                        }
                        MCtrl.ComboBox {
                            id: comboBoxGenre
                            anchors.fill: parent
                            font.family: Constants.font
                            model: ["", "Genre 1", "Genre 2", "Genre 3", "Genre 4"]
                            onCurrentValueChanged: {
                                textFieldGenre.text = comboBoxGenre.currentValue;
                            }
                            z: 1
                        }
                    }
                    Item {
                        width: 100
                        height: 160
                            Text {
                                anchors.right: parent.right
                                anchors.top: parent.top
                            text: qsTr("Remarks") + ":"
                            font.family: Constants.font
                            color: Constants.contentColor1
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                    ScrollView {
                        width: 320
                        height: 160
                        TextArea {
                            width: parent.width
                            color: Constants.contentColor1
                            placeholderText: qsTr("Type other info...")
                            placeholderTextColor: Constants.currentElementColor
                            wrapMode: TextEdit.Wrap
                            font.family: Constants.font
                            background: Rectangle {
                                color: Constants.backgroundColor
                                border.color: Constants.borderColor
                            }
                            selectByMouse: true
                            selectionColor: Constants.currentElementColor
                            selectedTextColor: Constants.backgroundColor
                            topPadding: 0
                            bottomPadding: 0
                            leftPadding: 5
                            rightPadding: 5
                        }
                    }
                    Text {
                        width: 100
                        height: 20
                        text: qsTr("Mixer")
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
                        text: qsTr("Pan law") + ":"
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.ComboBox {
                        model: ["-6dB", "-3dB", "0dB"]
                        width: 100
                        height: 20
                    }
                    Text {
                        width: 100
                        text: qsTr("Compensate") + ":"
                        font.family: Constants.font
                        color: Constants.contentColor1
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    MCtrl.CheckBox {
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
                anchors.left: parent.left
                anchors.leftMargin: spacing
                anchors.verticalCenter: parent.verticalCenter
                horizontalItemAlignment: Grid.AlignLeft
                verticalItemAlignment: Grid.AlignVCenter
                MCtrl.Button {
                    text: qsTr("&Reset")
                    width: 75
                    height: 30
                    font.family: Constants.font
                }
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
                    text: Constants.okTextWithMnemonic
                    width: 75
                    height: 30
                    font.family: Constants.font
                    onClicked: {
                        root.close();
                        root.destroy();
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
