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

    property alias driverList: asioDriverSettings.driverListModel
    property alias pluginDirectoryList: pluginSettings.pluginPathListModel
    property alias currentDriver: asioDriverSettings.currentDriver
    property bool running: Objects.mainWindow.engineRunning
    property alias bufferSize: asioDriverSettings.bufferSize
    property alias inputLatencyInSamples: asioDriverSettings.inputLatencyInSamples
    property alias outputLatencyInSamples: asioDriverSettings.outputLatencyInSamples
    property alias sampleRate: asioDriverSettings.sampleRate
    property alias inputList: asioDriverSettings.inputList
    property alias outputList: asioDriverSettings.outputList

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
                        onLanguageSelectionChanged: (currentLanguage) => {
                            EventBridge.languageSelectionChanged(currentLanguage);
                        }
                        onSystemRenderChanged: {
                            EventBridge.systemTextRenderingChanged(systemRender);
                        }
                    }
                    ASIODriverSettings {
                        id: asioDriverSettings
                        running: root.running
                        onDriverASIOSelectionChanged: (currentSelectionValue) => {
                            EventBridge.driverASIOSelectionChanged(currentSelectionValue);
                        }
                        onSampleRateChanged: {
                            EventBridge.sampleRateChanged(sampleRate);
                        }
                        onOpenASIODriverControlPanel: {
                            EventBridge.openASIODriverControlPanel();
                        }
                        onReloadDriver: {
                            EventBridge.reloadDriver();
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
