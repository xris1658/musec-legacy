import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import Qt.labs.platform 1.1
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Rectangle {
    property alias pluginPathListModel: pluginPathListView.model
    readonly property QtObject eventBridge: EventBridge
    property alias scanShortcut: checkScanShortcut.checked
    id: root
    width: 500
    height: 500
    clip: true
    color: Constants.backgroundColor
    property int firstColumnWidth: 100
    property int secondColumnWidth: 320
    property bool scanning: false

    Grid {
        columns: 2
        columnSpacing: 10
        rowSpacing: 5
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignLeft
        Text {
            width: firstColumnWidth
            height: 20
            text: qsTr("Plugin List")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: secondColumnWidth
            height: 20
        }
        Text {
            width: root.firstColumnWidth
            height: 100
            text: qsTr("Scan plugins from") + ":"
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            leftPadding: 5
            verticalAlignment: Text.AlignTop
        }
        Rectangle {
            id: pluginPathListViewPlaceholder
            width: root.secondColumnWidth
            height: 100
            color: Constants.backgroundColor
            border.color: Constants.borderColor
            clip: true
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
                    color: pluginPathListView.currentIndex == index? Constants.currentElementColor: "transparent"
                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: parent.height / 2
                        font.family: Constants.font
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
            width: root.firstColumnWidth
            height: 20
        }
        Item {
            width: root.secondColumnWidth
            height: 20
            Row {
                id: buttonPathOperationGrid
                spacing: 5
                anchors.left: parent.left
                MCtrl.Button {
                    id: buttonAddPath
                    text: qsTr("&Add...")
                    enabled: !scanning
                    width: 80
                    height: 20
                    font.family: Constants.font
                    FolderDialog {
                        id: folderDialog
                        onAccepted: {
                            let newPath = Constants.directoryFromUrl(folder);
                            EventBridge.pluginDirectoryAdded(String(newPath));
                            // If `pluginPathListModel` is changed directly, the UI won't update.
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
                    text: qsTr("&Remove")
                    width: 80
                    height: 20
                    font.family: Constants.font
                    enabled: pluginPathListModel.length && (!scanning)
                    onClicked: {
                        EventBridge.pluginDirectoryRemoved(
                            pluginPathListModel[pluginPathListView.currentIndex]
                        );
                        var newModel = pluginPathListModel;
                        newModel.splice(pluginPathListView.currentIndex, 1);
                        pluginPathListModel = newModel;
                    }
                }
            }
            MCtrl.Button {
                id: buttonRescan
                anchors.right: parent.right
                text: scanning? qsTr("Scanning..."): qsTr("&Scan Plugins")
                height: 20
                font.family: Constants.font
                enabled: !scanning
                onClicked: {
                    scanning = true;
                    EventBridge.scanPluginButtonClicked();
                }
            }
        }
        Text {
            width: 100
            height: 20
            text: qsTr("Others")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: root.secondColumnWidth
            height: 20
        }
        Text {
            width: root.firstColumnWidth
            text: qsTr("Scan Shortcuts")
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
            onCheckedChanged: {
                EventBridge.scanShortcutChanged(checked);
            }
        }
    }
}
