import QtQuick 2.15
import QtQuick.Controls 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    id: root
    property bool trackSelected: false
    readonly property int lineHeight: 20
    readonly property color itemUnselectedColor: Constants.backgroundColor
    readonly property color itemSelectedColor: Constants.currentElementColor
    property color trackColor
    property int trackIndex: 0
    property string trackName: "Track Name"
    property bool trackMute: false
    property bool trackSolo: false
    property bool trackRecord: false

    signal renameComplete(newName: string)
    signal setMute(newMute: bool)
    signal setSolo(newSolo: bool)
    signal setArmRecording(newArmRecording: bool)

    Popup {
        id: trackNameEditPopup
        width: firstLine.width - x
        height: firstLine.height
        x: colorBar.width + trackIndexBar.width
        y: firstLine.y + firstLine.height
        visible: false
        clip: false
        padding: 0
        background: Rectangle {
            color: Constants.backgroundColor
            border.width: 1
            border.color: Constants.borderColor
        }
        TextField {
            id: trackNameEdit
            focus: true
            anchors.fill: parent
            text: trackName
            color: Constants.contentColor1
            placeholderText: qsTr("键入轨道名称")
            placeholderTextColor: Constants.currentElementColor
            selectionColor: Constants.currentElementColor
            selectedTextColor: Constants.backgroundColor
            padding: 0
            background: Rectangle {
                color: Constants.backgroundColor
                border.width: 1
                border.color: Constants.borderColor
            }
            font.family: Constants.font
            onAccepted: {
                renameComplete(text);
                trackNameEditPopup.visible = false;
            }
        }
    }

    function rename() {
        trackNameEditPopup.visible = true;
        trackNameEdit.selectAll();
        trackNameEditPopup.forceActiveFocus();
    }

    Rectangle {
        anchors.fill: parent
        z: 0
        color: itemUnselectedColor
    }
    clip: true
    width: 400
    height: 50
    implicitHeight: lineHeight
    Row {
        z: 1
        Rectangle {
            id: colorBar
            width: 10
            height: root.height
            color: trackColor
        }
        Column {
            Rectangle {
                id: firstLine
                width: root.width
                height: lineHeight
                color: trackSelected? Constants.contentColor2: Constants.backgroundColor2
                Row {
                    Rectangle {
                        id: trackIndexBar
                        width: 25
                        height: lineHeight
                        color: "transparent"
                        Text {
                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: 0.5
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            color: trackSelected? "#000000": "#FFFFFF"
                            text: trackIndex
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Item {
                        id: trackNameBar
                        width: root.width - colorBar.width - trackIndexBar.width - 3 * height
                        height: lineHeight
                        Text {
                            anchors.left: parent.left
                            anchors.leftMargin: 5
                            anchors.verticalCenter: parent.verticalCenter
                            text: trackName
                            font.family: Constants.font
                            color: trackSelected? "#000000": "#FFFFFF"
                            elide: Text.ElideRight
                        }
                    }
                    Item {
                        id: muteButton
                        width: height
                        height: lineHeight
                        MCtrl.Button {
                            anchors.centerIn: parent
                            width: parent.width - 4
                            height: parent.height - 4
                            background: Rectangle {
                                anchors.fill: parent
                                border.width: 1
                                border.color: Constants.muteIndicatorColor
                                color: trackMute? Qt.darker(border.color, 1.25): Constants.backgroundColor
                            }
                            contentItem: Text {
                                anchors.centerIn: parent
                                text: "M"
                                font.family: "Noto Sans Mono"
                                font.styleName: "Condensed SemiBold"
                                color: Constants.contentColor1
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            onClicked: {
                                setMute(!trackMute);
                            }
                        }
                    }
                    Item {
                        id: soloButton
                        width: height
                        height: lineHeight
                        MCtrl.Button {
                            anchors.centerIn: parent
                            width: parent.width - 4
                            height: parent.height - 4
                            background: Rectangle {
                                anchors.fill: parent
                                border.width: 1
                                border.color: Constants.soloIndicatorColor
                                color: trackSolo? Qt.darker(border.color, 1.25): Constants.backgroundColor
                            }
                            contentItem: Text {
                                anchors.centerIn: parent
                                text: "S"
                                font.family: "Noto Sans Mono"
                                font.styleName: "Condensed SemiBold"
                                color: Constants.contentColor1
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            onClicked: {
                                setSolo(!trackSolo);
                            }
                        }
                    }
                    Item {
                        id: armRecordButton
                        width: height
                        height: lineHeight
                        MCtrl.Button {
                            anchors.centerIn: parent
                            width: parent.width - 4
                            height: parent.height - 4
                            background: Rectangle {
                                anchors.fill: parent
                                border.width: 1
                                border.color: Constants.recordIndicatorColor
                                color: trackRecord? Qt.darker(border.color, 1.25): Constants.backgroundColor
                            }
                            contentItem: Text {
                                anchors.centerIn: parent
                                text: "R"
                                font.family: "Noto Sans Mono"
                                font.styleName: "Condensed SemiBold"
                                color: Constants.contentColor1
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            onClicked: {
                                setArmRecording(!trackRecord);
                            }
                        }
                    }
                }
            }
        }
    }
}
