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
    property bool running: false
    property int bufferSize: 512
    property int inputLatencyInSamples: 512
    property int outputLatencyInSamples: 512
    property int sampleRate
    property alias inputList: inputListView.model
    property alias outputList: outputListView.model
    function driverLoadedAndWorking() {
        return comboBoxDriver.count != 0 && currentDriver != -1 && running;
    }

    signal driverASIOSelectionChanged(currentSelectionValue: string)
    signal openASIODriverControlPanel()
    signal reloadDriver()

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
        id: mainGrid
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
            id: secondColumn
            width: 320
            height: 20
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Driver") + ": "
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: secondColumn.width
            height: 20
            MCtrl.ComboBox {
                id: comboBoxDriver
                width: parent.width - reloadDriverButton.width - mainGrid.rowSpacing
                height: 20
                font.family: Constants.font
                textRole: "name"
                valueRole: "clsid"
                displayText: count == 0? qsTr("No ASIO driver found"):
                             currentIndex == -1? qsTr("No driver loaded"):
                             root.running? currentText:
                             currentText + " (" + qsTr("Not running") + ")"
                onCurrentValueChanged: {
                    driverASIOSelectionChanged(currentValue);
                }
            }
            MCtrl.Button {
                id: reloadDriverButton
                anchors.right: parent.right
                width: 20
                height: 20
                hoverEnabled: true
                Image {
                    anchors.centerIn: parent
                    width: 16
                    height: 16
                    source: "images/restart.svg"
                }
                MCtrl.ToolTip {
                    visible: parent.hovered
                    text: qsTr("Reset driver")
                }
                onClicked: {
                    reloadDriver();
                }
            }
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Sample Rate") + ": "
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Row {
            visible: root.driverLoadedAndWorking()
            spacing: 5
            MCtrl.ComboBox {
                id: comboBoxSampleRate
                width: openDriverSettingsButton.width
                model: ["44100", "48000", "88200", "96000", "176400", "192000"]
                currentIndex: 0
                font.family: Constants.font
                onCurrentTextChanged: {
                    root.sampleRate = parseInt(currentText);
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
            visible: root.driverLoadedAndWorking()
        }
        MCtrl.Button {
            id: openDriverSettingsButton
            width: comboBoxDriver.width
            visible: root.driverLoadedAndWorking()
            text: qsTr("Open Driver Settings")
            height: 20
            enabled: root.driverLoadedAndWorking();
            onClicked: {
                openASIODriverControlPanel();
            }
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
            visible: root.driverLoadedAndWorking()
        }
        Item {
            width: 320
            height: 20
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Buffer size") + ": "
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: bufferSize + " " + qsTr("sample(s)")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Input latency") + ": "
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: inputLatencyInSamples + " " + qsTr("sample(s)") + " (" + (inputLatencyInSamples * 1000 / sampleRate).toPrecision(3) + " " + qsTr("ms") + ")"
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Output latency") + ": "
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: outputLatencyInSamples + " " + qsTr("sample(s)") + " (" + (outputLatencyInSamples * 1000 / sampleRate).toPrecision(3) + " " + qsTr("ms") + ")"
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Sum latency") + ": "
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: (inputLatencyInSamples + outputLatencyInSamples) + " " + qsTr("sample(s)") + " (" + ((inputLatencyInSamples + outputLatencyInSamples) * 1000 / sampleRate).toPrecision(3) + " " + qsTr("ms") + ")"
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Channels")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
            visible: root.driverLoadedAndWorking()
        }
        Rectangle {
            id: header
            width: 320
            height: 20
            color: "transparent"
            border.color: Constants.borderColor
            readonly property alias indexWidth: indexHeader.width
            readonly property alias activeWidth: activeHeader.width
            readonly property alias nameWidth: nameHeader.width
            visible: root.driverLoadedAndWorking()
            Row {
                Item {
                    id: indexHeader
                    width: indexText.contentWidth + 10
                    height: 20
                    Rectangle {
                        width: 1
                        height: parent.height
                        anchors.right: parent.right
                        color: Constants.borderColor
                    }
                    Text {
                        id: indexText
                        text: "#"
                        anchors.centerIn: parent
                        font.family: Constants.font
                        color: Constants.contentColor2
                    }
                }
                Item {
                    id: activeHeader
                    width: activeText.contentWidth + 10
                    height: 20
                    Rectangle {
                        width: 1
                        height: parent.height
                        anchors.right: parent.right
                        color: Constants.borderColor
                    }
                    Text {
                        id: activeText
                        text: qsTr("Active")
                        anchors.centerIn: parent
                        font.family: Constants.font
                        color: Constants.contentColor2
                    }
                }
                Item {
                    id: nameHeader
                    width: header.width - indexHeader.width - activeHeader.width
                    height: 20
                    Rectangle {
                        width: 1
                        height: parent.height
                        anchors.right: parent.right
                        color: Constants.borderColor
                    }
                    Text {
                        id: nameText
                        text: qsTr("Name")
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        font.family: Constants.font
                        color: Constants.contentColor2
                    }
                }
            }
        }
        Item {
            width: 100
            height: inputListViewArea.height
            visible: root.driverLoadedAndWorking() && (inputListView.count != 0)
            Text {
                anchors.top: parent.top
                width: 100
                height: 20
                text: qsTr("Inputs") + ": "
                font.family: Constants.font
                color: Constants.contentColor1
                horizontalAlignment: Text.AlignRight
                leftPadding: 5
                verticalAlignment: Text.AlignVCenter
            }
        }
        Rectangle {
            id: inputListViewArea
            width: 320
            height: Math.max(inputListView.height, 20)
            color: "transparent"
            border.color: Constants.borderColor
            visible: root.driverLoadedAndWorking() && (inputListView.count != 0)
            ListView {
                id: inputListView
                width: 320
                height: contentHeight
                delegate: Item {
                    width: inputListView.width
                    height: 20
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: Constants.borderColor
                    }
                    Row {
                        Item {
                            width: header.indexWidth
                            height: 20
                            Text {
                                text: index + 1
                                anchors.centerIn: parent
                                font.family: Constants.font
                                color: Constants.contentColor1
                            }
                            Rectangle {
                                width: 1
                                height: parent.height
                                anchors.right: parent.right
                                color: Constants.borderColor
                            }
                        }
                        Item {
                            width: header.activeWidth
                            height: 20
                            MCtrl.CheckBox {
                                anchors.centerIn: parent
                                width: 16
                                height: width
                                checked: isActive
                            }
                            Rectangle {
                                width: 1
                                height: parent.height
                                anchors.right: parent.right
                                color: Constants.borderColor
                            }
                        }
                        Item {
                            width: header.nameWidth
                            height: 20
                            Text {
                                text: name
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                                font.family: Constants.font
                                color: Constants.contentColor1
                            }
                            Rectangle {
                                width: 1
                                height: parent.height
                                anchors.right: parent.right
                                color: Constants.borderColor
                            }
                        }
                    }
                }
            }
        }
        Item {
            width: 100
            height: outputListViewArea.height
            visible: root.driverLoadedAndWorking() && (outputListView.count != 0)
            Text {
                anchors.top: parent.top
                width: 100
                height: 20
                text: qsTr("Outputs") + ": "
                font.family: Constants.font
                color: Constants.contentColor1
                horizontalAlignment: Text.AlignRight
                leftPadding: 5
                verticalAlignment: Text.AlignVCenter
            }
        }
        Rectangle {
            id: outputListViewArea
            width: 320
            height: Math.max(outputListView.height, 20)
            color: "transparent"
            border.color: Constants.borderColor
            visible: root.driverLoadedAndWorking() && (outputListView.count != 0)
            ListView {
                id: outputListView
                width: 320
                height: contentHeight
                delegate: Item {
                    width: outputListView.width
                    height: 20
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: Constants.borderColor
                    }
                    Row {
                        Item {
                            width: header.indexWidth
                            height: 20
                            Text {
                                text: index + 1
                                anchors.centerIn: parent
                                font.family: Constants.font
                                color: Constants.contentColor1
                            }
                            Rectangle {
                                width: 1
                                height: parent.height
                                anchors.right: parent.right
                                color: Constants.borderColor
                            }
                        }
                        Item {
                            width: header.activeWidth
                            height: 20
                            MCtrl.CheckBox {
                                anchors.centerIn: parent
                                width: 16
                                height: width
                                checked: isActive
                            }
                            Rectangle {
                                width: 1
                                height: parent.height
                                anchors.right: parent.right
                                color: Constants.borderColor
                            }
                        }
                        Item {
                            width: header.nameWidth
                            height: 20
                            Text {
                                text: name
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                                font.family: Constants.font
                                color: Constants.contentColor1
                            }
                            Rectangle {
                                width: 1
                                height: parent.height
                                anchors.right: parent.right
                                color: Constants.borderColor
                            }
                        }
                    }
                }
            }
        }
    }
}
