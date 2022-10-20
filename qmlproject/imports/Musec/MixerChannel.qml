import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQml.Models 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    id: root
    width: 80
    height: 320
    clip: false
    Rectangle {
        anchors.fill: parent
        color: Constants.backgroundColor
    }
    enum ChannelType {
        AudioTrack,
        InstrumentTrack
    }
    property bool instrumentEnabled
    property string instrumentName
    property bool instrumentSidechainExist: false
    property bool instrumentSidechainEnabled: false
    property bool instrumentEditorVisible: false
    property alias effectListModel: channelEffectList.model
    property int channelType
    property bool channelMuted: false
    property bool channelSolo: false
    property bool channelInverted: false
    property bool channelArmRecording: false
    property bool channelMonoDownMix: false
    property real panning: 0.0
    property real stereo: 1.0
    property real gain: 1.00
    property real peak: 0.00
    property string channelName
    property color channelColor
    property int channelNumber: 0
    property alias effectVisible: channelEffects.visible
    property alias gainAndMeterVisible: channelGainAndPeak.visible
    readonly property int channelInfoHeight: 20
    readonly property int channelEffectListFooterMinimumHeight: 20
    property Item instrumentSlot
    signal instrumentSlotRightClicked(x: int, y: int)
    function getAudioEffectSlot(index: int) {
        return channelEffectList.itemAtIndex(index);
    }
    signal audioSlotRightClicked(index: int, x: int, y: int)
    // Operations of instrument
    signal instrumentSlotDragEventEntered(drag: var)
    onInstrumentSlotDragEventEntered: (drag) => {
        if(drag.getDataAsString("type") != 2) {
            drag.accepted = false;
        }
    }
    signal instrumentSlotDragEventDropped(drop: var)
    onInstrumentSlotDragEventDropped: (drop) => {
        var format = parseInt(drop.getDataAsString("format"));
        var path = drop.getDataAsString("path");
        var pluginSubId = parseInt(drop.getDataAsString("uid"));
        loadInstrument(path, pluginSubId, format);
    }
    signal instrumentSlotVisibleToggled(instrumentWindowVisible: bool)
    signal instrumentSlotEnabledToggled(instrumentSlotEnabled: bool)
    signal audioEffectSlotVisibleToggled(audioEffectWindowVisible: bool, effectIndex: int)
    signal audioEffectSlotEnabledToggled(audioEffectEnabled: bool, effectIndex: int)
    // Operations of audio effect
    signal audioEffectSlotDragEventEntered(drag: var, audioEffectIndex: int)
    onAudioEffectSlotDragEventEntered: (drag, audioEffectIndex) => {
        if(drag.getDataAsString("type") != 3) {
            drag.accepted = false;
        }
    }
    signal audioEffectSlotDragEventDropped(drop: var, audioEffectIndex: int)
    onAudioEffectSlotDragEventDropped: (drop, audioEffectIndex) => {
        var format = parseInt(drop.getDataAsString("format"));
        var path = drop.getDataAsString("path");
        var pluginSubId = parseInt(drop.getDataAsString("uid"));
        replaceEffect(path, pluginSubId, format, audioEffectIndex);
    }
    signal betweenAudioEffectSlotDragEventEntered(drag: var, audioEffectIndex: int)
    onBetweenAudioEffectSlotDragEventEntered: (drag, audioEffectIndex) => {
        if(drag.getDataAsString("type") != 3) {
            drag.accepted = false;
        }
    }

    signal betweenAudioEffectSlotDragEventDropped(drop: var, audioEffectIndex: int)
    onBetweenAudioEffectSlotDragEventDropped: (drop, audioEffectIndex) => {
        var format = parseInt(drop.getDataAsString("format"));
        var path = drop.getDataAsString("path");
        var pluginSubId = parseInt(drop.getDataAsString("uid"));
        insertEffect(path, pluginSubId, format, audioEffectIndex);
    }
    signal blankAreaDragEventEntered(drag: var)
    signal blankAreaDragEventDropped(drop: var)
    onBlankAreaDragEventDropped: (drop) => {
        var format = parseInt(drop.getDataAsString("format"));
        var path = drop.getDataAsString("path");
        var pluginSubId = parseInt(drop.getDataAsString("uid"));
        if(drop.getDataAsString("type") == 2) {
            loadInstrument(path, pluginSubId, format);
        }
        else {
            insertEffect(path, pluginSubId, format, channelEffectList.count);
        }
    }
    signal loadInstrument(pluginPath: string, pluginSubId: int, pluginFormat: int)
    signal insertEffect(pluginPath: string, pluginSubId: int, pluginFormat: int, effectIndex: int)
    signal replaceEffect(pluginPath: string, pluginSubId: int, pluginFormat: int, effectIndex: int)
    signal setMute(newMute: bool)
    signal setSolo(newSolo: bool)
    signal setInvertPhase(newInvertPhase: bool)
    signal setArmRecording(newArmRecording: bool)
    signal setMonoDownMix(newMonoDownMix: bool)
    signal setGain(newGain: double)
    signal setPanning(newPanning: double)
    Column {
        width: parent.width
        height: parent.height
        Item {
            id: channelEffects
            width: parent.width
            height: (root.height - channelControl.height - channelStereo.height - channelInfo.height) / (gainAndMeterVisible? 2: 1)
            clip: false
            ListView {
                id: channelEffectList
                anchors.fill: parent
                anchors.margins: 2
                visible: parent.visible
                spacing: 2
                interactive: false
                clip: false
                property int headerHeight: 20
                header: Item {
                    id: effectListHeader
                    width: parent.width
                    height: channelEffectList.headerHeight + channelEffectList.spacing
                    MixerSlot {
                        id: instrumentButton
                        visible: root.channelType == MixerChannel.ChannelType.InstrumentTrack
                        anchors.left: parent.left
                        anchors.fill: parent
                        anchors.bottomMargin: channelEffectList.spacing
                        name: root.instrumentName
                        slotEnabled: root.instrumentEnabled
                        sidechainExist: root.instrumentSidechainExist
                        sidechainEnabled: root.instrumentSidechainEnabled
                        editorVisible: root.instrumentEditorVisible
                        onEntered: (drag) => {
                            root.instrumentSlotDragEventEntered(drag);
                        }
                        onDropped: (drop) => {
                            root.instrumentSlotDragEventDropped(drop);
                        }
                        onClicked: {
                            root.instrumentSlotVisibleToggled(!editorVisible);
                        }
                        onEnabledButtonClicked: {
                            console.log(!slotEnabled);
                            root.instrumentSlotEnabledToggled(!slotEnabled);
                        }
                        onRightClicked: (x, y) => {
                            root.instrumentSlot = instrumentButton;
                            root.instrumentSlotRightClicked(x, y);
                        }
                    }
                }
                delegate: Item {
                    width: parent.width
                    height: channelEffectList.headerHeight
                    MixerSlot {
                        anchors.fill: parent
                        name: plugin_name
                        slotEnabled: processing
                        sidechainExist: sidechain_exist
                        sidechainEnabled: sidechain_enabled
                        editorVisible: window_visible
                        onEntered: (drag) => {
                            root.audioEffectSlotDragEventEntered(drag, index);
                        }
                        onDropped: (drop) => {
                            root.audioEffectSlotDragEventDropped(drop, index);
                        }
                        onClicked: {
                            window_visible = !editorVisible;
                        }
                        onEnabledButtonClicked: {
                            processing = !slotEnabled;
                        }
                        onRightClicked: (x, y) => {
                            root.audioSlotRightClicked(index, x, y);
                        }
                    }
                    DropArea {
                        id: betweenMixerSlot
                        width: parent.width
                        height: 10
                        anchors.top: parent.top
                        anchors.topMargin: (-height) / 2
                        Rectangle {
                            anchors.fill: parent
                            color: betweenMixerSlot.containsDrag? Constants.mouseOverElementColor: "transparent"
                        }
                        onEntered: (drag) => {
                            root.betweenAudioEffectSlotDragEventEntered(drag, index);
                        }
                        onDropped: (drop) => {
                            root.betweenAudioEffectSlotDragEventDropped(drop, index);
                        }
                    }
                }
                footer: Item {
                    width: parent.width
                    height: channelEffects.height <= channelEffectList.headerHeight + 20 * channelEffectList.count + root.channelEffectListFooterMinimumHeight?
                        root.channelEffectListFooterMinimumHeight:
                        channelEffects.height - channelEffectList.headerHeight - 20 * channelEffectList.count
                    Rectangle {
                        anchors.fill: parent
                        anchors.topMargin: channelEffectList.spacing
                        color: channelEffectDropArea.containsDrag? Constants.mouseOverElementColor: "transparent"
                    }
                    DropArea {
                        id: channelEffectDropArea
                        anchors.fill: parent
                        onEntered: (drag) => {
                            root.blankAreaDragEventEntered(drag);
                        }
                        onDropped: (drop) => {
                            root.blankAreaDragEventDropped(drop);
                        }
                    }
                }
            }
        }
        Item {
            id: channelControl
            width: parent.width
            height: 20
            clip: true
            Row {
                Item {
                    width: root.width / 4
                    height: channelControl.height
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 2
                        border.width: 2
                        border.color: Constants.muteIndicatorColor
                        color: channelMuted? Qt.darker(border.color, 1.5) : Constants.backgroundColor
                        Text {
                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: -1.2
                            text: "M"
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pixelSize: 15
                            color: Constants.contentColor1
                        }
                        onClicked: {
                            setMute(!channelMuted);
                        }
                    }
                }
                Item {
                    width: root.width / 4
                    height: channelControl.height
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 2
                        border.width: 2
                        border.color: Constants.soloIndicatorColor
                        color:  channelSolo? Qt.darker(border.color, 1.5) : Constants.backgroundColor
                        Text {
                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: -1.2
                            text: "S"
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pixelSize: 15
                            color: Constants.contentColor1
                        }
                        onClicked: {
                            setSolo(!channelSolo);
                        }
                    }
                }
                Item {
                    width: root.width / 4
                    height: channelControl.height
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 2
                        border.width: 2
                        border.color: Constants.invertIndicatorColor
                        color:  channelInverted? border.color : Constants.backgroundColor
                        Text {
                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: -1.2
                            text: "\u2205"
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pixelSize: 15
                            color: Constants.contentColor1
                        }
                        onClicked: {
                            setInvertPhase(!channelInverted);
                        }
                    }
                }
                Item {
                    width: root.width / 4
                    height: channelControl.height
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 2
                        border.width: 2
                        border.color: Constants.recordIndicatorColor
                        color: channelArmRecording? border.color : Constants.backgroundColor
                        Text {
                            anchors.centerIn: parent
                            anchors.verticalCenterOffset: -1.2
                            text: "R"
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pixelSize: 15
                            color: Constants.contentColor1
                        }
                        onClicked: {
                           setArmRecording(!channelArmRecording);
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
                    MCtrl.Dial {
                        anchors.centerIn: parent
                        diameter: parent.height - 2
                        from: -1.0
                        to: 1.0
                        value: root.panning
                        inputMode: MCtrl.Dial.Vertical
                        live: true
                        onValueChanged: {
                            root.setPanning(value);
                        }
                    }
                }
                Item {
                    width: root.width / 2
                    height: 20
                    MCtrl.Button {
                        anchors.centerIn: parent
                        width: parent.width - 2
                        height: parent.height - 2
                        border.width: 1
                        border.color: Constants.borderColor
                        color: root.channelMonoDownMix? Constants.contentColor1: Constants.backgroundColor
                        contentItem: Text {
                            anchors.centerIn: parent
                            text: qsTr("Mono")
                            font.family: Constants.font
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: root.channelMonoDownMix? Constants.backgroundColor: Constants.contentColor1
                        }
                        z: 2
                        clip: true
                        onClicked: {
                            root.setMonoDownMix(!root.channelMonoDownMix);
                        }
                    }
                }
            }
        }
        Item {
            id: channelGainAndPeak
            width: parent.width
            height: root.height - (effectVisible? channelEffects.height: 0) - channelControl.height - channelStereo.height - channelInfo.height
            clip: true
            Row {
                visible: parent.visible
                Column {
                    Item {
                        width: root.width / 2
                        height: channelGainAndPeak.height - 35
                        MCtrl.Slider {
                            topPadding: 5
                            id: mixerChannelSlider
                            implicitThickness: 4
                            handleImplicitThickness: 8
                            handleImplicitLength: 12
                            anchors.horizontalCenter: parent.horizontalCenter
                            orientation: Qt.Vertical
                            width: 20
                            height: parent.height
                            from: -144
                            to: 6
                            live: true
                            value: 20 * Math.log10(root.gain)
                            snapMode: Slider.NoSnap
                            onValueChanged: {
                                var newGain = Math.pow(10.0, value * 0.05);
                                setGain(newGain);
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
                                    var newGain = text.length == 0? 1.0: Math.pow(10.0, text * 0.05);
                                    gainTextInputPopup.visible = false;
                                    setGain(newGain);
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
                            id: gainMouseArea
                            anchors.fill: parent
                            onClicked: {
                                gainTextInputPopup.visible = true;
                                gainTextInput.text = mixerChannelGainText.text;
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
            height: channelInfoHeight
            color: channelColor
            Row {
                Item {
                    id: mixerChannelIndex
                    width: channelNumber? 30: 0
                    height: channelInfo.height
                    clip: true
                    Text {
                        id: textInfoIndex
                        anchors.centerIn: parent
                        text: channelNumber
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        color: Constants.contentColorOnBackground(channelColor)
                    }
                }
                Item {
                    width: root.width - mixerChannelIndex.width
                    height: channelInfo.height
                    clip: true
                    Text {
                        width: parent.width
                        anchors.centerIn: parent
                        text: channelName
                        font.family: Constants.font
                        color: textInfoIndex.color
                        horizontalAlignment: Text.AlignHCenter
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}

