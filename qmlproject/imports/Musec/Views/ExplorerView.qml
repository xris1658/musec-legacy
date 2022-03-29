import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl
import Musec.Models 1.0 as MModel

Item {
    id: root
    width: 200
    height: contentColumn.height
    property string name: ""
    property string path: ""
    property int itemHeight: 20
    property int level: 0
    property int indentPerLevel: itemHeight
    property MModel.FolderListModel expandableItemList: MModel.FolderListModel {}
    property MModel.FileListModel nonExpandableItemList: MModel.FileListModel {}

    function requestExplorerViewComplete() {
        expandableItemListView.model = null;
        nonExpandableItemListView.model = null;
        expandableItemListView.model = expandableItemList;
        nonExpandableItemListView.model = nonExpandableItemList;
    }

    signal requestExplorerView(explorerView: ExplorerView)

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
                    width: root.width
                    height: root.itemHeight
                    border.width: 0
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
                                    source: "../../../images/folder.svg"
                                    width: sourceSize.width
                                    height: sourceSize.height
                                }
                            }
                            Text {
                                width: expandableItem.width - expandableItemIcon.width
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
                            explorerViewLoader.item.width = root.width - explorerViewLoader.item.x;
                        }
                    }
                    onClicked: {
                        expanded = !expanded;
                        if(expanded) {
                            if(explorerViewLoader.source == "") {
                                explorerViewLoader.source = "ExplorerView.qml";
                                // TODO: 改成异步加载
                                while(explorerViewLoader.status != Loader.Ready) {}
                            }
                            explorerViewLoader.item.path = expandableItemList.getPathOfIndex(index);
                            explorerViewLoader.item.level = root.level + 1;
                            explorerViewLoader.item.requestExplorerView(explorerViewLoader.item);
                            requestExplorerView(explorerViewLoader.item);
                            notifyChildren();
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
                }
            }

        }
        Repeater {
            id: nonExpandableItemListView
            model: nonExpandableItemList
            MCtrl.Button {
                width: root.width
                height: root.itemHeight
                border.width: 0
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
                            width: nonExpandableItem.width - nonExpandableItemIcon.width
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
