import QtQml.Models 2.15

ListModel {
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
        plugin_list: PluginSequenceModel {}
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
        plugin_list: PluginSequenceModel {}
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
        plugin_list: PluginSequenceModel {}
        channel_gain: 1.0
        channel_panning: 0.0
    }
}