import QtQml.Models 2.15

ListModel {
    property double masterTrackGain: 1.0
    property double masterTrackPanning: 0.0
    property bool masterTrackMute: false
    property bool masterTrackSolo: false
    property bool masterTrackInvertPhase: false
    property bool masterTrackArmRecording: false
    property bool masterTrackMonoDownMix: false
    ListElement {
        trackname: "Track 1"
        type: 0
        trackcolor: "#FF0000"
        trackheight: 40
        mute: false
        solo: false
        invertPhase: false
        armRecording: false
        monoDownMix: false
        instrument: null
        plugin_list: null
        channel_gain: 1.0
        channel_panning: 0.0
    }
    ListElement {
        trackname: "Track 2"
        type: 1
        trackcolor: "#00FF00"
        trackheight: 40
        mute: false
        solo: false
        invertPhase: false
        armRecording: false
        monoDownMix: false
        instrument: null
        plugin_list: null
        channel_gain: 1.0
        channel_panning: 0.0
    }
    ListElement {
        trackname: "Track 3"
        type: 2
        trackcolor: "#0000FF"
        trackheight: 40
        mute: false
        solo: false
        invertPhase: false
        armRecording: false
        monoDownMix: false
        instrument: null
        plugin_list: null
        channel_gain: 1.0
        channel_panning: 0.0
    }
}
