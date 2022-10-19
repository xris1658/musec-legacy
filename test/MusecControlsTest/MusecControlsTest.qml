import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    visible: true
    width: 1000
    height: 800
    color: "#202020"
    Grid {
        rows: 5
        columns: 5
        Rectangle {
            width: 200
            height: 160
            color: "transparent"
            border.color: "#CCCCCC"
            Column {
                anchors.centerIn: parent
                spacing: 5
                MCtrl.Button {
                    text: "default"
                }
                MCtrl.Button {
                    enabled: false
                    text: "disabled"
                }
                MCtrl.Button {
                }
                MCtrl.Button {
                    height: 20
                    text: "Explicit height"
                }
                MCtrl.Button {
                    width: 120
                    text: "Explicit width"
                }
            }
        }
        Rectangle {
            width: 200
            height: 160
            color: "transparent"
            border.color: "#CCCCCC"
            MCtrl.Slider {
                orientation: Qt.Horizontal
                width: 160
                height: 40
                lengthPadding: 5
                thicknessPadding: 5
            }
            MCtrl.Slider {
                anchors.right: parent.right
                orientation: Qt.Vertical
                width: 40
                height: 160
                lengthPadding: 5
                thicknessPadding: 5
            }
        }
        Rectangle {
            width: 200
            height: 160
            color: "transparent"
            border.color: "#CCCCCC"
            Column {
                anchors.centerIn: parent
                spacing: 5
                MCtrl.CheckBox {
                    text: "Default"
                }
                MCtrl.CheckBox {
                    enabled: false
                    checkState: Qt.Checked
                    text: "Checked"
                }
                MCtrl.CheckBox {
                    checkState: Qt.PartiallyChecked
                    text: "PartiallyChecked"
                }
            }
        }
        Rectangle {
            width: 200
            height: 160
            color: "transparent"
            border.color: "#CCCCCC"
            MCtrl.Dial {
                anchors.centerIn: parent
                width: 40
                height: 40
            }
        }
    }
}
