pragma Singleton
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import Musec 1.0
import Musec.Dialogs 1.0 as MDlg
import Musec.Views 1.0 as MView
import Musec.Entities 1.0

QtObject {
    id: eventBridge
    objectName: "eventBridge"
    // Send to C++ backend
    signal prepareToQuit()
    signal optionsWindowOpened()
    signal optionsWindowClosed()
    signal pluginDirectoryAdded(directory: string)
    signal pluginDirectoryRemoved(directory: string)
    signal scanPluginButtonClicked()
    signal openASIODriverControlPanel()
    signal driverASIOSelectionChanged(clsid: string)
    signal sampleRateChanged(sampleRate: int)
    signal systemTextRenderingChanged(newValue: bool)
    signal scanShortcutChanged(newValue: bool)
    signal openSpecialCharacterInput()
    signal setArrangementPosition(position: int)
    signal requestExplorerView()
    signal appendTrack(track: CompleteTrack)
    signal insertTrack(track: CompleteTrack, index: int)
    signal newPluginWindowReady()
    signal setIcon()
    function setWindowIcon(window: Window) {
        Objects.mainWindow.windowToSetIcon = window;
        setIcon();
    }

    signal addAssetDirectory(directory: string)
    signal renameAssetDirectory(id: int, name: string)
    signal removeAssetDirectory(id: int)

    signal playStart();
    signal playStop();

    signal languageSelectionChanged(language: string)

    signal updateCPUMeter()

    // Receive from C++ backend
    signal setBootText(newBootText: string)
    onSetBootText: {
        Objects.splashScreen.bootText = newBootText;
    }
    signal setStatusText(newText: string)
    onSetStatusText: {
        Objects.mainWindow.setStatusText(newText);
    }
    signal updateArrangementPosition(position: int)
    onUpdateArrangementPosition: {
        Objects.mainWindow.arrangementPosition = position;
    }
    signal requestExplorerViewComplete();
    onRequestExplorerViewComplete: {
        Objects.mainWindow.explorerViewOnRequest.requestExplorerViewComplete();
    }
    signal messageDialog(message: string, title: string, icon: int);
    onMessageDialog: {
        var component = Qt.createComponent("./imports/Musec/Dialogs/MessageDialog.qml");
        if(component.status == Component.Ready) {
            var rebootPrompt = component.createObject(eventBridge);
            rebootPrompt.message = message;
            rebootPrompt.title = title;
            rebootPrompt.standardButtons = DialogButtonBox.Ok;
            rebootPrompt.icon = icon;
            rebootPrompt.showNormal();
        }
    }

    signal setSystemTextRenderingComplete()
    onSetSystemTextRenderingComplete: {
        messageDialog(Strings.optionSaveRebootPromptText,
            Strings.applicationName, MDlg.MessageDialog.Icon.Info);
    }

    signal trackInserted(index: int)
    onTrackInserted: {
        Objects.mainWindow.trackInserted(index);
    }

    signal updateArrangement()
    onUpdateArrangement: {
        Objects.mainWindow.updateArrangement();
    }

    signal newPluginWindow()
    onNewPluginWindow: {
        Objects.mainWindow.newPluginWindow();
    }

    signal updateUsage(usage: double)
    onUpdateUsage: {
        Objects.mainWindow.cpu = Math.round(usage * 100);
    }

    signal setLanguageComplete()
    onSetLanguageComplete: {
        messageDialog(Strings.optionSaveRebootPromptText,
            Strings.applicationName, MDlg.MessageDialog.Icon.Info);
    }
    signal setRealtimeTimerInterval(intervalInMilliseconds: int)
    onSetRealtimeTimerInterval: {
        Objects.mainWindow.setRealtimeTimerInterval(intervalInMilliseconds);
    }
}
