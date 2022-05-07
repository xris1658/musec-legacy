import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Window 2.15
import Qt.labs.platform 1.1 as Labs
import QtQml.Models 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl
import Musec.Views 1.0 as MView

ApplicationWindow {
    id: mainWindow
    readonly property Item mainArea: mainFunctionArea
    MusecList { id: musecList }
    property alias pluginList: musecList.pluginList
    property alias assetDirectoryList: musecList.assetDirectoryList
    property alias midiEffectList: musecList.midiEffectList
    property alias instrumentList: musecList.instrumentlist
    property alias audioEffectList: musecList.audioEffectList
    property alias trackList: musecList.trackList
    property alias pluginDirectoryList: optionsWindow.pluginDirectoryList
    property alias driverList: optionsWindow.driverList
    property alias currentDriver: optionsWindow.currentDriver
    readonly property EventBridge eventBridge: EventBridge
    property bool canClose: false
    property PluginWindow windowForPlugin: null
    property Window windowToSetIcon: null

    property MView.ExplorerView explorerViewOnRequest

    property alias assetVisible: actionAssetVisible.checked
    property alias midiEditorVisible: actionMidiEditorVisible.checked
    property alias mixerVisible: actionMixerVisible.checked

    property alias bpm: mainBar.bpm

    property alias arrangementPosition: arrangement.position

    signal setStatusText(newText: string)
    onSetStatusText: {
        status.text = newText;
    }

    signal updateArrangement()
    onUpdateArrangement: {
        arrangement.updateArrangement();
    }

    OptionsWindow {
        id: optionsWindow
        objectName: "optionsWindow"
        visible: false
        onVisibilityChanged: {
            if(visibility == 0) {
                EventBridge.optionsWindowClosed();
            }
        }
    }
    // 自定义属性
    property string filePath: ""
    property string openedFile: qsTr("未命名项目")
    property bool newProject: true
    property bool changed: false
    property bool maximized: true
    property bool _fullscreen: false
    property bool playing: false

    // 内置属性
    width: Constants.width
    height: Constants.height
    color: Constants.backgroundColor2
    visible: true
    visibility: "Maximized"
    title: "Musec - " + openedFile + (changed? "*": "")

    // 函数
    function saveProject() {
        // TODO
    }

    function toggleFullscreen() {
        if(_fullscreen == false)
        {
            _fullscreen = true;
            mainWindow.showFullScreen();
        }
        else {
            _fullscreen = false;
            if(mainWindow.maximized) mainWindow.showMaximized();
            else mainWindow.showNormal();
        }
    }

    function newPluginWindow() {
        var component = Qt.createComponent(
            "qrc:/qmlproject/imports/Musec/PluginWindow.qml",
            mainWindow);
        if(component.status == Component.Ready) {
            var window = component.createObject(mainWindow);
            mainWindow.windowForPlugin = window;
            eventBridge.newPluginWindowReady();
            // TODO: 将插件窗口传给后台
        }
        else {
            console.log(component.errorString());
        }
    }

    // 事件
    Component.onCompleted: {
        Objects.mainWindow = mainWindow;
    }

    onVisibilityChanged: {
        if(visibility != 5/*FullScreen*/)
        {
            if(visibility == 4/*Maximized*/) maximized = true;
            else maximized = false;
        }
    }

    // 对于使用集成显卡绘制的 QML 窗口: 第一次全屏时, 或者全屏时
    // 焦点失而复得 (e.g. 打开或切换到另一个窗口, 然后关闭这一窗
    // 口) 后并发生交互 (e.g. 菜单栏项目高亮) 时均会重绘. 此操作
    // 较为耗时 (1-3s), 容易使用户产生程序运行不流畅的感觉.

    // 解决: 1. 在应用启动时进行一次全屏, 然后取消全屏.
    // (在 MainWindow::openMainWindow() 中进行不会暴露这一行为.)
//    Component.onCompleted: {
//        toggleFullscreen();
//        toggleFullscreen();
//    }
    // 2. 在丢失焦点时使窗口退出全屏, 再次得到焦点时恢复全屏.
    onActiveFocusItemChanged: {
        if(activeFocusItem)
        {
            if(_fullscreen) {
                mainWindow.showFullScreen();
            }
        }
        else {
            var menuCount = mainMenuBar.menus.length;
            for(var i = 0; i < menuCount; ++i) {
                mainMenuBar.menus[i].dismiss();
            }
            if(_fullscreen) {
                if(mainWindow.maximized) mainWindow.showMaximized();
                else mainWindow.showNormal();
            }
        }
    }

    signal trackInserted(index: int)
    onTrackInserted: {
        arrangement.appendTrackComplete(index);
    }

    signal readyToQuit()
    onReadyToQuit: {
        canClose = true;
        close();
    }

    onClosing: {
        if(!canClose) {
            close.accepted = false;
            if(_fullscreen) {
                toggleFullscreen();
            }
            quitAction.trigger();
        }
    }

    // 对话框和连接
    Labs.FileDialog {
        id: fileDialog
        nameFilters: ["Musec 项目文件 (*.mvr)"]
    }

    // 信号
    signal showWindow()
    onShowWindow: {
        showMaximized();
    }

    signal setNewAssetDirectoryId(newId: int)
    onSetNewAssetDirectoryId: {
        //
    }

    // 轨道信息
    ListModel {
        id: tracks
        dynamicRoles: true
    }

    //菜单栏
    menuBar: MCtrl.MenuBar {
        id: mainMenuBar
        backgroundColor: "#000000"
        MCtrl.Menu {
            title: qsTr("&Musec")

            MCtrl.Action {
                text: qsTr("关于 Musec(&A)...")
                onTriggered: {
                    var component = Qt.createComponent("AboutWindow.qml");
                    if(component.status == Component.Ready) {
                        var aboutWindow = component.createObject(mainWindow);
                        EventBridge.setWindowIcon(aboutWindow);
                        aboutWindow.showNormal();
                    }
                }
            }
            MCtrl.Action {
                text: qsTr("关于 Qt(&Q)...")
                onTriggered: {
                    var component = Qt.createComponent("AboutQtWindow.qml");
                    if(component.status == Component.Ready) {
                        var aboutWindow = component.createObject(mainWindow);
                        EventBridge.setWindowIcon(aboutWindow);
                        aboutWindow.showNormal();
                    }
                }
            }

            MCtrl.MenuSeparator {}
            MCtrl.Action { text: qsTr("键盘快捷键(&K)...") }
            MCtrl.Action { text: qsTr("检查更新(&U)...") }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("选项(&P)...")
                shortcut: "Ctrl+,"
                onTriggered: {
                    EventBridge.setWindowIcon(optionsWindow);
                    optionsWindow.showNormal();
                    EventBridge.optionsWindowOpened();
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: quitAction
                text: qsTr("退出(&X)")
                shortcut: "Ctrl+Q"
                onTriggered: {
                    EventBridge.prepareToQuit();
                }
            }
        }
        MCtrl.Menu {
            title: qsTr("文件(&F)")

            MCtrl.Action {
                text: qsTr("新建(&N)")
                shortcut: "Ctrl+N"
            }
            MCtrl.Action {
                text: qsTr("打开(&O)")
                shortcut: "Ctrl+O"
            }
            MCtrl.Menu {
                title: qsTr("最近打开的文件")

                MCtrl.Action {
                    text: qsTr("列表为空")
                    enabled: false
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("清除列表")
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("保存(&S)")
                shortcut: "Ctrl+S"
                onTriggered: {
                    if(newProject) {
                        //
                    }
                    else {
                        saveProject();
                    }
                }
            }
            MCtrl.Action {
                id: actionSaveAs
                text: qsTr("另存为(&A)...")
                shortcut: "Ctrl+Shift+S"
                onTriggered: {
                    fileDialog.title = "保存项目文件";
                    fileDialog.fileMode = Labs.FileDialog.SaveFile;
                    fileDialog.open();
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Menu {
                title: qsTr("导出(&E)")

                MCtrl.Action {
                    text: qsTr("并轨(&B)...")
                    shortcut: "Ctrl+Shift+R"
                    onTriggered: {
                        var component = Qt.createComponent("MixdownWindow.qml");
                        if(component.status == Component.Ready) {
                            var mixdownWindow = component.createObject(mainWindow);
                            EventBridge.setWindowIcon(mixdownWindow);
                            mixdownWindow.showNormal();
                        }
                    }
                }
                MCtrl.Action {
                    text: qsTr("选择分轨(&T)...")
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("打包项目文件(&P)...")
                }
                MCtrl.Action {
                    text: qsTr("MusicXML(&M)...")
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("五线谱(&S)...")
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("项目属性(&P)...")
                shortcut: "Alt+3"
                onTriggered: {
                    var component = Qt.createComponent("ProjectPropertiesWindow.qml");
                    if(component.status == Component.Ready) {
                        var projectPropertiesWindow = component.createObject(mainWindow);
                        projectPropertiesWindow.fileName = openedFile;
                        projectPropertiesWindow.filePath = filePath;
                        projectPropertiesWindow.newProject = newProject;
                        EventBridge.setWindowIcon(projectPropertiesWindow);
                        projectPropertiesWindow.showNormal();
                    }
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("还原至上次保存(&R)...")
                shortcut: "F12"
            }
        }
        MCtrl.Menu {
            title: qsTr("编辑(&E)")

            MCtrl.Action {
                text: qsTr("撤销(&U)")
                shortcut: "Ctrl+Z"
                enabled: false
            }
            MCtrl.Action {
                text: qsTr("重做(&R)")
                shortcut: "Ctrl+Y"
                enabled: false
            }
            MCtrl.Action {
                text: qsTr("历史记录(&H)...")
                shortcut: "Ctrl+H"
                enabled: false
            }

            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("剪切(&T)")
                shortcut: "Ctrl+X"
                onTriggered: {
                    changed = true;
                }
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
                text: qsTr("跳转至起始")
                shortcut: "Home"
            }
            MCtrl.Action {
                text: qsTr("跳转至结束")
                shortcut: "End"
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: actionLoop
                text: qsTr("循环")
                shortcut: "Ctrl+L"
                checkable: true
                checked: false
            }
            MCtrl.Action {
                text: qsTr("设置入点")
                shortcut: "I"
            }
            MCtrl.Action {
                text: qsTr("设置出点")
                shortcut: "O"
            }
        }
        MCtrl.Menu {
            title: qsTr("查看(&V)")

            MCtrl.Action {
                id: actionFullScreen
                text: qsTr("全屏")
                shortcut: "F11"
                checkable: true
                Binding on checked {
                    value: _fullscreen
                }
                onTriggered: {
                    toggleFullscreen();
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: actionAssetVisible
                text: qsTr("素材(&A)")
                shortcut: "F8"
                checkable: true
                checked: true
            }
            MCtrl.Action {
                id: actionMidiEditorVisible
                text: qsTr("片段编辑器(&C)")
                shortcut: "F7"
                checkable: true
                checked: true
            }
            MCtrl.Action {
                id: actionMixerVisible
                text: qsTr("混音器(&M)")
                shortcut: "F9"
                checkable: true
                checked: true
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: actionFollow
                text: qsTr("跟随(&F)")
                shortcut: "Ctrl+Shift+F"
                checkable: true
                checked: false
            }
            MCtrl.Action {
                id: actionShowKeyScale
                text: qsTr("显示音阶(&K)")
                checkable: true
                checked: midiEditor.showKeyScale
                onTriggered: {
                    midiEditor.showKeyScale = checked;
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: actionSnap
                text: qsTr("对齐(&S)")
                checkable: true
                checked: false
            }
            MCtrl.MenuSeparator {}
            MCtrl.Menu {
                id: menuShowMasterTrack
                title: qsTr("主轨道")

                Repeater {
                    model: arrangement.showMasterTrackModel
                    MCtrl.MenuItem {
                        text: modelData
                        Item {
                            opacity: index == arrangement.showMasterTrack? 1: 0
                            width: parent.height
                            height: parent.height
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width / 3
                                height: parent.width / 3
                                radius: width / 2
                                color: parent.parent.highlighted? Constants.menuHighlightContentColor: Constants.menuContentColor
                            }
                        }
                        onTriggered: {
                            arrangement.showMasterTrack = index;
                        }
                    }
                }
            }
            MCtrl.Menu {
                title: qsTr("编排视图对齐单位")
                Repeater {
                    model: arrangement.arrangementSnapUnit.model
                    MCtrl.MenuItem {
                        text: modelData
                        Item {
                            opacity: index == arrangement.arrangementSnapUnit.currentIndex? 1: 0
                            width: parent.height
                            height: parent.height
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width / 3
                                height: parent.width / 3
                                radius: width / 2
                                color: parent.parent.highlighted? Constants.menuHighlightContentColor: Constants.menuContentColor
                            }
                        }
                        onTriggered: {
                            arrangement.arrangementSnapUnit.currentIndex = index;
                        }
                    }
                }
            }
            MCtrl.Menu {
                title: qsTr("片段编辑器对齐单位")
                Repeater {
                    model: midiEditor.midiEditorSnapUnit.model
                    MCtrl.MenuItem {
                        text: modelData
                        Item {
                            opacity: index == midiEditor.midiEditorSnapUnit.currentIndex? 1: 0
                            width: parent.height
                            height: parent.height
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width / 3
                                height: parent.width / 3
                                radius: width / 2
                                color: parent.parent.highlighted? Constants.menuHighlightContentColor: Constants.menuContentColor
                            }
                        }
                        onTriggered: {
                            midiEditor.midiEditorSnapUnit.currentIndex = index;
                        }
                    }
                }
                MCtrl.MenuSeparator {}
                MCtrl.MenuItem {
                    text: qsTr("三连音")
                    checkable: true
                    checked: midiEditor.triplets
                    onTriggered: {
                        midiEditor.triplets = checked;
                    }
                }
            }
        }
        MCtrl.Menu {
            title: qsTr("工具(&T)")

            MCtrl.Action {
                id: menuItemMetronome
                text: qsTr("节拍器")
                shortcut: "Ctrl+M"
                checkable: true
                checked: false
            }
            MCtrl.Action {
                id: actionEnableKeyboard
                text: qsTr("电脑键盘映射为琴键")
                checkable: true
                checked: false
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("测速...")
                onTriggered: {
                    var component = Qt.createComponent("TapTempoWindow.qml");
                    if(component.status == Component.Ready) {
                        var tapTempoWindow = component.createObject(mainWindow);
                        tapTempoWindow.bpm = mainWindow.bpm;
                        EventBridge.setWindowIcon(tapTempoWindow);
                        tapTempoWindow.showNormal();
                    }
                }
            }
        }
        MCtrl.Menu {
            title: qsTr("帮助(&H)")

            MCtrl.Action {
                text: qsTr("用户指南(&M)")
                shortcut: "F1"
            }
            MCtrl.Action {
                text: qsTr("快速教程(&Q)")
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("注册产品(&R)...")
            }
            MCtrl.Action {
                text: qsTr("用户支持(&S)...")
            }
        }
    }

    //顶部的工具栏
    MainBar {
        id: mainBar
        onPlayStart: {
            EventBridge.playStart();
        }
        onPlayStop: {
            EventBridge.playStop();
        }
    }

    //功能区
    Item {
        id: mainFunctionArea
        anchors.top: mainBar.bottom
        anchors.bottom: statusBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        MCtrl.SplitView { orientation: Qt.Horizontal
            anchors.fill: parent
                // // 素材
            Item {
                visible: assetVisible
                SplitView.preferredWidth: parent.width / 5
                SplitView.minimumHeight: 20
                FunctionArea {
                    id: assetsFunctionArea
                    title: qsTr("素材")
                    Assets {
                        id: assets
                        parent: assetsFunctionArea.contentArea
                        anchors.fill: parent
                        anchors.margins: 1
                        y: parent.y
                        assetDirectoryListModel: musecList.assetDirectoryList
                        pluginListModel: musecList.pluginList
                        midiEffectListModel: musecList.midiEffectList
                        instrumentListModel: musecList.instrumentlist
                        audioEffectListModel: musecList.audioEffectList
                        onOpenSpecialCharacterInput: {
                            EventBridge.openSpecialCharacterInput();
                        }
                        onAddAssetDirectory: {
                            EventBridge.addAssetDirectory(directory);
                        }
                        onRenameAssetDirectory: {
                            EventBridge.renameAssetDirectory(id, name);
                        }
                        onRemoveAssetDirectory: {
                            EventBridge.removeAssetDirectory(id);
                        }
                    }
                }
            }
            // // 编排视图 + 片段编辑器 + 混音器
            MCtrl.SplitView { orientation: Qt.Vertical
                anchors.top: parent.top
                anchors.right: parent.right
                // // 编排视图 + 片段编辑器
                MCtrl.SplitView {
                    orientation: Qt.Horizontal
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width
                    SplitView.preferredHeight: parent.height * 0.625
                    SplitView.minimumHeight: 20
                    // 编排视图
                    Item {
                        SplitView.preferredWidth: parent.width * 0.6
                        FunctionArea {
                            id: arrangementFunctionArea
                            title: qsTr("编排视图")
                            Arrangement {
                                id: arrangement
                                timelineNumerator: mainBar.numerator
                                tracks: trackList
                                parent: arrangementFunctionArea.contentArea
                                barCount: 40
                                anchors.fill: parent
                                anchors.margins: 1
                                y: parent.y
                                loop: actionLoop.checked
                                position: 0
                                onAppendTrack: {
                                    eventBridge.appendTrack(track);
                                }
                                onInsertTrack: {
                                    eventBridge.insertTrack(track, index);
                                }
                            }
                        }
                    }
                    // 片段编辑器
                    Item {
                        visible: midiEditorVisible
                        FunctionArea {
                            id: editorFunctionArea
                            property string clipName: ""
                            title: qsTr("编辑器")
                            MidiEditor {
                                id: midiEditor
                                parent: editorFunctionArea.contentArea
                                anchors.fill: parent
                                anchors.margins: 1
                                y: parent.y
                            }
                        }
                    }
                }
                // // 混音器
                Item {
                    SplitView.minimumHeight: 20
                    visible: mixerVisible
                    FunctionArea {
                        id: mixerFunctionArea
                        title: qsTr("混音器")
                        Mixer {
                            parent: mixerFunctionArea.contentArea
                            anchors.fill: parent
                            anchors.margins: 1
                            y: parent.y
                            tracks: trackList
                        }
                    }
                }
            }
        }
    }
    //状态栏
    Rectangle {
        id: statusBar
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: parent.width
        height: 20
        color: Constants.backgroundColor
        Rectangle {
            x: 0
            y: 0
            width: parent.width
            height: 1
            color: Constants.borderColor
        }
        Text {
            id: status
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("就绪")
            font.family: Constants.font
            color: Constants.contentColor1
        }
        Button {
            id: quitFullScreen
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: parent.height / 2
            height: parent.height / 2
            enabled: mainWindow.visibility == 5? true: false
            opacity: quitFullScreen.enabled? 1: 0
            contentItem: Text {
                anchors.centerIn: parent
                text: "↖"
                font.family: Constants.font
                font.pixelSize: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                anchors.fill: parent
                color: Constants.exitFullscreenButtonBackgroundColor
            }
            onClicked: {
                toggleFullscreen();
            }
        }
    }
}
