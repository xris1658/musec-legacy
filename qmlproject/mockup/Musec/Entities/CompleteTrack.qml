import QtQuick 2.15

QtObject {
    property string trackName: "Track 1"
    property color trackColor: "#FF0000"
    property int trackType: 0
    property int height: 40
    property double gain: 1.0
    property double panning: 0.0
    property bool trackMute: false
    property bool trackSolo: false
    property bool trackInvertPhase: false
    property bool trackArmRecording: false
    property bool trackMonoDownMix: false
}
