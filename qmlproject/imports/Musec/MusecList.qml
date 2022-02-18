import QtQuick 2.15

import Musec 1.0
import Musec.Models 1.0

QtObject {
    property PluginListModel pluginList: PluginListModel {}
    property PluginListModel midiEffectList: PluginListModel {}
    property PluginListModel instrumentlist: PluginListModel {}
    property PluginListModel audioEffectList: PluginListModel {}
//    property PluginDirectoryListModel pluginDirectoryList: PluginDirectoryListModel {}
//    property AssetListModel assetList: AssetListModel {}
    property AssetDirectoryListModel assetDirectoryList: AssetDirectoryListModel {}
}
