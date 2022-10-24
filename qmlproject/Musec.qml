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
import Musec.Models 1.0 as MModel

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
    property alias masterTrackPluginSequence: mixer.masterTrackPluginSequence
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

    property alias engineRunning: mainBar.engineRunning
    property alias bpm: mainBar.bpm
    property alias cpu: mainBar.cpu

    property alias arrangementPosition: arrangement.position

    property MModel.VolumeFaderScaleModel volumeFaderScaleModel: MModel.VolumeFaderScaleModel {
        maxDecibel: 6
        minDecibel: -144
        Component.onCompleted: {
            insertPoint(0.9, 0);
            insertPoint(0.8, -6);
            insertPoint(0.7, -12);
            insertPoint(0.6, -18);
            insertPoint(0.5, -24);
            insertPoint(0.4, -36);
            insertPoint(0.3, -54);
            insertPoint(0.2, -78);
            insertPoint(0.1, -108);
            mixer.volumeFaderScaleModel = this;
        }
    }
    onEngineRunningChanged: {
        if(engineRunning) {
            realtimeUpdateTimer.start();
        }
        else {
            realtimeUpdateTimer.stop();
        }
    }

    signal setStatusText(newText: string)
    onSetStatusText: (newText) => {
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
        onVisibilityChanged: (visibility) => {
            if(visibility == 0) {
                EventBridge.optionsWindowClosed();
            }
        }
    }

    property string filePath: ""
    property string openedFile: qsTr("Untitled project")
    property bool newProject: true
    property bool dirty: false
    property bool maximized: true
    property bool _fullscreen: false
    property bool playing: false

    width: Constants.width
    height: Constants.height
    color: Constants.backgroundColor2
    visible: true
    visibility: "Maximized"
    title: (dirty? "*": "") + openedFile + " - Musec"

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
        }
        else {
            console.log(component.errorString());
        }
    }

    function setRealtimeTimerInterval(intervalInMilliseconds: int) {
        realtimeUpdateTimer.interval = intervalInMilliseconds;
    }

    Component.onCompleted: {
        Objects.mainWindow = mainWindow;
    }

    onVisibilityChanged: (visibility) => {
        if(visibility != 5/*FullScreen*/)
        {
            if(visibility == 4/*Maximized*/) maximized = true;
            else maximized = false;
        }
    }

    // If the window is painted by the integrated graphics (e.g. Intel HD Graphics),
    // the window needs to update graphics when
    // 1. toggled to full screen for the first time, or
    // 2. regain focus after losing it in full screen mode.
    // Updating graphics take about 1-3 seconds.
    // To make them invisible by the user:
    // 1. Toggle the main window to full screen, and then quit full screen right after this.
    // This is done on opening the window.
    // (Doing this in MainWindow::openMainWindow() won't reveal this operation.)
    // 2. Quit full screen mode on losing focus, and enter full screen mode on regaining focus.
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
    onTrackInserted: (index) => {
        arrangement.appendTrackComplete(index);
    }

    signal readyToQuit()
    onReadyToQuit: {
        canClose = true;
        close();
    }

    onClosing: (close) => {
        if(!canClose) {
            close.accepted = false;
            if(_fullscreen) {
                toggleFullscreen();
            }
            quitAction.trigger();
        }
    }

    Timer {
        id: realtimeUpdateTimer
        onTriggered: {
            if(mainWindow.engineRunning) {
                eventBridge.updateCPUMeter();
            }
            start();
        }
    }

    Labs.FileDialog {
        id: fileDialog
        nameFilters: [qsTr("Musec project file (*.mvr)")]
    }

    signal showWindow()
    onShowWindow: {
        showMaximized();
    }

    signal setNewAssetDirectoryId(newId: int)
    onSetNewAssetDirectoryId: {
        //
    }

    ListModel {
        id: tracks
        dynamicRoles: true
    }

    menuBar: MCtrl.MenuBar {
        id: mainMenuBar
        backgroundColor: "#000000"
        MCtrl.Menu {
            title: "&Musec"

            MCtrl.Action {
                text: qsTr("&About Musec...")
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
                text: qsTr("About &Qt...")
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
            MCtrl.Action { text: qsTr("&Keyboard Shortcuts...") }
            MCtrl.Action { text: qsTr("Check for &Updates...") }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("&Preferences...")
                shortcut: "Ctrl+,"
                onTriggered: {
                    EventBridge.setWindowIcon(optionsWindow);
                    optionsWindow.showNormal();
                    EventBridge.optionsWindowOpened();
                    optionsWindow.requestActivate();
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: quitAction
                text: qsTr("E&xit")
                shortcut: "Ctrl+Q"
                onTriggered: {
                    EventBridge.prepareToQuit();
                }
            }
        }
        MCtrl.Menu {
            title: qsTr("&File")

            MCtrl.Action {
                text: qsTr("&New")
                shortcut: "Ctrl+N"
            }
            MCtrl.Action {
                text: qsTr("&Open")
                shortcut: "Ctrl+O"
            }
            MCtrl.Menu {
                title: qsTr("Recent files")

                MCtrl.Action {
                    text: qsTr("Empty list")
                    enabled: false
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("Clear list")
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("&Save")
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
                text: qsTr("Save &As...")
                shortcut: "Ctrl+Shift+S"
                onTriggered: {
                    fileDialog.title = qsTr("Save Project");
                    fileDialog.fileMode = Labs.FileDialog.SaveFile;
                    fileDialog.open();
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Menu {
                title: qsTr("&Export")

                MCtrl.Action {
                    text: qsTr("&Bounce...")
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
                    text: qsTr("Multiple &Tracks...")
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("&Packed Project...")
                }
                MCtrl.Action {
                    text: qsTr("&MusicXML...")
                }
                MCtrl.MenuSeparator {}
                MCtrl.Action {
                    text: qsTr("&Score...")
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("&Project Properties...")
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
                text: qsTr("&Revert to Last Save...")
                shortcut: "F12"
            }
        }
        MCtrl.Menu {
            title: qsTr("&Edit")

            MCtrl.Action {
                text: Constants.undoTextWithMnemonic
                shortcut: "Ctrl+Z"
                enabled: false
            }
            MCtrl.Action {
                text: Constants.redoTextWithMnemonic
                shortcut: "Ctrl+Y"
                enabled: false
            }
            MCtrl.Action {
                text: qsTr("&History...")
                shortcut: "Ctrl+H"
                enabled: false
            }

            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: Constants.cutTextWithMnemonic
                shortcut: "Ctrl+X"
                onTriggered: {
                    dirty = true;
                }
            }
            MCtrl.Action {
                text: Constants.copyTextWithMnemonic
                shortcut: "Ctrl+C"
            }
            MCtrl.Action {
                text: Constants.pasteTextWithMnemonic
                shortcut: "Ctrl+V"
            }
            MCtrl.Action {
                text: Constants.deleteTextWithMnemonic
                shortcut: "Delete"
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: Constants.selectAllTextWithMnemonic
                shortcut: "Ctrl+A"
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("Jump to Start")
                shortcut: "Home"
            }
            MCtrl.Action {
                text: qsTr("Jump to End")
                shortcut: "End"
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: actionLoop
                text: qsTr("Loop")
                shortcut: "Ctrl+L"
                checkable: true
                checked: false
            }
            MCtrl.Action {
                text: qsTr("Set Loop In Point")
                shortcut: "I"
            }
            MCtrl.Action {
                text: qsTr("Set Loop Out Point")
                shortcut: "O"
            }
        }
        MCtrl.Menu {
            title: qsTr("&View")

            MCtrl.Action {
                id: actionFullScreen
                text: qsTr("&Fullscreen")
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
                text: qsTr("&Assets")
                shortcut: "F8"
                checkable: true
                checked: true
            }
            MCtrl.Action {
                id: actionMidiEditorVisible
                text: qsTr("&Clip Editor")
                shortcut: "F7"
                checkable: true
                checked: true
            }
            MCtrl.Action {
                id: actionMixerVisible
                text: qsTr("&Mixer")
                shortcut: "F9"
                checkable: true
                checked: true
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: actionFollow
                text: qsTr("&Follow")
                shortcut: "Ctrl+Shift+F"
                checkable: true
                checked: false
            }
            MCtrl.Action {
                id: actionShowKeyScale
                text: qsTr("Show &Key Scale")
                checkable: true
                checked: midiEditor.showKeyScale
                onTriggered: {
                    midiEditor.showKeyScale = checked;
                }
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                id: actionSnap
                text: qsTr("&Snap")
                checkable: true
                checked: false
            }
            MCtrl.MenuSeparator {}
            MCtrl.Menu {
                id: menuShowMasterTrack
                title: qsTr("Master Track")

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
                title: qsTr("Arrangement Snap Unit")
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
                title: qsTr("Editor Snap Unit")
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
                    text: qsTr("Triplets")
                    checkable: true
                    checked: midiEditor.triplets
                    onTriggered: {
                        midiEditor.triplets = checked;
                    }
                }
            }
        }
        MCtrl.Menu {
            title: qsTr("&Tools")

            MCtrl.Action {
                id: menuItemMetronome
                text: qsTr("Metronome")
                shortcut: "Ctrl+M"
                checkable: true
                checked: false
            }
            MCtrl.Action {
                id: actionEnableKeyboard
                text: qsTr("Map Keyboard")
                checkable: true
                checked: false
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("Tap Tempo...")
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
            title: qsTr("&Help")

            MCtrl.Action {
                text: qsTr("&Manual")
                shortcut: "F1"
            }
            MCtrl.Action {
                text: qsTr("&Quick Start")
            }
            MCtrl.MenuSeparator {}
            MCtrl.Action {
                text: qsTr("&Register Product")
            }
            MCtrl.Action {
                text: qsTr("User Support")
            }
        }
    }

    MainBar {
        id: mainBar
        onPlayStart: {
            EventBridge.playStart();
        }
        onPlayStop: {
            EventBridge.playStop();
        }
    }

    Item {
        id: mainFunctionArea
        anchors.top: mainBar.bottom
        anchors.bottom: statusBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        MCtrl.SplitView { orientation: Qt.Horizontal
            anchors.fill: parent
            Item {
                visible: assetVisible
                SplitView.preferredWidth: parent.width / 5
                SplitView.minimumHeight: 20
                FunctionArea {
                    id: assetsFunctionArea
                    title: qsTr("Assets")
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
                        onAddAssetDirectory: (directory) => {
                            EventBridge.addAssetDirectory(directory);
                        }
                        onRenameAssetDirectory: (id, name) => {
                            EventBridge.renameAssetDirectory(id, name);
                        }
                        onRemoveAssetDirectory: (id) => {
                            EventBridge.removeAssetDirectory(id);
                        }
                    }
                }
            }
            MCtrl.SplitView { orientation: Qt.Vertical
                anchors.top: parent.top
                anchors.right: parent.right
                MCtrl.SplitView {
                    orientation: Qt.Horizontal
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width
                    SplitView.preferredHeight: parent.height * 0.625
                    SplitView.minimumHeight: 20
                    Item {
                        SplitView.preferredWidth: parent.width * 0.6
                        FunctionArea {
                            id: arrangementFunctionArea
                            title: qsTr("Arrangement")
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
                                onAppendTrack: (track) => {
                                    eventBridge.appendTrack(track);
                                }
                                onInsertTrack: (track, index) => {
                                    eventBridge.insertTrack(track, index);
                                }
                            }
                        }
                    }
                    Item {
                        visible: midiEditorVisible
                        FunctionArea {
                            id: editorFunctionArea
                            property string clipName: ""
                            title: qsTr("Editor")
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
                Item {
                    SplitView.minimumHeight: 20
                    visible: mixerVisible
                    FunctionArea {
                        id: mixerFunctionArea
                        title: qsTr("Mixer")
                        Mixer {
                            id: mixer
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
            text: qsTr("Ready")
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
