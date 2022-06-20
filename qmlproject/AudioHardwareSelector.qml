import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Rectangle {
    id: root
    width: 500
    height: 500
    clip: true
    color: Constants.backgroundColor
    property alias driverListModel: comboBoxDriver.model
    property alias currentDriver: comboBoxDriver.currentIndex
    property int bufferSize: 512
    property int inputLatencyInSamples: 512
    property int outputLatencyInSamples: 512
    property int sampleRate
    onSampleRateChanged: {
        for(var i = 0; i < comboBoxSampleRate.count; ++i) {
            if(sampleRate == parseFloat(comboBoxSampleRate.model[i])) {
                comboBoxSampleRate.currentIndex = i;
                return;
            }
        }
        var comboBoxModel = comboBoxSampleRate.model;
        comboBoxModel.push(sampleRate.toString());
        comboBoxSampleRate.model = comboBoxModel;
        comboBoxSampleRate.currentIndex = comboBoxModel.length - 1;
    }

    Grid {
        columns: 2
        columnSpacing: 10
        rowSpacing: 5
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignLeft
        Text {
            width: 100
            height: 20
            text: qsTr("Driver")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: 320
            height: 20
        }
        Text {
            width: 100
            text: qsTr("Driver: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxDriver
            width: openDriverSettingsButton.width
            font.family: Constants.font
            textRole: "name"
            valueRole: "clsid"
            displayText: count == 0? qsTr("No ASIO driver found"):
                         currentIndex == -1? qsTr("No ASIO driver loaded"):
                         currentText
            onCurrentValueChanged: {
                EventBridge.driverASIOSelectionChanged(currentValue);
            }
        }
        Text {
            width: 100
            text: qsTr("Sample Rate: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        Row {
            spacing: 5
            MCtrl.ComboBox {
                id: comboBoxSampleRate
                width: openDriverSettingsButton.width
                model: ["44100", "48000", "88200", "96000", "176400", "192000"]
                currentIndex: 0
                font.family: Constants.font
                onCurrentTextChanged: {
                    root.sampleRate = parseInt(currentText);
                    EventBridge.sampleRateChanged(root.sampleRate);
                }
            }
            Text {
                text: qsTr("Hz")
                font.family: Constants.font
                color: Constants.contentColor1
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
        Item {
            width: 100
            height: 20
        }
        MCtrl.Button {
            id: openDriverSettingsButton
            text: qsTr("Open Driver Settings")
            height: 20
            enabled: comboBoxDriver.count != 0 && comboBoxDriver.currentIndex != -1
            onClicked: {
                EventBridge.openASIODriverControlPanel();
            }
        }

        Text {
            width: 100
            height: 20
            text: qsTr("Hardware I/O")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: 320
            height: 20
        }
        Text {
            width: 100
            text: qsTr("Input Devie: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxInputDevice
            width: openDriverSettingsButton.width
            model: ["Microphone", "Stereo Mix"]
            font.family: Constants.font
        }
        Text {
            width: 100
            text: qsTr("Output Device: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxOutputDevice
            width: openDriverSettingsButton.width
            model: ["Speaker", "Headphone"]
            font.family: Constants.font
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Status")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: 320
            height: 20
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Buffer size: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: bufferSize + qsTr(" sample(s)")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Input latency: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: inputLatencyInSamples + qsTr(" samples") + " (" + (inputLatencyInSamples * 1000 / sampleRate).toPrecision(3) + qsTr(" ms)")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Output latency: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: outputLatencyInSamples + qsTr(" samples") + " (" + (outputLatencyInSamples * 1000 / sampleRate).toPrecision(3) + qsTr(" ms)")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Sum latency: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: (inputLatencyInSamples + outputLatencyInSamples) + qsTr(" samples") + " (" + ((inputLatencyInSamples + outputLatencyInSamples) * 1000 / sampleRate).toPrecision(3) + qsTr(" ms)")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
    }
}
