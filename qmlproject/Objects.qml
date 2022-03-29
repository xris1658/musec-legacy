pragma Singleton
import QtQuick 2.15

import Musec 1.0
import Musec.Views 1.0

QtObject {
    property SplashScreen splashScreen: null
    property Musec mainWindow: null
    property ExplorerView currentExplorerViewOnRequest: null
    onCurrentExplorerViewOnRequestChanged: {
        if(currentExplorerViewOnRequest != null) {
            mainWindow.explorerViewOnRequest = currentExplorerViewOnRequest;
            EventBridge.requestExplorerView(currentExplorerViewOnRequest);
        }
    }
}
