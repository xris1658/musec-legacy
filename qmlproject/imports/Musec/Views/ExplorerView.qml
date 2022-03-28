import QtQuick 2.15
import QtQuick.Controls 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    id: root
    width: 200
    height: contentColumn.height
    property int itemHeight: 20
    property int level: 0
    property int indentPerLevel: itemHeight
    property string path
    property ListModel expandableItemList: ListModel {
        dynamicRoles: true
    }
    property ListModel nonExpandableItemList: ListModel {
        dynamicRoles: true
    }
    property ItemDelegate collapsedIndicator
    property ItemDelegate expandedIndicator
    property ItemDelegate expandableItemIndicator
    property ItemDelegate nonExpandableItemIndicator
    property ItemDelegate itemDelegate
    Component.onCompleted: {
        expandableItemList.append({'name': 'Directory 1'});
        expandableItemList.append({'name': 'Directory 2'});
        expandableItemList.append({'name': 'Directory 3'});
        expandableItemList.append({'name': 'Directory 4'});
        expandableItemList.append({'name': 'Directory 5'});
        nonExpandableItemList.append({'name': 'File 1'});
        nonExpandableItemList.append({'name': 'File 2'});
        nonExpandableItemList.append({'name': 'File 3'});
        nonExpandableItemList.append({'name': 'File 4'});
        nonExpandableItemList.append({'name': 'File 5'});
    }

    onPathChanged: {
    }
    Column {
        id: contentColumn
        Repeater {
            id: expandableItemListView
            model: expandableItemList
            Column {
                MCtrl.Button {
                    property bool expanded: false
                    width: root.width
                    height: root.itemHeight
                    border.width: 0
                    Text {
                        x: root.indentPerLevel * root.level + (parent.height - height) / 2
                        width: root.width - x
                        text: qsTr("目录") + index + ": " + name + " (" + level + ")"
                        anchors.verticalCenter: parent.verticalCenter
                        color: "#FFFFFF"
                        font: Constants.font
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
                                while(explorerViewLoader.status != Loader.Ready) {}
                            }
                            explorerViewLoader.visible = true;
                            explorerViewLoader.item.level = root.level + 1;
                            notifyChildren();
                        }
                        else {
                            explorerViewLoader.visible = false;
                        }
                    }
                    onWidthChanged: {
                        notifyChildren();
                    }
                }
                Loader {
                    clip: true
                    visible: false
                    id: explorerViewLoader
                    height: item? item.height: 0
                }
            }
        }
        Repeater {
            id: nonExpandableItemListView
            model: nonExpandableItemList
            Column {
                MCtrl.Button {
                    width: root.width
                    height: root.itemHeight
                    border.width: 0
                    Text {
                        x: root.indentPerLevel * root.level + (parent.height - height) / 2
                        width: root.width - x
                        height: parent.height
                        text: qsTr("文件") + index + ": " + name + " (" + level + ")"
                        anchors.verticalCenter: parent.verticalCenter
                        color: "#FFFFFF"
                        font: Constants.font
                    }
                }
            }
        }
    }
}
