import QtQuick 2.15

QtObject {
    enum TrackType {
        AudioTrack,
        MIDITrack,
        InstrumentTrack
    }
    id: root

    property string trackName: "Track 1"
    property color trackColor: "#FF0000"
    property int trackType: 0
    property int height: 60
    property double gain: 1.0
    property double panning: 0.0
    property bool trackMute: false
    property bool trackSolo: false
    property bool trackInvertPhase: false
    property bool trackArmRecording: false
    property bool trackMonoDownMix: false
    property alias trackname: root.trackName
    property alias type: root.trackType
    property alias color: root.trackColor
    property alias trackcolor: root.trackColor
    property alias trackheight: root.height
    property alias mute: root.trackMute
    property alias solo: root.trackSolo
    property alias invertPhase: root.trackInvertPhase
    property alias armRecording: root.trackArmRecording
    property alias monoDownMix: root.trackMonoDownMix
    property Plugin instrument: null
    property ListModel plugin_list: null
    property alias channel_gain: root.gain
    property alias channel_panning: root.panning
}
