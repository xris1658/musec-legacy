import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml 2.15
import QtQml.Models 2.15

import qmlproject 1.0
import Musec 1.0
import Musec.Controls 1.0 as MCtrl
import Musec.Models 1.0 as MModel

Item {
    id: root
    width: 200
    height: contentColumn.height
    property string name
    property string path
    property int itemHeight: 20
    property int level: 0
    property int indentPerLevel: 10
    property MModel.FolderListModel expandableItemList: null
    property MModel.FileListModel nonExpandableItemList: null
    property Item parentIcon: null

    function requestExplorerViewComplete() {
        if(parentIcon) {
            parentIcon.loading = false;
            parentIcon.notifyChildren();
        }
    }

    function requestExplorerView(explorerView: ExplorerView) {
        Objects.currentExplorerViewOnRequest = explorerView;
    }

    MCtrl.Menu {
        id: folderOptions
        title: qsTr("文件夹操作")
        property string path
        delegate: MCtrl.MenuItem {}
        width: 300
        implicitHeight: 20
        height: contentHeight
        MCtrl.Action {
            text: qsTr("在文件资源管理器中打开(&O)")
            shortcut: "Alt+Shift+R"
            onTriggered: {
                Qt.openUrlExternally(Constants.urlFromDirectory(folderOptions.path));
            }
        }
    }

    MCtrl.Menu {
        id: fileOptions
        title: qsTr("文件操作")
        property string path
        delegate: MCtrl.MenuItem {}
        width: 300
        implicitHeight: 20
        height: contentHeight
        MCtrl.Action {
            text: qsTr("在文件资源管理器中显示(&O)")
            shortcut: "Alt+Shift+R"
            onTriggered: {
                console.log("Show file in a explorer is yet to be implemented.");
            }
        }
    }

    Column {
        id: contentColumn
        Repeater {
            id: expandableItemListView
            model: expandableItemList
            Column {
                id: expandableItemAndChild
                MCtrl.Button {
                    id: expandableItemButton
                    property bool expanded: false
                    property bool loading: false
                    width: root.width
                    height: root.itemHeight
                    border.width: 0
                    MouseArea {
                        id: expandableItemButtonMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onClicked: {
                            if(mouse.button & Qt.LeftButton) {
                                expandableItemButton.clicked();
                            }
                            else if(mouse.button & Qt.RightButton) {
                                folderOptions.path = expandableItemList.getPathOfIndex(index);
                                folderOptions.parent = expandableItemButton;
                                folderOptions.x = mouseX;
                                folderOptions.y = mouseY;
                                folderOptions.open();
                            }
                        }
                    }
                    Item {
                        id: expandableItem
                        x: root.indentPerLevel * root.level
                        width: root.width - x
                        height: parent.height
                        Row {
                            id: expandableItemRow
                            Item {
                                id: expandableItemIcon
                                width: expandableItem.height
                                height: width
                                Image {
                                    anchors.centerIn: parent
                                    source: expandableItemButton.loading? "../../../images/loading.svg":
                                            expandableItemButton.expanded? "../../../images/expanded-folder.svg":
                                                                           "../../../images/folder.svg"
                                    width: sourceSize.width
                                    height: sourceSize.height
                                }
                            }
                            Text {
                                width: expandableItem.width - expandableItemIcon.width - 2
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                                color: "#FFFFFF"
                                font: Constants.font
                                elide: Text.ElideRight
                            }
                        }
                    }
                    function notifyChildren() {
                        if(explorerViewLoader.item != null) {
                            explorerViewLoader.item.width = root.width;
                        }
                    }
                    onClicked: {
                        expanded = !expanded;
                        if(expanded) {
                            if(explorerViewLoader.item == null) {
                                expandableItemButton.loading = true;
                                explorerViewLoader.source = "ExplorerView.qml";
                            }
                        }
                    }
                    onWidthChanged: {
                        notifyChildren();
                    }
                }
                Loader {
                    id: explorerViewLoader
                    clip: true
                    visible: expandableItemButton.expanded && item
                    height: item? item.height: 0
                    onVisibleChanged: {
                        if(item) {
                            item.visible = visible;
                        }
                    }
                    onLoaded: {
                        explorerViewLoader.item.path = expandableItemList.getPathOfIndex(index);
                        explorerViewLoader.item.level = root.level + 1;
                        explorerViewLoader.item.parentIcon = expandableItemButton;
                        requestExplorerView(explorerViewLoader.item);
                    }
                }
            }
        }
        Repeater {
            id: nonExpandableItemListView
            model: nonExpandableItemList
            MCtrl.Button {
                id: nonExpandableItemButton
                width: root.width
                height: root.itemHeight
                border.width: 0
                Item {
                    id: nonExpandableItemForDrag
                    x: root.indentPerLevel * root.level
                    y: 0
                    width: root.width
                    height: nonExpandableItemButton.height
                    Drag.active: dragArea.drag.active
                    Drag.hotSpot.x: 0
                    Drag.hotSpot.y: 0
                    Drag.mimeData: {
                        "FileName": root.nonExpandableItemList.getPathOfIndex(index),
                        "FromWithin": true
                    }
                    Drag.proposedAction: Qt.CopyAction
                    Drag.supportedActions: Qt.CopyAction
                    Drag.dragType: Drag.Automatic
                    Drag.keys: ["FileName"]
                    MouseArea {
                        id: dragArea
                        parent: nonExpandableItemButton
                        anchors.fill: parent
                        drag.target: nonExpandableItemForDrag
                        onReleased: {
                            nonExpandableItemForDrag.Drag.drop();
                            resetDrag();
                        }
                        function resetDrag() {
//                            nonExpandableItemForDrag.parent = nonExpandableItemButton;
                            nonExpandableItemForDrag.x = 0;
                            nonExpandableItemForDrag.y = 0;
                            dragArea.x = 0;
                            dragArea.y = 0;
                        }
                    }
                }
                Item {
                    id: nonExpandableItem
                    x: root.indentPerLevel * root.level
                    width: root.width - x
                    height: parent.height
                    Row {
                        Item {
                            id: nonExpandableItemIcon
                            width: nonExpandableItem.height
                            height: width
                            Image {
                                anchors.centerIn: parent
                                source: "../../../images/file.svg"
                                width: sourceSize.width
                                height: sourceSize.height
                            }
                        }
                        Text {
                            width: nonExpandableItem.width - nonExpandableItemIcon.width - 2
                            text: name
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#FFFFFF"
                            font: Constants.font
                            elide: Text.ElideRight
                        }
                    }
                }
            }
        }
    }
}
