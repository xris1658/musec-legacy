import QtQuick 2.15
import QtQuick.Controls 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    property int keyHeight: 18
    width: 100
    height: keyHeight * Constants.midiValueRange
    Column {
        anchors.fill: parent
        Repeater {
            model: Constants.midiValueRange
            MCtrl.Button {
                property int num: Constants.midiValueRange - 1 - index
                width: parent.width
                height: keyHeight
                border.color: "#000000"
                color: Constants.blackKey(num)? "#000000" : "#ffffff"
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    text: (num % 12 == 0) ? "C" + (num - (num % 12)) / 12 : ""
                    font.family: Constants.myMonoFont.name
                    font.bold: true
                    font.pixelSize: keyHeight - 2
                    color: "#808080"
                }
            }
        }
    }
}
