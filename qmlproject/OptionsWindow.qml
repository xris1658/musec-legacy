import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    flags: Qt.Tool
    id: root
    title: qsTr("选项")
    width: 600
    height: 600
    color: Constants.backgroundColor
    modality: Qt.NonModal
    visible: true
    signal scanPluginComplete()
    onScanPluginComplete: {
        pluginSettings.scanning = false;
    }

    property alias driverList: audioHardwareSelector.driverListModel
    property alias pluginDirectoryList: pluginSettings.pluginPathListModel
    property alias currentDriver: audioHardwareSelector.currentDriver
    property alias bufferSize: audioHardwareSelector.bufferSize
    property alias inputLatencyInSamples: audioHardwareSelector.inputLatencyInSamples
    property alias outputLatencyInSamples: audioHardwareSelector.outputLatencyInSamples
    property alias sampleRate: audioHardwareSelector.sampleRate
    MCtrl.SplitView {
        orientation: Qt.Horizontal
        anchors.fill: parent
        handleOpacity: 0.0
        Item {
            id: category
            SplitView.preferredWidth: 150
            SplitView.minimumWidth: 100
            SplitView.maximumWidth: 250
            height: parent.height
            Rectangle {
                anchors.fill: parent
                anchors.margins: 5
                anchors.rightMargin: 0
                color: Constants.backgroundColor
                border.color: Constants.borderColor
                ListView {
                    id: categoryList
                    anchors.fill: parent
                    interactive: false
                    highlightMoveDuration: 0
                    highlightResizeDuration: 0
                    model: ListModel {
                        id: assetsList
                        ListElement {
                            name: qsTr("常规")
                        }
                        ListElement {
                            name: qsTr("音频硬件")
                        }
                        ListElement {
                            name: qsTr("插件")
                        }
                    }
                    delegate: Item {
                        id: itemDelegate
                        width: parent.width
                        height: 20
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                categoryList.currentIndex = index;
                            }
                        }
                        Row {
                            anchors.fill: parent
                            Item {
                                width: height
                                height: itemDelegate.height
                            }
                            Text {
                                text: name
                                font.family: Constants.font
                                color: categoryList.currentIndex == index? Constants.listHighlightContentColor : Constants.contentColor1
                            }
                        }
                    }
                    highlight: Rectangle {
                        color: Constants.listHighlightBackColor
                    }
                    focus: true
                }
            }
        }
        Item {
            id: setting
            height: parent.height
            Item {
                id: settingContent
                anchors.fill: parent
                anchors.margins: 5
                StackLayout {
                    anchors.fill: parent
                    currentIndex: categoryList.currentIndex
                    GeneralSettings {
                    }
                    AudioHardwareSelector {
                        id: audioHardwareSelector
                    }
                    PluginSettings {
                        id: pluginSettings
                    }
                }
            }
        }
    }
}
