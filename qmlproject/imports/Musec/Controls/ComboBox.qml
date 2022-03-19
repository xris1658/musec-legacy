import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Shapes 1.15
import QtQml.Models 2.15

import Musec 1.0
import Musec.Controls 1.0

QQC2.ComboBox {
    enum ShowToolTip {
        Never,
        Needed,
        Always
    }
    property int showToolTip: ComboBox.ShowToolTip.Needed
    property string toolTipText: currentText
    id: comboBox
    opacity: enabled? 1.0: 0.5
    property alias color: _background.color
    property alias border: _background.border
    font.family: Constants.font
    height: 20
    property int popupWidth: width
    ToolTip {
        text: currentText
        visible: showToolTip === ComboBox.ShowToolTip.Always? hovered:
                 showToolTip === ComboBox.ShowToolTip.Needed? (popupWidth < width) && hovered:
                 false
    }
    delegate: QQC2.ItemDelegate {
        id: comboBoxDelegate
        width: comboBox.popup.width
        height: 20
        background: Rectangle { // 实现高亮
            width: parent.width
            height: parent.height
            color: comboBox.highlightedIndex === index? Constants.menuHighlightBackgroundColor : Constants.menuBackgroundColor
            Item {
                id: currentSelectionIndicator
                anchors.right: parent.right
                anchors.top: parent.top
                width: parent.height
                height: parent.height
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width / 3
                    height: parent.width / 3
                    radius: width / 2
                    color: comboBox.highlightedIndex === index? Constants.menuHighlightContentColor: Constants.menuContentColor
                    opacity: comboBox.currentIndex === index? 1: 0
                }
            }
        }
        contentItem: Rectangle {
            id: content
//            width: parent.width
            implicitWidth: comboBox.width
            width: popupWidth == comboBox.width? comboBox.width: popupWidth + parent.height
            height: parent.height
            Text {
                id: contentText
                width: parent.width - parent.height
                anchors.left: parent.left
                anchors.leftMargin: -5
                anchors.verticalCenter: content.verticalCenter
                anchors.verticalCenterOffset: -2
                text: comboBox.textRole ? (Array.isArray(comboBox.model) ? modelData[comboBox.textRole] : model[comboBox.textRole]) : modelData
                color: comboBox.highlightedIndex === index? Constants.menuHighlightContentColor: Constants.menuContentColor
                font: comboBox.font
//                elide: Text.ElideRight
            }
            Component.onCompleted: {
                if(contentText.contentWidth > comboBox.popupWidth) {
                    comboBox.popupWidth = contentText.contentWidth + 5;
                }
            }
        }
    }
    indicator: Item {
        anchors.right: parent.right
        width: parent.height
        height: width
        Text {
            anchors.centerIn: parent
            text: qsTr("\u25BC")
            font.pixelSize: parent.height * 0.4
            color: Constants.contentColor1
        }
    }
    contentItem: Text {
        id: contentItemAsText
        anchors.left: parent.left
        anchors.leftMargin: 5
        text: parent.displayText
        font: parent.font
        color: Constants.contentColor1
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    // 控件背景
    background: Rectangle {
        id: _background
        anchors.fill: parent
        color: Constants.backgroundColor
        border.color: Constants.borderColor
        border.width: comboBox.activeFocus? 2: 1
    }
    popup: Menu {
        width: popupWidth == comboBox.width? comboBox.width: popupWidth + parent.height
        y: parent.height
        implicitWidth: comboBox.width
        implicitHeight: contentItem.implicitHeight + 2
        padding: 1
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: comboBox.popup.visible?
                       comboBox.delegateModel : null
            currentIndex: comboBox.currentIndex
            focus: true
            interactive: false
        }
    }
}
