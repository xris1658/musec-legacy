import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    id: root
    width: 80
    height: 320
    clip: true
    Rectangle {
        anchors.fill: parent
        color: Constants.backgroundColor
    }
    property bool midiEffect: true
    property bool midiToAudio: true
    property ListModel list: ListModel {
        id: effects
        dynamicRoles: true
/*
        ListElement {
            name: //插件名
            effectEnabled: //插件启用状态
            sidechainInputEnabled: //插件侧链输入存在
            sidechainInputArmed: //插件侧链输入接入
        }
*/
    }
    property bool muted: false
    property bool solo: false
    property bool inverted: false
    property bool armRecording: false
    property real panning: 0.0
    property real stereo: 1.0
    property real gain: 1.00
    property real peak: 0.00
    property string channelName
    property color channelColor
    property int channelNumber: 0
    Column {
        width: parent.width
        height: parent.height
        Item {
            id: channelEffects
            width: parent.width
            height: (root.height - channelControl.height - channelStereo.height - channelInfo.height) / 2
            clip: true
            Column {
                Repeater {
                    model: effects
                    delegate: Item {
                        width: root.width
                        height: 20
                        Rectangle {
                            id: effect
                            anchors.centerIn: parent
                            width: parent.width - 2
                            height: parent.height - 2
                            color: Constants.backgroundColor2
                            border.width: 1
                            border.color: Constants.borderColor
                            Row {
                                Item {
                                    width: effect.height
                                    height: effect.height
                                    Rectangle {
                                        anchors.centerIn: parent
                                        width: 12
                                        height: 12
                                        radius: 6
                                        color: effectEnabled == true? Constants.deviceEnabledColor
                                                                    : Constants.deviceDisabledColor
                                        border.width: 1
                                        border.color: Constants.backgroundColor
                                    }
                                }
                                Text {
                                    color: Constants.contentColor1
                                    font.family: Constants.font
                                    text: name
                                    elide: Text.ElideRight
                                }
                            }
                            Item {
                                opacity: sidechainInputEnabled? 1: 0
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                width: parent.height
                                height: width
                                Item {
                                    opacity: sidechainInputArmed? 1: 0.5
                                    anchors.centerIn: parent
                                    width: 12
                                    height: 10
                                    Rectangle {
                                        anchors.top: parent.top
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        width: 6
                                        height: 5
                                        color: Constants.contentColor1
                                    }
                                    Rectangle {
                                        anchors.left: parent.left
                                        anchors.top: parent.top
                                        anchors.topMargin: 2
                                        width: 2
                                        height: 2
                                        color: Constants.contentColor1
                                    }
                                    Rectangle {
                                        anchors.right: parent.right
                                        anchors.top: parent.top
                                        anchors.topMargin: 2
                                        width: 2
                                        height: 2
                                        color: Constants.contentColor1
                                    }
                                    Rectangle {
                                        anchors.bottom: parent.bottom
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        width: 2
                                        height: 4
                                        color: Constants.contentColor1
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Item {
            id: channelControl
            width: parent.width
            height: 30
            clip: true
            Row {
                Item {
                    width: root.width / 4
                    height: 30
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 6
                        border.width: 2
                        border.color: Constants.muteIndicatorColor
                        color: muted? Qt.darker(border.color, 1.5) : Constants.backgroundColor
                        Text {
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "M"
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pixelSize: 18
                            color: Constants.contentColor1
                        }
                        onClicked: {
                            muted = !muted;
                        }
                    }
                }
                Item {
                    width: root.width / 4
                    height: 30
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 6
                        border.width: 2
                        border.color: Constants.soloIndicatorColor
                        color: solo? Qt.darker(border.color, 1.5) : Constants.backgroundColor
                        Text {
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "S"
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pixelSize: 18
                            color: Constants.contentColor1
                        }
                        onClicked: {
                            solo = !solo;
                        }
                    }
                }
                Item {
                    width: root.width / 4
                    height: 30
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 6
                        border.width: 2
                        border.color: Constants.invertIndicatorColor
                        color: inverted? border.color : Constants.backgroundColor
                        Text {
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "I"
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pixelSize: 18
                            color: Constants.contentColor1
                        }
                        onClicked: {
                            inverted = !inverted;
                        }
                    }
                }
                Item {
                    width: root.width / 4
                    height: 30
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 6
                        border.width: 2
                        border.color: Constants.recordIndicatorColor
                        color: armRecording? border.color : Constants.backgroundColor
                        Text {
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "R"
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pixelSize: 18
                            color: Constants.contentColor1
                        }
                        onClicked: {
                           armRecording = !armRecording;
                        }
                    }
                }
            }
        }
        Item {
            id: channelStereo
            width: parent.width
            height: 20
            clip: true
            Row {
                Item {
                    width: root.width / 2
                    height: 20
                    Rectangle {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 2
                        color: "transparent"
                        border.width: 1
                        border.color: Constants.borderColor
                        z: 2
                        clip: true
                    }
                    Rectangle {
                        color: "#1D2088"
                        width: panning>0? parent.width * panning / 2 : parent.width * panning / -2
                        height: parent.height - 2
                        x: panning>0? parent.width / 2: parent.width / 2 - width
                        y: 1
                        z: 0
                    }
                    Text {
                        anchors.centerIn: parent
                        color: Constants.contentColor1
                        font.family: Constants.font
                        text: panning == 0? "C" : panning < 0? panning * -100 + "L" : panning * 100 + "R"
                        z: 1
                    }
                }
                Item {
                    width: root.width / 2
                    height: 20
                    Rectangle {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 2
                        color: "transparent"
                        border.width: 1
                        border.color: Constants.borderColor
                        z: 2
                        clip: true
                    }
                    Text {
                        anchors.centerIn: parent
                        color: Constants.contentColor1
                        font.family: Constants.font
                        text: stereo == 0? "Mono" : stereo * 100 + "%"
                        z: 1
                    }
                }
            }
        }
        Item {
            id: channelGainAndPeak
            width: parent.width
            height: (root.height - channelControl.height - channelStereo.height - channelInfo.height) / 2
            clip: true
            Row {
                Column {
                    Item {
                        width: root.width / 2
                        height: channelGainAndPeak.height - 35
                        MCtrl.Slider {
                            topPadding: 5
                            id: mixerChannelSlider
                            implicitThickness: 4
                            handleImplicitThickness: 12
                            handleImplicitLength: 16
                            anchors.horizontalCenter: parent.horizontalCenter
                            orientation: Qt.Vertical
                            width: 20
                            height: parent.height
                            from: -96
                            to: 6
                            value: 20 * Math.log10(gain)
                            onValueChanged: {
                                gain = Math.pow(10.0, value * 0.05);
                            }
                        }
                    }
                    Item {
                        width: root.width / 2
                        height: 35
                        Popup {
                            width: parent.width * 0.75
                            height: 20
                            anchors.centerIn: parent
                            padding: 0
                            id: gainTextInputPopup
                            visible: false
                            clip: false
                            background: Rectangle {
                                color: Constants.backgroundColor
                                border.width: 1
                                border.color: Constants.borderColor
                            }
                            TextField {
                                id: gainTextInput
                                focus: true
                                anchors.fill: parent
                                text: mixerChannelGainText.text
                                color: Constants.contentColor1
                                selectionColor: Constants.currentElementColor
                                selectedTextColor: Constants.backgroundColor
                                horizontalAlignment: TextInput.AlignHCenter
                                placeholderText: "0.00"
                                placeholderTextColor: Constants.currentElementColor
                                padding: 0
                                inputMethodHints: Qt.ImhFormattedNumbersOnly
                                background: Rectangle {
                                    color: Constants.backgroundColor
                                    border.width: 1
                                    border.color: Constants.borderColor
                                }
                                font.family: Constants.font
                                onAccepted: {
                                    gain = text.length == 0? 1.0: Math.pow(10.0, text * 0.05);
                                    gainTextInputPopup.visible = false;
                                }
                            }
                        }
                        Text {
                            id: mixerChannelGainText
                            anchors.centerIn: parent
                            text: gain == 0? "-inf": (20 * Math.log10(gain)).toPrecision(3)
                            font.family: Constants.font
                            color: Constants.contentColor1
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                gainTextInputPopup.visible = true;
                                gainTextInput.selectAll();
                                gainTextInput.forceActiveFocus();
                            }
                        }
                    }
                }
                Column {
                    Item {
                        id: levelPlaceholder
                        width: root.width / 2
                        height: channelGainAndPeak.height - 35
                        Row {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 5
                            topPadding: 5
                            Level {
                                width: 10
                                height: levelPlaceholder.height - 5
                                level: peak
                            }
                            Level {
                                width: 10
                                height: levelPlaceholder.height- 5
                                level: peak
                            }
                        }
                    }
                    Item {
                        width: root.width / 2
                        height: 35
                        Rectangle {
                            anchors.centerIn: parent
                            width: parent.width * 0.75
                            height: 20
                            color: peak > 1 ? Constants.peakAboveZeroColor : Constants.backgroundColor
                            border.width: 1
                            border.color: Constants.borderColor
                            Text {
                                anchors.centerIn: parent
                                text: peak == 0? "-inf": (20 * Math.log10(peak)).toPrecision(3)
                                font.family: Constants.font
                                color: peak > 1? "#000000" : Constants.contentColor1
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            id: channelInfo
            width: parent.width
            height: 25
            color: channelColor
            Row {
                Item {
                    id: mixerChannelIndex
                    width: channelNumber? 30: 0
                    height: 25
                    clip: true
                    Text {
                        id: textInfoIndex
                        anchors.centerIn: parent
                        text: channelNumber
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        color: channelColor.r  * 0.299 + channelColor.g * 0.587 + channelColor.b * 0.514 > 0.6?
                                   "#000000": "#FFFFFF"
                    }
                }
                Item {
                    width: root.width - mixerChannelIndex.width
                    height: 25
                    Text {
//                        anchors.left: parent.left
//                        anchors.verticalCenter: parent.verticalCenter
                        anchors.centerIn: parent
                        text: channelName
                        font.family: Constants.font
                        color: textInfoIndex.color
                    }
                }
            }
        }
    }
}

