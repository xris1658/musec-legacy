pragma Singleton
import QtQuick 2.15

import Musec 1.0

QtObject {
    property QtObject splashScreen: null
    property QtObject mainWindow: null
    property QtObject currentExplorerViewOnRequest: null
    onCurrentExplorerViewOnRequestChanged: {
        if(mainWindow && currentExplorerViewOnRequest) {
            mainWindow.explorerViewOnRequest = currentExplorerViewOnRequest;
            EventBridge.requestExplorerView();
        }
    }
}
