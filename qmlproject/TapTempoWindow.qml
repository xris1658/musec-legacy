import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    property double bpm: 127.5
    flags: Qt.Dialog
    id: root
    title: qsTr("Tap Tempo")
    width: 200
    height: 260
    color: Constants.backgroundColor
    modality: Qt.WindowModal
    visible: true
    Component.onCompleted: {
        tapButton.forceActiveFocus();
    }

    function applyTempo(tempo: double) {
        //
    }

    QtObject {
        id: variables
        property double start
        property double now
        property int clickedInterval: 0
    }
    Grid {
        id: grid
        anchors.fill: parent
        anchors.margins: 10
        columns: 1
        columnSpacing: 10
        MCtrl.Button {
            id: tapButton
            text: "TAP"
            width: root.width - 10 * 2
            height: width
            onClicked: {
                var now_ = Date.now();
                if(variables.clickedInterval) {
                    variables.now = now_;
                    root.bpm = 60000.0 * variables.clickedInterval / (variables.now - variables.start);
                }
                else {
                    variables.start = now_;
                }
                ++variables.clickedInterval;
            }
        }
        Item {
            width: root.width - 10 * 2
            height: bpmText.contentHeight
            Text {
                id: bpmText
                anchors.centerIn: parent
                text: bpm.toFixed(3);
                font.family: "Noto Sans Mono"
                font.styleName: "Condensed"
                font.bold: true
                font.pointSize: Constants.font.pointSize * 2
                color: Constants.contentColor1
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Item {
            width: root.width - 10 * 2
            height: buttonRound.height
            Grid {
                rows: 1
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                verticalItemAlignment: Grid.AlignVCenter
                horizontalItemAlignment: Grid.AlignHCenter
                MCtrl.Button {
                    id: buttonRound
                    text: qsTr("&Round")
                    onClicked: {
                        bpm = Math.round(bpm);
                        variables.clickedInterval = 0;
                    }
                }
                MCtrl.Button {
                    text: qsTr("&Apply and Close")
                    onClicked: {
                        Objects.mainWindow.bpm = root.bpm;
                        root.close();
                    }
                }
            }
        }
    }
}
