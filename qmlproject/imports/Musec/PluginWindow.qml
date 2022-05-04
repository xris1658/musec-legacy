import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: root
    flags: Qt.Dialog
    modality: Qt.NonModal
    property bool destroyingPluigin: false
    onClosing: {
        if(!root.destroyingPlugin) {
            close.accepted = false;
            root.hide();
        }
    }
}
