import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml 2.15
import Qt.labs.platform 1.1 as Labs

import qmlproject 1.0
import Musec 1.0
import Musec.Controls 1.0 as MCtrl
import Musec.Views 1.0 as MView

Rectangle {
    id: root
    color: Constants.backgroundColor
    property alias assetDirectoryListModel: assetDirectoryList.model
    property alias pluginListModel: pluginFileList.model
    property alias midiEffectListModel: midiEffectPluginList.model
    property alias instrumentListModel: instrumentPluginList.model
    property alias audioEffectListModel: audioEffectPluginList.model

    signal requestPath(path: string);

    signal openSpecialCharacterInput()

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
           addAssetDirectory(currentFolder.toString());
       }
    }

    Rectangle {
        id: assetsAreaSearchBoxBackground
        width: parent.width
        height: 20
        color: Constants.backgroundColor
        clip: true
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
            opacity: assetsSearchBox.text.length? 0: 1
        }
        TextInput {
            id: assetsSearchBox
            property int selectStart
            property int selectEnd
            property int curPos
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 0
            color: Constants.contentColor1
            height: 20
            font.family: Constants.font
            selectByMouse: true
            selectionColor: Constants.currentElementColor
            selectedTextColor: Constants.backgroundColor
            clip: true
            function openMenu(x, y) {
                contextMenu.x = x;
                contextMenu.y = y;
                contextMenu.open();
            }
            Keys.onMenuPressed: {
                assetsSearchBox.selectStart = assetsSearchBox.selectionStart;
                assetsSearchBox.selectEnd = assetsSearchBox.selectionEnd;
                assetsSearchBox.curPos = assetsSearchBox.cursorPosition;
                openMenu(assetsSearchBox.cursorRectangle.x, assetsSearchBox.height / 2);
                assetsSearchBox.cursorPosition = assetsSearchBox.curPos;
                assetsSearchBox.select(assetsSearchBox.selectStart, assetsSearchBox.selectEnd);
            }
            MouseArea {
                id: assetSearchBoxMouseArea
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                hoverEnabled: true
                onClicked: {
                    assetsSearchBox.selectStart = assetsSearchBox.selectionStart;
                    assetsSearchBox.selectEnd = assetsSearchBox.selectionEnd;
                    assetsSearchBox.curPos = assetsSearchBox.cursorPosition;
                    assetsSearchBox.openMenu(mouse.x, mouse.y);
                    assetsSearchBox.cursorPosition = assetsSearchBox.curPos;
                    assetsSearchBox.select(assetsSearchBox.selectStart, assetsSearchBox.selectEnd);
                }
                onPressAndHold: {
                    if (mouse.source === Qt.MouseEventNotSynthesized) {
                        assetsSearchBox.selectStart = assetsSearchBox.selectionStart;
                        assetsSearchBox.selectEnd = assetsSearchBox.selectionEnd;
                        assetsSearchBox.curPos = assetsSearchBox.cursorPosition;
                        assetsSearchBox.openMenu(mouse.x, mouse.y);
                        assetsSearchBox.cursorPosition = assetsSearchBox.curPos;
                        assetsSearchBox.select(assetsSearchBox.selectStart, assetsSearchBox.selectEnd);
                    }
                }
            }
            MCtrl.Menu {
                id: contextMenu
                title: qsTr("素材搜索框选项")
                MCtrl.Action {
                    text: qsTr("撤销(&U)")
                    shortcut: "Ctrl+Z"
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("剪切(&T)")
                    shortcut: "Ctrl+X"
                }
                MCtrl.Action {
                    text: qsTr("复制(&C)")
                    shortcut: "Ctrl+C"
                }
                MCtrl.Action {
                    text: qsTr("粘贴(&P)")
                    shortcut: "Ctrl+V"
                }
                MCtrl.Action {
                    text: qsTr("删除(&D)")
                    shortcut: "Delete"
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("全选(&A)")
                    shortcut: "Ctrl+A"
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("输入特殊字符(&M)...")
                    onTriggered: {
                        openSpecialCharacterInput();
                    }
                }
            }
            MCtrl.Button {
                anchors.right: parent.right
                width: height
                height: parent.height
                text: qsTr("\u00d7")
                border.width: 0
                opacity: assetsSearchBox.text.length? 1: 0
                onClicked: {
                    if(opacity) {
                        assetsSearchBox.clear();
                    }
                    else {
                        assetsSearchBox.forceActiveFocus();
                    }
                }
            }
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
                            Component.onCompleted: {
                                font.pixelSize /= 1.25;
                            }
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
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    Rectangle {
                                        anchors.fill: parent
                                        color: rightColumn.currentIndex == 0 && directoryContent.currentIndex == index?
                                                   Constants.currentElementColor:
                                                   parent.containsMouse? Constants.mouseOverElementColor:
                                                                         Constants.backgroundColor
                                    }

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
                                            color: rightColumn.currentIndex == 0 && directoryContent.currentIndex == index?
                                                       Constants.backgroundColor:
                                                       parent.containsMouse? Constants.backgroundColor:
                                                                             Constants.contentColor1
                                        }
                                    }
                                    onClicked: {
                                        if(mouse.button == Qt.LeftButton) {
                                            rightColumn.currentIndex = 0;
                                            directoryContent.currentIndex = index;
                                            Objects.currentExplorerViewOnRequest = assetDirectoryRepeater.itemAt(index).getExplorerView();
                                        }
                                        else if(mouse.button == Qt.RightButton) {
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
                            Component.onCompleted: {
                                font.pixelSize /= 1.25;
                            }
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
                            }
                            delegate: MCtrl.Button {
                                id: categoryButton
                                width: parent.width
                                height: 20
                                border.width: 0
                                color: rightColumn.currentIndex == 1 && assetContent.currentIndex == index?
                                           Constants.currentElementColor:
                                           Constants.backgroundColor
                                onClicked: {
                                    rightColumn.currentIndex = 1;
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
                                        color: rightColumn.currentIndex == 1 && assetContent.currentIndex == index?
                                                   Constants.backgroundColor:
                                                   Constants.contentColor1
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
            StackLayout {
                id: rightColumn
                anchors.fill: parent
                currentIndex: 0
                StackLayout {
                    id: directoryContent
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    height: parent.height
                    clip: true
                    currentIndex: count
                    Repeater {
                        id: assetDirectoryRepeater
                        model: assetDirectoryListModel
                        delegate: Flickable {
                            id: rightFlickable
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            function getExplorerView() {
                                return rightExplorerView;
                            }
                            MView.ExplorerView {
                                id: rightExplorerView
                                path: directory
                                parent: directoryContent
                                y: -rightVBar.position * height
                                width: parent.width - rightVBar.width
                                visible: directoryContent.currentIndex == index
                            }
                            ScrollBar {
                                id: rightVBar
                                anchors.right: parent.right
                                size: height / rightExplorerView.height
                                position: 0
                                visible: size < 1
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
                        }
                    }
                }
                StackLayout {
                    id: assetContent
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    height: parent.height
                    clip: true
                    currentIndex: 0
                    ListView {
                        id: pluginFileList
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
}
