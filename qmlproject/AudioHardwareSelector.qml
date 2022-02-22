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
            text: qsTr("硬件驱动")
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
            text: qsTr("驱动: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxDriver
            width: 100
            font.family: Constants.font
            textRole: "name"
            valueRole: "clsid"
            onCurrentValueChanged: {
                EventBridge.driverASIOSelectionChanged(currentValue);
            }
        }
        Text {
            width: 100
            text: qsTr("采样率: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        Row {
            spacing: 5
            MCtrl.ComboBox {
                id: comboBoxSampleRate
                width: 100
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
            text: qsTr("打开驱动设置")
            width: 100
            height: 20
            onClicked: {
                EventBridge.openASIODriverControlPanel();
            }
        }

        Text {
            width: 100
            height: 20
            text: qsTr("硬件输入/输出")
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
            text: qsTr("输入设备: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxInputDevice
            model: ["麦克风", "立体声混音"]
            font.family: Constants.font
            width: 100
        }
        Text {
            width: 100
            text: qsTr("输出设备: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxOutputDevice
            model: ["扬声器", "耳机"]
            font.family: Constants.font
            width: 100
        }
        Text {
            width: 100
            height: 20
            text: qsTr("状态")
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
            text: qsTr("缓冲区大小: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: bufferSize + qsTr(" 采样")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: qsTr("输入延迟: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: inputLatencyInSamples + qsTr(" 采样 (") + (inputLatencyInSamples * 1000 / sampleRate).toPrecision(3) + qsTr(" ms)")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: qsTr("输出延迟: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: outputLatencyInSamples + qsTr(" 采样 (") + (outputLatencyInSamples * 1000 / sampleRate).toPrecision(3) + qsTr(" ms)")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: qsTr("总延迟: ")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: 100
            height: 20
            text: (inputLatencyInSamples + outputLatencyInSamples)  + qsTr(" 采样 (") + ((inputLatencyInSamples + outputLatencyInSamples) * 1000 / sampleRate).toPrecision(3) + qsTr(" ms)")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
    }
}
