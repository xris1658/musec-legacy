import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import Qt.labs.platform 1.1
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Rectangle {
    property alias pluginPathListModel: pluginPathListView.model
    readonly property EventBridge eventBridge: EventBridge
    property alias scanShortcut: checkScanShortcut.checked
    id: root
    width: 500
    height: 500
    clip: true
    color: Constants.backgroundColor
    property bool scanning: false

    Grid {
        columns: 2
        columnSpacing: 10
        rowSpacing: 5
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignLeft
        Text {
            width: 100
            height: 20
            text: qsTr("插件列表")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: 320
            height: 20
        }
        Text {
            width: 100
            height: 100
            text: qsTr("插件扫描目录")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignTop
        }
        Rectangle {
            width: 300
            height: 100
            color: Constants.backgroundColor
            border.color: Constants.borderColor
            ListView {
                id: pluginPathListView
                anchors.fill: parent
                anchors.margins: parent.border.width
                model: []
                delegate: MCtrl.Button {
                    border.width: 0
                    width: parent.width
                    height: 20
                    clip: true
                    color: pluginPathListView.currentIndex == index? Constants.currentElementColor: Constants.backgroundColor
                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: parent.height / 2
                        font: Constants.font
                        text: modelData
                        color: pluginPathListView.currentIndex == index? Constants.backgroundColor: Constants.contentColor1
                    }
                    onClicked: {
                        pluginPathListView.currentIndex = index;
                    }
                }
                ScrollBar.vertical: ScrollBar {
                    id: vbar
                    opacity: pluginPathListView.contentHeight > pluginPathListView? 1: 0
                    parent: pluginPathListView
                }
            }
        }
        Item {
            width: 100
            height: 20
        }
        Item {
            width: 300
            height: 20
            Grid {
                rows: 1
                spacing: 5
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalItemAlignment: Grid.AlignRight
                verticalItemAlignment: Grid.AlignVCenter
                MCtrl.Button {
                    id: buttonAddPath
                    text: qsTr("+ 添加目录...")
                    width: 100
                    height: 20
                    font.family: Constants.font
                    FolderDialog {
                        id: folderDialog
                        onAccepted: {
                            let newPath = Constants.directoryFromUrl(folder);
                            EventBridge.pluginDirectoryAdded(String(newPath));
                            // 不能直接对 pluginPathListModel 进行修改，否则界面中的数据不会更新。
                            // 貌似 pluginPathListModel 是全局对象？
                            var oldModel = pluginPathListModel;
                            oldModel.push(newPath);
                            pluginPathListModel = oldModel;
                        }
                    }
                    onClicked: {
                        folderDialog.open();
                    }
                }
                MCtrl.Button {
                    id: buttonDelPath
                    text: qsTr("- 移除选中目录")
                    width: 120
                    height: 20
                    font.family: Constants.font
                    enabled: pluginPathListModel.length
                    onClicked: {
//                        EventBridge.pluginDirectoryRemoved(
//                            pluginPathListModel.itemAtIndex(
//                                pluginPathListView.currentIndex
//                            )
//                        );
                        EventBridge.pluginDirectoryRemoved(
                            pluginPathListModel[pluginPathListView.currentIndex]
                        );
                        var newModel = pluginPathListModel;
                        newModel.splice(pluginPathListView.currentIndex, 1);
                        pluginPathListModel = newModel;
                    }
                }
            }
        }
        Item {
            width: 100
            height: 20
        }
        Item {
            width: 300
            height: 20
            Grid {
                id: scanPluginGrid
                rows: 1
                spacing: 5
                anchors.right: parent.right
                anchors.top: parent.top
                horizontalItemAlignment: Grid.AlignRight
                verticalItemAlignment: Grid.AlignTop
                MCtrl.Button {
                    id: buttonRescan
                    text: scanning? qsTr("正在扫描插件..."): qsTr("扫描插件")
                    width: 225
                    height: 20
                    font.family: Constants.font
                    enabled: !scanning
                    onClicked: {
                        scanning = true;
                        EventBridge.scanPluginButtonClicked();
                    }
                }
            }
        }
        Text {
            width: 100
            height: 20
            text: qsTr("其他")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: 320
            height: 20
        }
        Text {
            width: 100
            text: qsTr("扫描快捷方式")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.CheckBox {
            id: checkScanShortcut
            width: 16
            height: 16
            tristate: false
        }
        Text {
            width: 100
            text: qsTr("插件格式优先级")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxPluginPriority
            model: ["VST2优先", "VST3优先"]
            font.family: Constants.font
            width: 100
        }
    }
}
