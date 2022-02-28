import QtQuick 2.15
import QtQuick.Controls 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    width: 638
    height: 54
    property int numerator: timeSignatureIndicatorNumeratorComboBox.currentText
    property int demoninator: timeSignatureIndicatorDenominatorComboBox.currentText
    property int cpu: 20
    property int currentKey: 10
    property int currentNoteName: Constants.NoteName.Sharp
    property double bpm: 128

    function getFloorWithWidth(bpm_: double) {
        var ret = Math.floor(bpm).toString();
        var length = ret.length;
        for(var i = length; i < 3; ++i) {
            ret = "0" + ret;
        }
        return ret;
    }

    onBpmChanged: {
        tempoIndicatorBeforePoint.text = getFloorWithWidth(bpm);
        tempoIndicatorAfterPoint.text = bpm.toFixed(3).toString().slice(-3);
    }

    Rectangle {
        id: bigClock
        anchors.fill: parent
        color: Constants.bigClockColor
        radius: 5
        Row {
            id: bigClockItems
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            spacing: 9
            Gradient {
                id: separatorGradient
                GradientStop {
                    position: 0
                    color: Constants.bigClockColor
                }
                GradientStop {
                    position: 0.5
                    color: Constants.bigClockTextColor
                }
                GradientStop {
                    position: 1
                    color: Constants.bigClockColor
                }
            }
            property int itemSpacing: 3
            Column {
                id: cpuIndicator
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                spacing: 2
                Item {
                    id: cpuIndicatorEntity
                    width: 50
                    height: 18
                    Rectangle {
                        id: cpuIndicatorBackground
                        width: 50
                        height: 18
                        color: Constants.backgroundColor
                        z: 0
                    }
                    Rectangle {
                        id: cpuIndicatorForeground
                        anchors.left: cpuIndicatorBackground.left
                        width: cpu / 100 * cpuIndicatorBackground.width
                        height: 18
                        color: Constants.bigClockCpuBackColor
                        z: 1
                    }
                    Text {
                        id: cpuIndicatorText
                        text: cpu + qsTr("%")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        font.family: "Noto Sans Mono Condensed"
                        font.bold: true
                        font.pointSize: 12
                        color: Constants.contentColor1
                        z: 2
                    }
                }
                Text {
                    text: qsTr("CPU")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono Condensed"
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 18
                gradient: separatorGradient
            }
            Column {
                id: keyIndicator
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                spacing: 2
                Row {
                    spacing: 2
                    Text {
                        id: keyIndicatorMain
                        text: keyComboBox.currentText[0]
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        font.pointSize: 17
                        height: contentHeight * 0.8
                        color: Constants.bigClockTextColor
                        MCtrl.ComboBox {
                            id: keyComboBox
                            showToolTip: MCtrl.ComboBox.ShowToolTip.Never
                            width: parent.width
                            height: parent.height
                            opacity: 0
                            font.family: Constants.font
                            model: Constants.dynamicKeyName
                            textRole: keyComboBox2.currentIndex == 0? "sharp": "flat"
                            currentIndex: currentKey
                        }
                    }
                    Text {
                        id: keyIndicatorSharp
                        y: keyIndicatorMain.y + 3
                        text: keyComboBox2.displayText
                        opacity: Constants.blackKey(keyComboBox.currentIndex)? 1: 0.5
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        font.pointSize: 10
                        height: contentHeight * 0.7
                        color: Constants.bigClockTextColor
                        MCtrl.ComboBox {
                            id: keyComboBox2
                            showToolTip: MCtrl.ComboBox.ShowToolTip.Never
                            width: parent.width * 5
                            height: parent.height
                            opacity: 0
                            font.family: Constants.font
                            model: ["#", "b"]
                            currentIndex: currentNoteName
                            displayText: Constants.blackKey(keyComboBox.currentIndex)? currentText: "\u25BC"
                        }
                    }
                    Column {
                        anchors.verticalCenter: keyIndicatorMain.verticalCenter
                        Text {
                            text: qsTr("MAJOR")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pointSize: 8
                            height: contentHeight * 0.7
                            font.capitalization: Font.AllUppercase
                        }
                        Text {
                            text: qsTr("minor")
                            color: Constants.bigClockAltTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pointSize: 8
                            height: contentHeight * 0.7
                            font.capitalization: Font.AllLowercase
                        }
                    }
                }
                Text {
                    text: qsTr("KEY SIG.")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono Condensed"
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 18
                gradient: separatorGradient
            }
            Column {
                id: loopIndicator
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 1
                spacing: 2
                Column {
                    id: loopIndicatorEntity
                    Text {
                        text: qsTr("0001:01.00")
                        color: Constants.bigClockTextColor
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        height: contentHeight * 0.7
                        font.pointSize: 8
                        font.capitalization: Font.AllUppercase
                    }
                    Text {
                        text: qsTr("0005:01.00")
                        color: Constants.bigClockTextColor
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        height: contentHeight * 0.7
                        font.pointSize: 8
                        font.capitalization: Font.AllUppercase
                    }
                }
                Text {
                    text: qsTr("LOOP")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono Condensed"
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 18
                gradient: separatorGradient
            }
            Column {
                id: timeIndicator
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                spacing: 2
                Text {
                    id: textTime
                    text: qsTr("0001:01.00")
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Noto Sans Mono"
                    font.styleName: "Condensed SemiBold"
                    font.pointSize: 21
                    height: contentHeight * 0.8
                    font.capitalization: Font.AllUppercase
                    color: Constants.bigClockTextColor
//                    verticalAlignment: Text.AlignBottom
                }
                Text {
                    width: textTime.width
                    horizontalAlignment: Text.AlignHCenter
                    ListModel {
                        id: textTimeFormat
                        ListElement {
                            name: qsTr("BAR:BEAT.STEP \u25BC")
                        }
                        ListElement {
                            name: qsTr("HH:MM:SS.SS \u25BC")
                        }
                        ListElement {
                            name: qsTr("SAMPLE \u25BC")
                        }
                    }
                    text: textTimeFormat.get(comboTimeFormat.currentIndex).name
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono Condensed"
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase

                    MCtrl.ComboBox {
                        id: comboTimeFormat
                        anchors.fill: parent
                        opacity: 0
                        font.family: Constants.font
                        model: ["节拍", "十进制", "采样"]
                        currentIndex: 0
                    }
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 18
                gradient: separatorGradient
            }
            Column {
                id: tempoIndicator
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                spacing: 2
                Row {
                    id: tempoIndicatorEntity
                    spacing: -2
                    Text {
                        id: tempoIndicatorBeforePoint
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 3
                        text: Math.floor(bpm)
                        color: Constants.bigClockTextColor
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        font.pointSize: 17
                        height: contentHeight * 0.7
                    }
                    Text {
                        id: tempoIndicatorPoint
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 2
                        text: qsTr(".")
                        color: Constants.bigClockTextColor
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        font.pointSize: 12
                        height: contentHeight * 0.7
                    }

                    Text {
                        id: tempoIndicatorAfterPoint
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 2
                        text: bpm.toFixed(3).toString().slice(-3)
                        color: Constants.bigClockTextColor
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        font.pointSize: 8
                        height: contentHeight * 0.7
                    }
                }

                Text {
                    text: qsTr("TEMPO")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono Condensed"
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 18
                gradient: separatorGradient
            }
            Column {
                id: timeSignatureIndicator
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                spacing: parent.itemSpacing
                Item {
                    id: timeSignatureIndicatorEntity
                    width: 45
                    height: 17
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        id: timeSignatureIndicatorNumerator
                        anchors.top: parent.top
                        anchors.topMargin: -4
                        anchors.right: timeSignatureIndicatorSlash.left
                        anchors.rightMargin: 2
                        text: timeSignatureIndicatorNumeratorComboBox.currentText
                        color: Constants.bigClockTextColor
                        font.family: "Noto Sans Mono Condensed"
                        font.bold: true
                        font.pointSize: 12
                        MCtrl.ComboBox {
                            id: timeSignatureIndicatorNumeratorComboBox
                            showToolTip: MCtrl.ComboBox.ShowToolTip.Never
                            opacity: 0
                            anchors.fill: parent
                            model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"]
                            currentIndex: 3
                        }
                    }
                    Text {
                        id: timeSignatureIndicatorSlash
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("/")
                        color: Constants.bigClockTextColor
                        font.family: "Noto Sans Mono Condensed"
                        font.pointSize: 17
                    }
                    Text {
                        id: timeSignatureIndicatorDenominator
                        anchors.left: timeSignatureIndicatorSlash.right
                        anchors.rightMargin: 0
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: -6
                        text: timeSignatureIndicatorDenominatorComboBox.currentText
                        color: Constants.bigClockTextColor
                        font.family: "Noto Sans Mono Condensed"
                        font.bold: true
                        font.pointSize: 12
                        MCtrl.ComboBox {
                            id: timeSignatureIndicatorDenominatorComboBox
                            showToolTip: MCtrl.ComboBox.ShowToolTip.Never
                            opacity: 0
                            anchors.fill: parent
                            model: ["2", "4", "8", "16", "32"]
                            currentIndex: 1
                        }
                    }
                }
                Text {
                    text: qsTr("TIME SIG.")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono Condensed"
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 18
                gradient: separatorGradient
            }
            Column {
                id: levelIndicator
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
                spacing: 5
                Column {
                    id: dBMeter
                    height: 18
                    spacing: 2
                    Row {
                        id: dBMeterLeft
                        height: 8
                        spacing: 0
                        Rectangle {
                            id: dBMeterLeftGreen
                            width: 42
                            height: parent.height
                            color: Constants.belowNeg6dBColor
                        }
                        Rectangle {
                            id: dBMeterLeftYellow
                            width: 15
                            height: parent.height
                            color: Constants.below0dBColor
                        }
                        Rectangle {
                            id: dBMeterLeftRed
                            width: 10
                            height: parent.height
                            color: Constants.above0dBColor
                        }
                    }
                    Row {
                        id: dBMeterRight
                        height: 8
                        spacing: 0
                        Rectangle {
                            id: dBMeterRightGreen
                            width: 42
                            height: parent.height
                            color: Constants.belowNeg6dBColor
                        }
                        Rectangle {
                            id: dBMeterRightYellow
                            width: 15
                            height: parent.height
                            color: Constants.below0dBColor
                        }
                        Rectangle {
                            id: dBMeterRightRed
                            width: 10
                            height: parent.height
                            color: Constants.above0dBColor
                        }
                    }
                }
                Item {
                    id: masterVolumeFader
                    width: 67
                    height: 10
                    MCtrl.Slider {
                        orientation: Qt.Horizontal
                        width: parent.width
                        value: 0.8
                        anchors.centerIn: parent
                    }
                }
            }
        }
    }
}
