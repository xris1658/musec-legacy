pragma Singleton
import QtQuick 2.15

import Musec 1.0

QtObject {
    property SplashScreen splashScreen: null
    property Musec mainWindow: null
    property Item currentExplorerViewOnRequest: null
    onCurrentExplorerViewOnRequestChanged: {
        if(mainWindow && currentExplorerViewOnRequest) {
            mainWindow.explorerViewOnRequest = currentExplorerViewOnRequest;
            EventBridge.requestExplorerView();
        }
    }
}
