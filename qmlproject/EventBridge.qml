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
    // 发往 C++ 后端的信号
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

    // 收到 C++ 后端的信号
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
        // 弹出需要重启的对话框
        var component = Qt.createComponent("./imports/Musec/Dialogs/MessageDialog.qml");
        if(component.status == Component.Ready) {
            var rebootPrompt = component.createObject(eventBridge);
            rebootPrompt.message = qsTr("您更改的设置已经保存，需要重新启动才能生效。")
            rebootPrompt.title = qsTr("Musec");
            rebootPrompt.standardButtons = DialogButtonBox.Ok;
            rebootPrompt.icon = MDlg.MessageDialog.Icon.Info;
            rebootPrompt.showNormal();
        }
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
}
