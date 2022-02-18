pragma Singleton
import QtQuick 2.15
import QtQuick.Window 2.15

import Musec 1.0

QtObject {
    id: eventBridge
    objectName: "eventBridge"
    // 发往 C++ 后端的信号
    signal exitASIOThread()
    signal optionsWindowOpened()
    signal optionsWindowClosed()
    signal pluginDirectoryAdded(directory: string)
    signal pluginDirectoryRemoved(directory: string)
    signal scanPluginButtonClicked()
    signal openASIODriverControlPanel()
    signal driverASIOSelectionChanged(clsid: string)

    signal addAssetDirectory(directory: string)
    signal renameAssetDirectory(id: int, name: string)
    signal removeAssetDirectory(id: int)

    // 收到 C++ 后端的信号
    signal setBootText(newBootText: string)
    onSetBootText: {
        Objects.splashScreen.bootText = newBootText;
    }
}
