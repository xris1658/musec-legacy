import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml 2.15
import Qt.labs.platform 1.1 as Labs

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Rectangle {
    id: root
    color: Constants.backgroundColor
    property alias assetDirectoryListModel: assetDirectoryList.model
//    property alias assetListModel: assetFileList.model
    property alias pluginListModel: pluginFileList.model
    property alias midiEffectListModel: midiEffectPluginList.model
    property alias instrumentListModel: instrumentPluginList.model
    property alias audioEffectListModel: audioEffectPluginList.model

    signal addAssetDirectory(directory: string)
    signal renameAssetDirectory(id: int, name: string)
    signal removeAssetDirectory(id: int)
    signal setNewAssetDirectoryId(newId: int)
    onSetNewAssetDirectoryId: {
        assetDirectoryListModel.setProperty(assetDirectoryListModel.count - 1, "id", newId);
    }

    Popup {
        id: assetDirectoryNameEditPopup
        width: _left.width - x
        height: 20
        x: 20
        y: 0
        visible: false
        padding: 0
        background: Rectangle {
            clip: false
            color: Constants.backgroundColor
            border.width: 1
            border.color: Constants.borderColor
        }
        TextField {
            id: assetDirectoryNameEdit
            focus: true
            anchors.fill: parent
            color: Constants.contentColor1
            placeholderText: qsTr("键入文件夹名称")
            placeholderTextColor: Constants.currentElementColor
            selectionColor: Constants.currentElementColor
            selectedTextColor: Constants.backgroundColor
            padding: 0
            background: Rectangle {
                color: Constants.backgroundColor
                border.width: 1
                border.color: Constants.borderColor
            }
            font.family: Constants.font
            onAccepted: {
                renameAssetDirectory(assetDirectoryNameEditPopup.parent._id, assetDirectoryNameEdit.text);
//                assetDirectoryListModel.setProperty(assetDirectoryNameEditPopup.parent._index, "folderName", text);
                assetDirectoryNameEditPopup.visible = false;
//                let path = assetDirectoryListModel.get(assetDirectoryNameEditPopup.parent._index).path;
//                let folderName = assetDirectoryListModel.get(assetDirectoryNameEditPopup.parent._index).folderName;
            }
        }
    }

    MCtrl.Menu {
        id: assetDirectoryOptions
        title: qsTr("素材文件夹操作")
        delegate: MCtrl.MenuItem {}
        width: 200
        implicitHeight: 20
        height: contentHeight
        MCtrl.Action {
            text: qsTr("在文件资源管理器中打开(&O)")
            onTriggered: {
                let path = String(assetDirectoryOptions.parent._path);
                let url = Constants.urlFromDirectory(path.toString());
                Qt.openUrlExternally(url);
            }
        }
        MCtrl.Action {
            text: qsTr("重命名(&R)")
            onTriggered: {
                assetDirectoryNameEditPopup.parent = assetDirectoryOptions.parent;
                assetDirectoryNameEditPopup.visible = true;
                assetDirectoryNameEdit.text = assetDirectoryNameEditPopup.parent._folderName;
                assetDirectoryNameEdit.selectAll();
                assetDirectoryNameEdit.forceActiveFocus();
            }
        }
        MCtrl.Action {
            text: qsTr("从位置中移除(&D)")
            onTriggered: {
                removeAssetDirectory(assetDirectoryOptions.parent._id);
            }
        }
    }

    Labs.FolderDialog {
       id: folderDialog
       onAccepted: {
           var currentFolder = Constants.directoryFromUrl(folderDialog.currentFolder);
//           var newItem = {
//               'path': currentFolder.toString(),
//               'folderName': currentFolder.slice(currentFolder.lastIndexOf('\\') + 1).toString()
//           };
//           assetDirectoryListModel.append(newItem);
           addAssetDirectory(currentFolder.toString());
       }
    }

    Rectangle {
        id: assetsAreaSearchBoxBackground
        width: parent.width
        height: 20
        color: Constants.backgroundColor
        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 1
            color: Constants.borderColor
            border.width: 1
            border.color: color
        }

        Text {
            id: assetsAreaSearchBoxTextHint
            anchors.left: parent.left
            anchors.leftMargin: 5
            text: qsTr("搜索 (Ctrl+F)")
            font.family: Constants.font
            font.italic: true
            color: Constants.currentElementColor
            opacity: assetsSearchBox.focus && assetsSearchBox.text.length > 0? 0: 1
        }
        TextInput {
            id: assetsSearchBox
            anchors.top: parent.top
            width: parent.width - 10
            anchors.horizontalCenter: parent.horizontalCenter
            color: Constants.contentColor1
            height: 20
            font.family: Constants.font
        }
    }
    SplitView {
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height - assetsAreaSearchBoxBackground.height
        handle: Item {
            implicitWidth: 5
            implicitHeight: 5
            Rectangle {
                anchors.right: parent.right
                width: 1
                height: parent.height
                color: Constants.borderColor
            }
            Rectangle {
                id: splitDragIndicator
                anchors.fill: parent
                color: Constants.currentElementColor
                opacity: 0
                Column {
                    anchors.centerIn: parent
                    spacing: 2
                    Repeater {
                        anchors.centerIn: parent
                        model: 3
                        Rectangle {
                            width: 3
                            height: width
                            radius: width / 2
                            color: Constants.backgroundColor
                        }
                    }
                }
            }
        }
        Item {
            SplitView.preferredWidth: parent.width / 3
            height: parent.height
            clip: true
            Item {
                id: _left
                anchors.fill: parent
                anchors.margins: 5
                Column {
                    spacing: 20
                    Column {
                        Text {
                            text: qsTr("位置")
                            font.family: Constants.font
                            color: Constants.contentColor2
                        }
                        ListView {
                            id: assetDirectoryList
                            width: _left.width
                            height: contentHeight
                            interactive: false
                            delegate: Item {
                                id: _item
                                property int _id: id
                                property string _folderName: name
                                property string _name: name
                                property url _path: directory
                                property int _index: index
                                width: _left.width
                                height: 20
                                Rectangle {
                                    id: assetDirectoryButtonRect
                                    anchors.fill: parent
                                    color: Constants.mouseOverElementColor
                                    opacity: assetDirectoryButtonMouseArea.containsMouse? 1: 0
                                }
                                MouseArea {
                                    id: assetDirectoryButtonMouseArea
                                    acceptedButtons: Qt.RightButton
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    Row {
                                        Item {
                                            width: 20
                                            height: 20
                                            Image {
                                                width: 16
                                                height: width
                                                anchors.centerIn: parent
                                                source: "../../images/folder.svg"
                                            }
                                        }
                                        Text {
                                            text: name
                                            font.family: Constants.font
                                            color: Constants.contentColor1
                                        }
                                    }
                                    onClicked: {
                                        if(mouse.buttons == Qt.LeftButton) {
                                        }
                                        else {
                                            assetDirectoryOptions.parent = parent;
                                            assetDirectoryOptions.x = mouseX;
                                            assetDirectoryOptions.y = mouseY;
                                            assetDirectoryOptions.open();
                                        }
                                    }
                                }
                            }
                        }
                        MCtrl.Button {
                            width: _left.width
                            height: 20
                            border.width: 0
                            clip: true
                            Row {
                                Item {
                                    width: 20
                                    height: width
                                    Image {
                                        width: 16
                                        height: width
                                        anchors.centerIn: parent
                                        source: "../../images/addfolder.svg"
                                    }
                                }
                                Text {
                                    text: qsTr("添加位置...")
                                    font.family: Constants.font
                                    color: Constants.contentColor1
                                }
                            }
                            onClicked: {
                                folderDialog.title = "添加位置";
                                folderDialog.open();
                            }
                        }
                    }
                    Column {
                        Text {
                            text: qsTr("分类")
                            font.family: Constants.font
                            color: Constants.contentColor2
                        }
                        ListView {
                            width: _left.width
                            height: contentHeight
                            interactive: false
                            model: ListModel {
                                ListElement {
                                    name: "插件"
                                    iconPath: "../../images/plugin.svg"
                                }
                                ListElement {
                                    name: "MIDI 效果器"
                                    iconPath: "../../images/midieffect.svg"
                                }
                                ListElement {
                                    name: "音色发生器"
                                    iconPath: "../../images/instrument.svg"
                                }
                                ListElement {
                                    name: "音频效果器"
                                    iconPath: "../../images/audioeffect.svg"
                                }
//                                ListElement {
//                                    name: "音频文件"
//                                    iconPath: "../../images/audioeffect.svg"
//                                }
//                                ListElement {
//                                    name: "MIDI 片段"
//                                    iconPath: "../../images/instrument.svg"
//                                }
//                                ListElement {
//                                    name: "此项目"
//                                    iconPath: "../../images/plugin.svg"
//                                }
                            }
                            delegate: MCtrl.Button {
                                id: categoryButton
                                width: parent.width
                                height: 20
                                border.width: 0
                                onClicked: {
                                    assetContent.currentIndex = index;
                                    vbar.parent = assetContent.itemAt(index);
                                }
                                Row {
                                    Item {
                                        width: height
                                        height: categoryButton.height
                                        Image {
                                            width: 16
                                            height: width
                                            anchors.centerIn: parent
                                            source: iconPath
                                        }
                                    }

                                    Text {
                                        text: name
                                        font.family: Constants.font
                                        color: Constants.contentColor1
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Item {
            height: parent.height
            StackLayout {
                anchors.fill: parent
                anchors.margins: 1
                id: assetContent
                height: parent.height
                clip: true
                currentIndex: 0
                property ScrollBar scrollBar: ScrollBar {
                    id: vbar
                    parent: pluginFileList
                    size: assetContent.height / parent.contentHeight
                    opacity: parent.contentHeight > parent.height? 1: 0
                    width: 15
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    background: Rectangle {
                        color: Constants.backgroundColor
                    }
                    contentItem: Rectangle {
                        radius: 2
                        color: Constants.mouseOverElementColor
                    }
                }
                ListView {
                    id: pluginFileList
    //                anchors.fill: parent
    //                anchors.bottomMargin: 1
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    interactive: false
                    highlightMoveDuration: 0
                    highlightResizeDuration: 0
                    highlight: Rectangle {
                        color: Constants.currentElementColor
                    }
                    ScrollBar.vertical: vbar
                    focus: true
                    delegate: MCtrl.Button {
                        id: assetFileListButton
                        parent: pluginFileList
                        width: pluginFileList.width
                        height: 20
                        border.width: 0
                        Row {
                            Item {
                                width: height
                                height: assetFileListButton.height
                                Image {
                                    width: 16
                                    height: width
                                    anchors.centerIn: parent
                                    source: type == 1? "../../images/midieffect.svg":
                                            type == 2? "../../images/instrument.svg":
                                            type == 3? "../../images/audioeffect.svg":
                                            /*type == 0?*/ "../../images/plugin.svg"
                                }
                            }
                            Text {
                                text: name
                                font.family: Constants.font
                                color: Constants.contentColor1
                            }
                        }
                    }
                }
                ListView {
                    id: midiEffectPluginList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    interactive: false
                    highlightMoveDuration: 0
                    highlightResizeDuration: 0
                    highlight: Rectangle {
                        color: Constants.currentElementColor
                    }
                    ScrollBar.vertical: vbar
                    focus: true
                    delegate: MCtrl.Button {
                        id: midiEffectItemButton
                        parent: midiEffectPluginList
                        width: midiEffectPluginList.width
                        height: 20
                        border.width: 0
                        Row {
                            Item {
                                width: height
                                height: midiEffectItemButton.height
                                Image {
                                    width: 16
                                    height: width
                                    anchors.centerIn: parent
                                    source: type == 1? "../../images/midieffect.svg":
                                            type == 2? "../../images/instrument.svg":
                                            type == 3? "../../images/audioeffect.svg":
                                            /*type == 0?*/ "../../images/plugin.svg"

                                }
                            }
                            Text {
                                text: name
                                font.family: Constants.font
                                color: Constants.contentColor1
                            }
                        }
                    }
                }
                ListView {
                    id: instrumentPluginList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    interactive: false
                    highlightMoveDuration: 0
                    highlightResizeDuration: 0
                    highlight: Rectangle {
                        color: Constants.currentElementColor
                    }
                    ScrollBar.vertical: vbar
                    focus: true
                    delegate: MCtrl.Button {
                        id: instrumentItemButton
                        parent: instrumentPluginList
                        width: instrumentPluginList.width
                        height: 20
                        border.width: 0
                        Row {
                            Item {
                                width: height
                                height: instrumentItemButton.height
                                Image {
                                    width: 16
                                    height: width
                                    anchors.centerIn: parent
                                    source: type == 1? "../../images/midieffect.svg":
                                            type == 2? "../../images/instrument.svg":
                                            type == 3? "../../images/audioeffect.svg":
                                            /*type == 0?*/ "../../images/plugin.svg"

                                }
                            }
                            Text {
                                text: name
                                font.family: Constants.font
                                color: Constants.contentColor1
                            }
                        }
                    }
                }
                ListView {
                    id: audioEffectPluginList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    interactive: false
                    highlightMoveDuration: 0
                    highlightResizeDuration: 0
                    highlight: Rectangle {
                        color: Constants.currentElementColor
                    }
                    ScrollBar.vertical: vbar
                    focus: true
                    delegate: MCtrl.Button {
                        id: audioEffectItemButton
                        parent: audioEffectPluginList
                        width: audioEffectPluginList.width
                        height: 20
                        border.width: 0
                        Row {
                            Item {
                                width: height
                                height: audioEffectItemButton.height
                                Image {
                                    width: 16
                                    height: width
                                    anchors.centerIn: parent
                                    source: type == 1? "../../images/midieffect.svg":
                                            type == 2? "../../images/instrument.svg":
                                            type == 3? "../../images/audioeffect.svg":
                                            /*type == 0?*/ "../../images/plugin.svg"

                                }
                            }
                            Text {
                                text: name
                                font.family: Constants.font
                                color: Constants.contentColor1
                            }
                        }
                    }
                }
            }
        }

    }
}
