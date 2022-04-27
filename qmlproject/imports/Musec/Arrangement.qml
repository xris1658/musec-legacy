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
    // Qt 6 中移除了 QtQuick.Dialogs 中的 ColorDialog 和 MessageDialog,
    // 此处使用 Qt.labs.platform 中的 ColorDialog 以保证向后兼容.
    // 讲个笑话: Types in Qt.labs modules are not guaranteed to remain
    // compatible in future versions. - Qt Documentation
    property Labs.ColorDialog colorDialog: Labs.ColorDialog {
        property TrackHeader colorDest1
        color: Constants.backgroundColor
        title: qsTr("选择颜色")
        modality: Qt.WindowModal
        onAccepted: {
            colorDest1.setColor(this.currentColor);
        }
    }
    property ListModel trackClipboard: ListModel {
        dynamicRoles: true
    }
    signal appendTrack(track: CompleteTrack)
    signal appendTrackComplete(index: int)

    MCtrl.Menu {
        id: trackOptions
        property int trackIndex
        property TrackHeader trackHeader
        title: qsTr("轨道操作")
        delegate: MCtrl.MenuItem {}
        width: 200
        height: contentHeight
        MCtrl.Action {
            text: qsTr("剪切(&T)")
            shortcut: "Ctrl+X"
        }
        MCtrl.Action {
            text: qsTr("复制(&C)")
            shortcut: "Ctrl+C"
        }
        MCtrl.Action {
            text: qsTr("创建副本(&L)")
            shortcut: "Ctrl+D"
        }
        MCtrl.Action {
            text: qsTr("粘贴(&P)")
            shortcut: "Ctrl+V"
        }
        MCtrl.Action {
            text: qsTr("删除(&D)")
            shortcut: "Delete"
            onTriggered: {
                trackOptions.trackHeader.remove();
            }
        }
        MCtrl.MenuSeparator {}
        MCtrl.Action {
            text: qsTr("重命名(&R)")
            shortcut: "F2"
            onTriggered: {
                trackOptions.trackHeader.rename();
            }
        }
        MCtrl.Action {
            text: qsTr("颜色(&L)")
            onTriggered: {
                colorDialog.open();
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
                        text: qsTr("对齐单位")
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
                        text: qsTr("自动化包络: ") + (automationButton.automationEnabled? qsTr("显示"): qsTr("隐藏"))
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
                    model: ["不显示", "上方", "下方"]
                    currentIndex: showMasterTrack
                    MCtrl.ToolTip {
                        visible: parent.hovered
                        text: qsTr("显示主轨道")
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
                    title: qsTr("编排视图操作")
                    delegate: MCtrl.MenuItem {}
                    width: 200
                    height: contentHeight
                    MCtrl.Action {
                        text: qsTr("添加乐器轨道(&I)")
                        onTriggered: {
                            let completeTrack = Qt.createQmlObject("import Musec.Entities 1.0; CompleteTrack {}",
                                root, null);
                            completeTrack.trackColor_ = Qt.rgba(Math.random(), Math.random(), Math.random(), 1);
                            completeTrack.trackName_ = qsTr("乐器");
                            completeTrack.trackType_ = CompleteTrack.InstrumentTrack;
                            completeTrack.height_ = 60;
                            appendTrack(completeTrack);
                        }
                    }
                    MCtrl.Action {
                        text: qsTr("添加音频轨道(&A)")
                        onTriggered: {
                            let completeTrack = Qt.createQmlObject("import Musec.Entities 1.0; CompleteTrack {}",
                                root, null);
                            completeTrack.trackColor_ = Qt.rgba(Math.random(), Math.random(), Math.random(), 1);
                            completeTrack.trackName_ = qsTr("音频");
                            completeTrack.trackType_ = CompleteTrack.AudioTrack;
                            completeTrack.height_ = 60;
                            appendTrack(completeTrack);
                        }
                    }
                    MCtrl.MenuSeparator {}
                    MCtrl.Action {
                        text: qsTr("粘贴(&P)")
                        enabled: trackClipboard && (trackClipboard.count != 0)
                    }
                }
                MouseArea {
                    acceptedButtons: Qt.RightButton
                    anchors.fill: parent
                    anchors.margins: 0
                    anchors.rightMargin: 6
                    onClicked: {
                        arrangementOptions.x = mouseX;
                        arrangementOptions.y = mouseY;
                        arrangementOptions.open();
                    }
                }
                TrackHeader {
                    id: masterTrack
                    trackName: qsTr("Master")
                    parent: showMasterTrackButton.currentIndex == 1? masterTrackHeader: masterTrackFooter
                    height: 60
                    Rectangle {
                        width: parent.width
                        height: 1
                        x: 0
                        y: showMasterTrackButton.currentIndex == 1? parent.height - 1: 0
                        color: Constants.borderColor
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
                        onEntered: {
                            if(!checkDragEvent(drag)) {
                                drag.accepted = false;
                            }
                        }
                        onDropped: {
                            if(checkDragEvent(drop)) {
                                console.log(drop.getDataAsString("pluginId"));
                            }
                        }
                    }
                }
                Item {
                    id: masterTrackHeader
                    width: parent.width
                    height: showMasterTrackButton.currentIndex == 1? masterTrackHeight: 0
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
                        onEntered: {
                            if(!checkDragEvent(drag)) {
                                drag.accepted = false;
                            }
                        }
                        onDropped: {
                            if(checkDragEvent(drop)) {
                                console.log("Create a new track with plugin: ");
                                console.log(drop.getDataAsString("type"), drop.getDataAsString("pluginId"));
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
                        function setColor(newColor: color) {
                            trackcolor = newColor;
                        }
                        function remove() {
                            console.log(model.index);
                            tracks.removeTrack(model.index);
                        }
                        onRenameComplete: {
                            trackname = newName;
                        }
                        onSetHeight: (newHeight) => {
                                         trackheight = newHeight;
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
                            id: trackHeaderMouseArea
                            width: parent.width - 3 * 20
                            height: 20
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: {
                                if(mouse.button == Qt.RightButton) {
                                    trackOptions.parent = trackHeader;
                                    trackOptions.trackIndex = trackHeader.trackIndex;
                                    trackOptions.trackHeader = trackHeader;
                                    colorDialog.colorDest1 = trackHeader;
                                    colorDialog.title = "选择颜色 - 轨道 " + trackHeader.trackIndex + ": " + trackHeader.trackName
                                    colorDialog.color = trackHeader.trackColor;
                                    trackOptions.x = mouseX;
                                    trackOptions.y = mouseY;
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
                            onEntered: {
                                if(!checkDragEvent(drag)) {
                                    drag.accepted = false;
                                }
                            }
                            onDropped: {
                                if(checkDragEvent(drop)) {
                                    console.log("Append a plugin to track: ");
                                    console.log(drop.getDataAsString("type"), drop.getDataAsString("pluginId"));
                                }
                            }
                        }
                    }
                }
                Item {
                    id: masterTrackFooter
                    width: parent.width
                    height: showMasterTrackButton.currentIndex == 2? masterTrackHeight: 0
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
                            onWheel: {
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
                        onWheel: {
                            // 纵向滚动
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
                            // 横向滚动
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
                            // 横向缩放
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
                            // 纵向缩放（有这种东西吗？）
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
                            title: qsTr("MIDI 轨道操作")
                            delegate: MCtrl.MenuItem {}
                            width: 200
                            height: contentHeight
                            MCtrl.Action {
                                text: qsTr("添加 MIDI 片段")
                            }
                        }
                        MCtrl.Menu {
                            id: audioTrackContentOptions
                            title: qsTr("音频轨道操作")
                            delegate: MCtrl.MenuItem {}
                            width: 200
                            height: contentHeight
                            MCtrl.Action {
                                text: qsTr("音频轨道操作")
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
                                        onClicked: {
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
                                    onEntered: {
                                        console.log(drag.keys);
                                        var type = drag.getDataAsString(itemType);
                                        if(type == "plugin") {
                                            drag.accepted = false;
                                        }
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
                                    onDropped: {
                                        console.log(drop.getDataAsString("FileName"));
                                        console.log(drop.x);
                                        trackDropIndicator.visible = false;
                                    }
                                    onPositionChanged: {
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
                                        text: qsTr("将插件或片段拖动到这里")
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
                                        onEntered: {
                                            if(drag.getDataAsString("FromWithin")) {
                                                console.log("This drag event is generated in the application.");
                                            }
                                            else {
                                                console.log("This drag event is generated out of the application.");
                                            }
                                        }
                                        onDropped: {
                                            console.log(drop.getDataAsString("FileName"));
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
