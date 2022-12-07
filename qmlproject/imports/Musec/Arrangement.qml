import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Window 2.15
import Qt.labs.platform 1.1 as Labs
import QtQml.Models 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl
import Musec.Models 1.0 as MModel
import Musec.Entities 1.0

Item {
    id: root
    clip: true
    enum ShowMasterTrack {
        Hide,
        Top,
        Bottom
    }
    property alias timelineNumerator: timeline.numerator
    property int showMasterTrack: Arrangement.ShowMasterTrack.Hide
    readonly property alias showMasterTrackModel: showMasterTrackButton.model
    property alias masterTrackHeight: masterTrack.height
    property bool loop: false
    property bool showAutomation: automationButton.automationEnabled
    property MModel.TrackListModel tracks
    property alias arrangementSnapUnit: editorSnapUnit
    property int barCount: 20
    property int position
    signal updateArrangement()
    // Qt 6 removed `ColorDialog` and `MessageDialog` in `QtQuick.Dialogs`.
    // To maintain forward compatibility, we use `ColorDialog` in `Qt.labs.platform`.
    // Fun fact: "Types in Qt.labs modules are not guaranteed to remain
    // compatible in future versions" according to Qt Documentation.
    property Labs.ColorDialog colorDialog: Labs.ColorDialog {
        property TrackHeader colorDest1
        color: Constants.backgroundColor
        title: qsTr("Select Color")
        modality: Qt.WindowModal
        onAccepted: {
            colorDest1.setColor(currentColor);
        }
    }
    property ListModel trackClipboard: ListModel {
        dynamicRoles: true
    }

    CompleteTrack {
        id: completeTrack
    }
    function generateInstrumentTrackInfo() {
        completeTrack.trackColor = Qt.rgba(Math.random(), Math.random(), Math.random(), 1);
        completeTrack.trackName = qsTr("Instrument");
        completeTrack.trackType = CompleteTrack.InstrumentTrack;
        completeTrack.height = 60;
    }

    function generateAudioTrackInfo() {
        completeTrack.trackColor = Qt.rgba(Math.random(), Math.random(), Math.random(), 1);
        completeTrack.trackName = qsTr("Audio");
        completeTrack.trackType = CompleteTrack.AudioTrack;
        completeTrack.height = 60;
    }

    MCtrl.Menu {
        id: trackOptions
        property int trackIndex
        property TrackHeader trackHeader
        title: qsTr("Track Options")
        delegate: MCtrl.MenuItem {}
        width: 200
        height: contentHeight
        MCtrl.Action {
            text: Constants.cutTextWithMnemonic
            shortcut: "Ctrl+X"
        }
        MCtrl.Action {
            text: Constants.copyTextWithMnemonic
            shortcut: "Ctrl+C"
        }
        MCtrl.Action {
            text: Constants.duplicateTextWithMnemonic
            shortcut: "Ctrl+D"
        }
        MCtrl.Action {
            text: Constants.pasteTextWithMnemonic
            shortcut: "Ctrl+V"
        }
        MCtrl.Action {
            text: Constants.deleteTextWithMnemonic
            shortcut: "Delete"
            onTriggered: {
                trackOptions.trackHeader.remove();
            }
        }
        MCtrl.MenuSeparator {}
        MCtrl.Action {
            text: Constants.renameTextWithMnemonic
            shortcut: "F2"
            onTriggered: {
                trackOptions.trackHeader.rename();
            }
        }
        MCtrl.Action {
            text: qsTr("Co&lor...")
            onTriggered: {
                colorDialog.open();
            }
        }
        MCtrl.MenuSeparator {}
        MCtrl.Action {
            text: qsTr("Insert &Instrument Track Above")
            onTriggered: {
                generateInstrumentTrackInfo();
                tracks.insertTrack(trackOptions.trackIndex - 1, completeTrack);
            }
        }
        MCtrl.Action {
            text: qsTr("Insert &Audio Track Above")
            onTriggered: {
                generateAudioTrackInfo();
                tracks.insertTrack(trackOptions.trackIndex - 1, completeTrack);
            }
        }
    }
    MCtrl.SplitView {
        id: headerContentSplitView
        anchors.fill: parent
        orientation: Qt.Horizontal
        handle: Item {
            implicitWidth: 6
            implicitHeight: 5
            Rectangle {
                anchors.right: parent.right
                width: 2
                height: parent.height
                color: Constants.gridColor
            }
        }
        Item {
            id: _left
            MCtrl.SplitView.minimumWidth: 150
            MCtrl.SplitView.maximumWidth: 500
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            Rectangle {
                id: lu
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width + 4
                height: 40
                z: 3
                color: Constants.backgroundColor
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: Constants.borderColor
                    z: 2
                }
                MCtrl.ComboBox {
                    id: editorSnapUnit
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: 60
                    height: parent.height / 2
                    font.family: Constants.font
                    model: ["1/64", "1/32", "1/16", "1/8", "1/4", "1/2", "1/1", "2", "4", "8", "16"]
                    currentIndex: 4
                    z: 1
                    MCtrl.ToolTip {
                        visible: parent.hovered
                        text: qsTr("Snap Unit")
                    }
                }
                MCtrl.Button {
                    id: automationButton
                    property bool automationEnabled: false
                    property color enabledForeColor: Constants.backgroundColor
                    property color disabledForeColor: Constants.contentColor1
                    property color enabledBackColor: Constants.contentColor1
                    property color disabledBackColor: Constants.backgroundColor
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    width: height
                    height: editorSnapUnit.height
                    background: Rectangle {
                        anchors.fill: parent
                        color: automationButton.automationEnabled? Constants.currentElementColor: Constants.backgroundColor
                    }
                    Item {
                        anchors.centerIn: parent
                        width: 12
                        height: 12
                        Rectangle {
                            anchors.left: parent.left
                            anchors.bottom: parent.bottom
                            width: 5
                            height: width
                            radius: width / 2
                            color: automationButton.automationEnabled? automationButton.enabledForeColor: automationButton.enabledBackColor
                        }
                        Rectangle {
                            anchors.right: parent.right
                            anchors.top: parent.top
                            width: 5
                            height: width
                            radius: width / 2
                            color: automationButton.automationEnabled? automationButton.enabledForeColor: automationButton.enabledBackColor
                        }
                        Shape {
                            width: parent.width
                            height: parent.height
                            antialiasing: true
                            smooth: true
                            ShapePath {
                                startX: 3
                                startY: 9
                                PathLine {
                                    x: 9
                                    y: 3
                                }
                                strokeWidth: 1.5
                                strokeColor: fillColor
                                fillColor: automationButton.automationEnabled? automationButton.enabledForeColor: automationButton.enabledBackColor
                            }
                        }
                    }
                    border.width: 0
                    z: 1
                    MCtrl.ToolTip {
                        visible: parent.hovered
                        text: qsTr("Automation") +": " + (automationButton.automationEnabled? qsTr("Show"): qsTr("Hide"))
                    }
                    onClicked:  {
                        automationEnabled = !automationEnabled;
                    }
                }
                MCtrl.ComboBox {
                    id: showMasterTrackButton
                    width: height * 3
                    height: editorSnapUnit.height
                    anchors.left: automationButton.right
                    anchors.top: automationButton.top
                    model: [qsTr("Hide"), qsTr("Show at Top"), qsTr("Show at Bottom")]
                    currentIndex: showMasterTrack
                    showToolTip: MCtrl.ComboBox.ShowToolTip.Never
                    MCtrl.ToolTip {
                        visible: parent.hovered
                        text: qsTr("Show Master Track") + ": " + parent.currentText
                    }
                }
            }
            Rectangle {
                id: headers
                anchors.top: lu.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width + 4
                color: Constants.backgroundColor
                clip: true
                MCtrl.Menu {
                    id: arrangementOptions
                    title: qsTr("Arrangement Options")
                    delegate: MCtrl.MenuItem {}
                    width: 200
                    height: contentHeight
                    MCtrl.Action {
                        id: insertInstrumentTrack
                        text: qsTr("Append an &Instrument Track")
                        onTriggered: {
                            generateInstrumentTrackInfo();
                            tracks.appendTrack(completeTrack);
                        }
                    }
                    MCtrl.Action {
                        id: insertAudioTrack
                        text: qsTr("Append an &Audio Track")
                        onTriggered: {
                            generateAudioTrackInfo();
                            tracks.appendTrack(completeTrack);
                        }
                    }
                    MCtrl.MenuSeparator {}
                    MCtrl.Action {
                        text: Constants.pasteTextWithMnemonic
                        enabled: trackClipboard && (trackClipboard.count != 0)
                    }
                }
                MouseArea {
                    acceptedButtons: Qt.RightButton
                    anchors.fill: parent
                    anchors.margins: 0
                    anchors.rightMargin: 6
                    onClicked: (mouse) => {
                        arrangementOptions.x = mouse.x;
                        arrangementOptions.y = mouse.y;
                        arrangementOptions.open();
                    }
                }
                TrackHeader {
                    id: masterTrack
                    trackName: qsTr("Master")
                    parent: showMasterTrackButton.currentIndex == 1? masterTrackHeader: masterTrackFooter
                    width: headers.width
                    height: 60
                    Rectangle {
                        width: parent.width
                        height: 1
                        x: 0
                        y: showMasterTrackButton.currentIndex == 1? parent.height - 1: 0
                        color: Constants.borderColor
                    }
                    Item {
                        visible: root.showAutomation
                        anchors.top: parent.top
                        anchors.topMargin: masterTrack.lineHeight
                        anchors.left: parent.left
                        anchors.leftMargin: masterTrack.colorBarWidth
                        anchors.right: parent.right
                        height: 20
                        MCtrl.Button {
                            anchors.fill: parent
                            anchors.margins: 2
                        }
                    }
                    DropArea {
                        id: masterTrackDropArea
                        anchors.fill: parent
                        Rectangle {
                            anchors.fill: parent
                            visible: parent.containsDrag
                            color: Constants.mouseOverElementColor
                            opacity: 0.5
                        }
                        function checkDragEvent(dragEvent: DragEvent) {
                            return dragEvent.getDataAsString("itemType") == "plugin"
                                    && parseInt(dragEvent.getDataAsString("type")) == 3;
                        }
                        onEntered: (drag) => {
                            if(!checkDragEvent(drag)) {
                                drag.accepted = false;
                            }
                        }
                        onDropped: (drop) => {
                            if(checkDragEvent(drop)) {
                                console.log(drop.getDataAsString("pluginId"));
                            }
                        }
                    }
                }
                Item {
                    id: masterTrackHeader
                    width: parent.width
                    height: visible? masterTrackHeight: 0
                    visible: showMasterTrackButton.currentIndex == 1
                    anchors.top: parent.top
                    z: 2
                }
                ListView {
                    id: trackHeaderList
                    property int footerHeight: 50
                    orientation: Qt.Vertical
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: masterTrackHeader.bottom
                    anchors.bottom: masterTrackFooter.top
                    model: root.tracks
                    interactive: false
                    property int minimumBlankHeight: 50
                    z: 1
                    clip: true
                    ScrollBar.vertical: ScrollBar {
                        id: vbar
                        parent: vbarPlaceholder
                        width: parent.width
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        background: Rectangle {
                            color: Constants.backgroundColor
                        }
                        contentItem: Rectangle {
                            radius: 2
                            color: Constants.mouseOverElementColor
                            opacity: trackHeaderList.height < trackHeaderList.contentHeight? 1: 0
                        }
                    }
                    footer: Item {
                        id: trackHeaderListFooter
                        width: headers.width
                        height: trackHeaderList.footerHeight
                    }
                    DropArea {
                        id: blankHeaderDropArea
                        anchors.left: parent.left
                        anchors.right: parent.right
                        y: parent.count?
                               parent.itemAtIndex(parent.count - 1).mapToItem(parent, 0, parent.itemAtIndex(parent.count - 1).height).y:
                               0
                        height: masterTrackFooter.y - masterTrackHeader.height
                        function checkDragEvent(dragEvent: DragEvent) {
                            return dragEvent.getDataAsString("itemType") == "plugin"
                                    && (parseInt(dragEvent.getDataAsString("type")) == 3
                                    || parseInt(dragEvent.getDataAsString("type")) == 2);
                        }
                        Rectangle {
                            anchors.fill: parent
                            color: Constants.mouseOverElementColor
                            opacity: parent.containsDrag? 0.6: 0
                        }
                        onEntered: (drag) => {
                            if(!checkDragEvent(drag)) {
                                drag.accepted = false;
                            }
                        }
                        onDropped: (drop) => {
                            if(checkDragEvent(drop)) {
                                var type = parseInt(drop.getDataAsString("type"));
                                if(type == Plugin.Instrument) {
                                    generateInstrumentTrackInfo();
                                    tracks.appendTrack(completeTrack);
                                    tracks.loadInstrument(tracks.trackCount() - 1, parseInt(drop.getDataAsString("pluginId")));
                                }
                                else if(type == Plugin.AudioEffect) {
                                    generateAudioTrackInfo();
                                    tracks.appendTrack(completeTrack);
                                    tracks.insertEffect(tracks.trackCount() - 1, 0, parseInt(drop.getDataAsString("pluginId")));
                                }
                                console.log("Create a new track with plugin: ");
                                console.log(drop.getDataAsString("type"), drop.getDataAsString("pluginId"));
                            }
                            else {
                                var filePath = drop.getDataAsString("FileName");
                                var extension = filePath.slice(filePath.lastIndexOf('.') + 1).toLowerCase();
                                if(extension == "mid" || extension == "midi") {
                                    insertInstrumentTrack.trigger(blankHeaderDropArea);
                                }
                                else {
                                    insertAudioTrack.trigger(blankHeaderDropArea);
                                }
                            }
                        }
                    }
                    delegate: TrackHeader {
                        id: trackHeader
                        z: 3
                        height: trackheight
                        trackName: trackname
                        trackColor: trackcolor
                        width: headers.width
                        trackIndex: model.index + 1
                        trackSelected: false
                        trackMute: mute
                        trackSolo: solo
                        trackRecord: armRecording
                        Item {
                            visible: root.showAutomation
                            anchors.top: parent.top
                            anchors.topMargin: masterTrack.lineHeight
                            anchors.left: parent.left
                            anchors.leftMargin: masterTrack.colorBarWidth
                            anchors.right: parent.right
                            height: 20
                            MCtrl.Button {
                                anchors.fill: parent
                                anchors.margins: 2
                            }
                        }
                        function setColor(newColor: color) {
                            trackcolor = newColor;
                        }
                        function remove() {
                            console.log(model.index);
                            tracks.removeTrack(model.index);
                        }
                        onRenameComplete: (newName) => {
                            trackname = newName;
                        }
                        onSetMute: (newMute) => {
                                       mute = newMute;
                                   }
                        onSetSolo: (newSolo) => {
                                       solo = newSolo;
                                   }
                        onSetArmRecording: (newArmRecording) => {
                                               armRecording = newArmRecording;
                                           }
                        Rectangle {
                            width: parent.width
                            height: 1
                            x: 0
                            y: parent.height - 1
                            color: Constants.borderColor
                        }
                        MouseArea {
                            id: resizeArea
                            z: 2
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            width: parent.width
                            height: 5
                            property int initialY: 0
                            Rectangle {
                                id: resizeAreaRect
                                anchors.fill: parent
                                visible: resizeArea.containsMouse
                                gradient: Gradient {
                                    orientation: Qt.Vertical
                                    GradientStop {
                                        position: 0
                                        color: "transparent"
                                    }
                                    GradientStop {
                                        position: 1
                                        color: Constants.borderColor
                                    }
                                }
                            }
                            hoverEnabled: true
                            cursorShape: containsMouse? Qt.SizeVerCursor: Qt.ArrowCursor
                            onPressed: {
                                initialY = mouseY;
                            }
                            onMouseYChanged: {
                                if(pressed) {
                                    var rootY = mapToItem(trackHeader, mouseX, mouseY).y;
                                    if(rootY >= lineHeight + 1) {
                                        var newHeight = trackHeader.height + mouseY - initialY;
                                        if(newHeight < lineHeight + 1) {
                                            newHeight = lineHeight + 1;
                                        }
                                        trackheight = newHeight;
                                    }
                                    else {
                                        trackheight = lineHeight + 1;
                                    }
                                    initialY = mouseY;
                                }
                            }
                        }
                        MouseArea {
                            id: trackHeaderMouseArea
                            width: parent.width - 3 * 20
                            height: 20
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: (mouse) => {
                                if(mouse.button == Qt.RightButton) {
                                    trackOptions.parent = trackHeader;
                                    trackOptions.trackIndex = trackHeader.trackIndex;
                                    trackOptions.trackHeader = trackHeader;
                                    colorDialog.colorDest1 = trackHeader;
                                    colorDialog.title = qsTr("Select Color") + " - " + qsTr("Track") + trackHeader.trackIndex + ": " + trackHeader.trackName
                                    colorDialog.color = trackHeader.trackColor;
                                    trackOptions.x = mouse.x;
                                    trackOptions.y = mouse.y;
                                    trackOptions.open();
                                }
                            }
                        }
                        DropArea {
                            id: trackHeaderDropArea
                            anchors.fill: parent
                            function checkDragEvent(dragEvent: DragEvent) {
                                return dragEvent.getDataAsString("itemType") == "plugin"
                                        && (parseInt(dragEvent.getDataAsString("type")) == 3
                                        || parseInt(dragEvent.getDataAsString("type")) == 2);
                            }
                            Rectangle {
                                anchors.fill: parent
                                color: Constants.mouseOverElementColor
                                opacity: parent.containsDrag? 0.6: 0
                            }
                            onEntered: (drag) => {
                                if(checkDragEvent(drag)) {
                                    if(type == CompleteTrack.AudioTrack
                                       && (parseInt(drag.getDataAsString("type")) != Plugin.AudioEffect)) {
                                        drag.accepted = false;
                                    }
                                    return;
                                }
                                drag.accepted = false;
                            }
                            onDropped: (drop) => {
                                let pluginType = parseInt(drop.getDataAsString("type"));
                                let pluginId = parseInt(drop.getDataAsString("pluginId"));
                                if(type == CompleteTrack.InstrumentTrack) {
                                    if(pluginType == Plugin.Instrument) {
                                        tracks.loadInstrument(index, pluginId);
                                    }
                                    else if(pluginType == Plugin.AudioEffect) {
                                        tracks.insertEffect(index, plugin_list.itemCount(), pluginId);
                                    }
                                }
                                else if(type == CompleteTrack.AudioTrack) {
                                    if(pluginType == Plugin.AudioEffect) {
                                        tracks.insertEffect(index, plugin_list.itemCount(), pluginId);
                                    }
                                }
                            }
                        }
                    }
                }
                Item {
                    id: masterTrackFooter
                    width: parent.width
                    height: visible? masterTrackHeight: 0
                    visible: showMasterTrackButton.currentIndex == 2
                    anchors.bottom: parent.bottom
                    z: 2
                }
            }
        }
        Item {
            id: contentAndVBar
            Item {
                id: _right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 15
                Rectangle {
                    id: ru
                    anchors.top: parent.top
                    anchors.left: parent.left
                    width: parent.width
                    height: lu.height
                    z: 3
                    color: Constants.backgroundColor
                }
                Rectangle {
                    id: vbarPlaceholder
                    width: 15
                    anchors.top: ru.bottom
                    anchors.topMargin: masterTrackHeader.height
                    anchors.right: parent.right
                    height: headers.height - anchors.topMargin - masterTrackFooter.height
                    color: Constants.mouseOverElementColor
                }
            }
            Rectangle {
                id: _medium
//                anchors.left: _left.right
                width: parent.width - _right.width
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: Constants.backgroundColor2
                Item {
                    id: timelineAndHbar
                    anchors.top: parent.top
                    height: lu.height
                    anchors.left: parent.left
                    width: parent.width
                    z: 1
                    clip: true
                    ScrollBar {
                        id: hbar
                        anchors.left: parent.left
                        width: parent.width
                        anchors.top: parent.top
                        height: 15
                        background: Rectangle {
                            anchors.fill: parent
                            color: timeline.color
                        }
                        contentItem: Rectangle {
                            radius: 2
                            color: Constants.mouseOverElementColor
                        }
                        size: parent.width / timeline.width
                        orientation: Qt.Horizontal
                        active: hovered || pressed
                        position: 0.0
                    }
                    Timeline {
                        id: timeline
                        barCount: root.barCount
                        anchors.bottom: parent.bottom
                        height: parent.height - hbar.height
                        x: -hbar.position * width
                        Rectangle {
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            width: parent.width
                            height: 1
                            color: Constants.borderColor
                        }
                        MouseArea {
                            anchors.fill: parent
                            onWheel: (wheel) => {
                                hbar.position -= 0.1 * wheel.angleDelta.y / 540.0 * (wheel.inverted? -1: 1);
                                if(hbar.position > 1 - hbar.size)
                                {
                                    hbar.position = 1 - hbar.size;
                                }
                                else if(hbar.position < 0)
                                {
                                    hbar.position = 0;
                                }
                            }
                        }
                    }
                }
                Item {
                    id: contentAreaRect
                    anchors.left: parent.left
                    width: parent.width
                    y: headers.y
                    height: root.height - timelineAndHbar.height
                    clip: true
                    Rectangle {
                        id: timeIndicator
                        width: 1
                        height: parent.height
                        x: root.position / 96.0 * (timeline.barWidth / timeline.numerator) + contentArea.x
                        y: 0
                        z: 3
                    }
                    MouseArea {
                        id: contentAreaMouseArea
                        anchors.fill: parent
                        scrollGestureEnabled: true
                        onWheel: (wheel) => {
                            // Scroll vertically
                            if(wheel.modifiers == Qt.NoModifier)
                            {
                                vbar.position += 0.1 * wheel.angleDelta.y / 360.0 * (wheel.inverted? 1: -1);
                                if(vbar.position > 1 - vbar.size)
                                {
                                    vbar.position = 1 - vbar.size;
                                }
                                else if(vbar.position < 0)
                                {
                                    vbar.position = 0;
                                }
                                hbar.position += 0.1 * wheel.angleDelta.x / 540.0 * (wheel.inverted? 1: -1);
                                if(hbar.position > 1 - hbar.size)
                                {
                                    hbar.position = 1 - hbar.size;
                                }
                                else if(hbar.position < 0)
                                {
                                    hbar.position = 0;
                                }
                            }
                            // Scroll horizontally
                            else if(wheel.modifiers == Qt.ShiftModifier)
                            {
                                hbar.position += 0.1 * wheel.angleDelta.y / 540.0 * (wheel.inverted? 1: -1);
                                if(hbar.position > 1 - hbar.size)
                                {
                                    hbar.position = 1 - hbar.size;
                                }
                                else if(hbar.position < 0)
                                {
                                    hbar.position = 0;
                                }
                                vbar.position += 0.1 * wheel.angleDelta.x / 360.0 * (wheel.inverted? 1: -1);
                                if(vbar.position > 1 - vbar.size)
                                {
                                    vbar.position = 1 - vbar.size;
                                }
                                else if(vbar.position < 0)
                                {
                                    vbar.position = 0;
                                }
                            }
                            // Zoom horizontally
                            else if(wheel.modifiers == Qt.ControlModifier) {
                                let constant = 1250.0;
                                let multiplier = (wheel.angleDelta.y/* * (wheel.inverted? 1: -1)*/ + constant) / constant;
                                timeline.barWidth *= multiplier;
                                if(timeline.barWidth * timeline.numerator > 500) {
                                    timeline.barWidth = 500 / timeline.numerator;
                                }
                                if(timeline.width < hbar.width) {
//                                    timeline.barWidth = Math.ceil(hbar.width / timeline.barCount);
                                    timeline.barWidth = hbar.width / timeline.barCount;
                                }
                                if(timeline.x + timeline.width < hbar.width) {
                                    hbar.position = 1.0 - hbar.width / timeline.width;
                                }
                            }
                            // Zoom vertically
                        }
                    }
                    Rectangle {
                        id: contentAreaMaster
                        z: 2
                        width: timeline.width
                        x: timeline.x
                        y: showMasterTrackButton.currentIndex == 1?
                            0:
                            masterTrackFooter.y
                        height: masterTrack.height
                        color: Constants.backgroundColor2
                        Item {
                            anchors.fill: parent
                            Rectangle {
                                y: showMasterTrackButton.currentIndex == 1?
                                    parent.height - 1: 0
                                anchors.left: parent.left
                                width: parent.width
                                height: 1
                                color: Constants.gridColor
                            }
                        }
                        ListView {
                            model: timeline.barCount * timeline.numerator
                            orientation: Qt.Horizontal
                            interactive: false
                            anchors.fill: parent
                            delegate: Item {
                                width: timeline.barWidth / timeline.numerator
                                height: masterTrack.height
                                Rectangle {
                                    anchors.left: parent.left
                                    anchors.top: parent.top
                                    width: 1
                                    height: parent.height
                                    color: Constants.gridColor
                                }
                            }
                        }
                    }
                    Rectangle {
                        id: contentArea
                        z: 1
                        width: timeline.width
                        height: contentAreaRect.height > trackHeaderList.contentHeight? contentAreaRect.height: trackHeaderList.contentHeight
                        x: timeline.x
                        y: -(trackHeaderList.contentY) + masterTrackHeader.height
                        color: Constants.backgroundColor2
                        clip: true
                        property int menuAtTrackIndex
                        MCtrl.Menu {
                            id: midiTrackContentOptions
                            title: qsTr("MIDI Track Options")
                            delegate: MCtrl.MenuItem {}
                            width: 200
                            height: contentHeight
                            MCtrl.Action {
                                text: qsTr("Add MIDI Clip")
                            }
                        }
                        MCtrl.Menu {
                            id: audioTrackContentOptions
                            title: qsTr("Audio Track Options")
                            delegate: MCtrl.MenuItem {}
                            width: 200
                            height: contentHeight
                            MCtrl.Action {
                                text: qsTr("Audio Track Options")
                                enabled: false
                            }
                        }
                        ListView {
                            id: trackContentListView
                            z: 2
                            orientation: Qt.Vertical
                            model: tracks
                            interactive: false
                            anchors.fill: parent
                            delegate: Item {
                                id: trackContentListDelegate
                                width: contentArea.width
                                height: trackheight
                                Rectangle {
                                    anchors.bottom: parent.bottom
                                    anchors.left: parent.left
                                    width: parent.width
                                    height: 1
                                    color: Constants.gridColor
                                }
                                DropArea {
                                    id: trackDropArea
                                    anchors.fill: parent
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: false
                                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                                        onClicked: (mouse) => {
                                            if(mouse.button == Qt.RightButton) {
                                                contentArea.menuAtTrackIndex = index;
                                                var pointOfTrackDropArea = trackDropArea.mapToItem(contentArea, mouse.x, mouse.y);
                                                if(type != CompleteTrack.AudioTrack) {
                                                    midiTrackContentOptions.x = pointOfTrackDropArea.x;
                                                    midiTrackContentOptions.y = pointOfTrackDropArea.y;
                                                    midiTrackContentOptions.open();
                                                }
                                                else {
                                                    audioTrackContentOptions.x = pointOfTrackDropArea.x;
                                                    audioTrackContentOptions.y = pointOfTrackDropArea.y;
                                                    audioTrackContentOptions.open();
                                                }
                                            }
                                        }
                                    }
                                    Rectangle {
                                        id: trackDropIndicator
                                        x: 0
                                        width: 10
                                        height: parent.height - 1
                                        visible: false
                                        gradient: Gradient {
                                            orientation: Qt.Horizontal
                                            GradientStop {
                                                position: 0
                                                color: Constants.mouseOverElementColor
                                            }
                                            GradientStop {
                                                position: 1
                                                color: "transparent"
                                            }
                                        }
                                        color: Constants.mouseOverElementColor
                                        opacity: parent.containsDrag? 0.6: 0
                                    }
                                    onEntered: (drag) => {
                                        var itemType = drag.getDataAsString("itemType");
                                        if(itemType == "plugin") {
                                            console.log("type:", type);
                                            var pluginType = parseInt(drag.getDataAsString("type"));
                                            if(type == CompleteTrack.InstrumentTrack) {
                                                if(pluginType == 2) {
                                                    console.log("Apply instrument to an instrument track");
                                                }
                                                else if(pluginType == 3) {
                                                    console.log("Append an audio effect to an instrument track");
                                                }
                                            }
                                            else if(type == CompleteTrack.AudioTrack) {
                                                if(pluginType == 2) {
                                                    console.log("Apply instrument to an audio track (not allowed)");
                                                    drag.accepted = false;
                                                    return;
                                                }
                                                else if(pluginType == 3) {
                                                    console.log("Append an audio effect to an audio track");
                                                }
                                            }
                                        }
                                        else {
                                            var filePath = drag.getDataAsString("FileName");
                                            var extension = filePath.slice(filePath.lastIndexOf('.')).toLowerCase();
                                            if(type != CompleteTrack.AudioTrack) {
                                                if(extension != ".mid" && extension != ".midi") {
                                                    drag.accepted = false;
                                                    return;
                                                }
                                            }
                                            trackDropIndicator.visible = true;
                                        }
                                    }
                                    onDropped: (drop) => {
                                        var itemType = drop.getDataAsString("itemType");
                                        if(itemType == "plugin") {
                                            console.log("type:", type);
                                            var pluginType = drop.getDataAsString("type");
                                            if(type == CompleteTrack.InstrumentTrack) {
                                                if(pluginType == 2) {
                                                    console.log("Apply an instrument to an instrument track");
                                                }
                                                else if(pluginType == 3) {
                                                    console.log("Append and audio effect to an audio track");
                                                }
                                            }
                                            else if(type == CompleteTrack.AudioTrack) {
                                                if(pluginType == 2) {
                                                    console.log("Apply instrument to an audio track (not allowed)");
                                                }
                                                else if(pluginType == 3) {
                                                    console.log("Append an audio effect to an audio track");
                                                }
                                            }
                                        }
                                        else {
                                            console.log(drop.getDataAsString("FileName"));
                                            console.log(drop.x);
                                            trackDropIndicator.visible = false;
                                        }
                                    }
                                    onPositionChanged: (drag) => {
                                        trackDropIndicator.x = drag.x;
                                    }
                                }
                            }
                            footer: Item {
                                width: contentArea.width
                                height: trackHeaderList.height > trackHeaderList.contentHeight?
                                        trackHeaderList.height - trackHeaderList.contentHeight + trackHeaderList.footerHeight:
                                        trackHeaderList.footerHeight
                                Item {
                                    x: contentArea.x * -1
                                    width: contentAreaRect.width
                                    height: parent.height
                                    Text {
                                        text: qsTr("Drag & Drop plugins or audio clips here")
                                        anchors.centerIn: parent
                                        font: Constants.font
                                        color: Constants.contentColor2
                                    }
                                    DropArea {
                                        id: contentDropArea
                                        anchors.fill: parent
                                        Rectangle {
                                            anchors.fill: parent
                                            color: Constants.mouseOverElementColor
                                            opacity: parent.containsDrag? 0.6: 0
                                        }
                                        onEntered: (drag) => {
                                        }
                                        onDropped: (drop) => {
                                            if(drop.getDataAsString("itemType") == "plugin") {
                                                blankHeaderDropArea.dropped(drop);
                                            }
                                            else {
                                                console.log(drop.getDataAsString("FileName"));
                                                console.log(drop.x);
                                                blankHeaderDropArea.dropped(drop);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        ListView {
                            id: trackGrid
                            z: 1
                            model: timeline.barCount * timeline.numerator
                            orientation: Qt.Horizontal
                            height: contentAreaRect.height
                            interactive: false
                            anchors.fill: parent
                            delegate: Item {
                                width: timeline.barWidth / timeline.numerator
                                height: contentAreaRect.height > trackHeaderList.contentHeight? contentAreaRect.height: trackHeaderList.contentHeight
                                Rectangle {
                                    anchors.left: parent.left
                                    anchors.top: parent.top
                                    width: 1
                                    height: parent.height
                                    color: Constants.gridColor
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
