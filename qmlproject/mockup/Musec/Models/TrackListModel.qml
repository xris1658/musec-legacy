import QtQml 2.15
import QtQml.Models 2.15

ListModel {
    property double masterTrackGain: 1.0
    property double masterTrackPanning: 0.0
    property bool masterTrackMute: false
    property bool masterTrackSolo: false
    property bool masterTrackInvertPhase: false
    property bool masterTrackArmRecording: false
    property bool masterTrackMonoDownMix: false

    dynamicRoles: true

    function insertTrack(index: int, track: jsobject) {
        insert(index, track);
    }
    function appendTrack(track: jsobject) {
        append(track);
    }
    function removeTrack(index: int) {
        remove(index, 1);
    }
    function loadInstrument(trackIndex: int, pluginId: int) {
        //
    }
    function insertEffect(trackIndex: int, pluginIndex: int, pluginId: int) {
        //
    }
    function replaceEffect(trackIndex: int, pluginIndex: int, pluginId: int) {
        //
    }
    function insertEffectMasterTrack(pluginIndex: int, pluginId: int) {
        //
    }
    function replaceEffectMasterTrack(pluginIndex: int, pluginId: int) {
        //
    }
    function removeInstrument(trackIndex: int) {
        //
    }
    function removeEffect(trackIndex: int, pluginIndex: int) {
        //
    }
    function removeEffectMasterTrack(pluginIndex: int) {
        //
    }
    function copyInstrument(fromTrackIndex: int, toTrackIndex: int) {
        //
    }
    function copyEffect(fromTrackIndex: int, fromPluginIndex: int, toTrackIndex: int, toPluginIndex: int) {
        //
    }
    function moveInstrument(fromTrackIndex: int, toTrackIndex: int) {
        //
    }
    function moveEffect(fromTrackIndex: int, fromPluginIndex: int, toTrackIndex: int, toPluginIndex: int) {
        //
    }
}
