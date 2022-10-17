import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    id: root
    flags: Qt.Tool
    title: qsTr("Preferences")
    width: 600
    height: 600
    color: Constants.backgroundColor
    modality: Qt.NonModal
    visible: true
    signal scanPluginComplete()
    onScanPluginComplete: {
        pluginSettings.scanning = false;
    }
    property alias languageList: generalSettings.languageList
    property alias currentLanguage: generalSettings.currentLanguage
    property alias systemRender: generalSettings.systemRender

    property alias driverList: audioHardwareSelector.driverListModel
    property alias pluginDirectoryList: pluginSettings.pluginPathListModel
    property alias currentDriver: audioHardwareSelector.currentDriver
    property alias bufferSize: audioHardwareSelector.bufferSize
    property alias inputLatencyInSamples: audioHardwareSelector.inputLatencyInSamples
    property alias outputLatencyInSamples: audioHardwareSelector.outputLatencyInSamples
    property alias sampleRate: audioHardwareSelector.sampleRate
    property alias outputChannelList: audioHardwareSelector.outputChannelListModel
    property alias leftOutputChannel: audioHardwareSelector.leftOutputChannel
    property alias rightOutputChannel: audioHardwareSelector.rightOutputChannel

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
            Keys.onEscapePressed: {
                root.close();
            }
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
                    model: [qsTr("General"), qsTr("Audio Hardware"), qsTr("Plugins"), qsTr("Arrangement")]
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
                        Text {
                            text: modelData
                            font.family: Constants.font
                            anchors.left: parent.left
                            anchors.leftMargin: parent.height / 2
                            anchors.right: parent.right
                            anchors.rightMargin: anchors.leftMargin
                            anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideRight
                            color: categoryList.currentIndex == index? Constants.listHighlightContentColor: Constants.contentColor1
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
                        id: generalSettings
                        onLanguageSelectionChanged: {
                            EventBridge.languageSelectionChanged(currentLanguage);
                        }
                        onSystemRenderChanged: {
                            EventBridge.systemTextRenderingChanged(systemRender);
                        }
                    }
                    AudioHardwareSelector {
                        id: audioHardwareSelector
                        onDriverASIOSelectionChanged: {
                            EventBridge.driverASIOSelectionChanged(currentSelectionValue);
                        }
                        onSampleRateChanged: {
                            EventBridge.sampleRateChanged(sampleRate);
                        }
                        onOpenASIODriverControlPanel: {
                            EventBridge.openASIODriverControlPanel();
                        }
                    }
                    PluginSettings {
                        id: pluginSettings
                    }
                    ArrangementSettings {
                        id: arrangementSettings
                    }
                }
            }
        }
    }
}
