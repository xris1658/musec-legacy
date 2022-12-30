import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl
import Musec.Models 1.0 as MModel
import Musec.Shapes 1.0

Window {
    id: root
    flags: Qt.Dialog
    modality: Qt.NonModal
    property bool destroyingPlugin: false
    onClosing: (close) => {
        if(!root.destroyingPlugin) {
            close.accepted = false;
            root.hide();
        }
    }
    width: 400
    height: 400
    color: Constants.backgroundColor
    property MModel.PluginParameterListModel parameterListModel: MModel.PluginParameterListModel {}
    property string pluginName: "Plugin name here"
    Column {
        Item {
            id: titleAndParamCount
            width: root.width
            height: 30
            Text {
                width: root.width - paramCountText.width
                anchors.left: parent.left
                anchors.leftMargin: (parent.height - contentHeight) / 2
                anchors.verticalCenter: parent.verticalCenter
                text: root.pluginName
                font.family: Constants.font
                color: Constants.contentColor1
                font.pixelSize: parent.height * 0.6
                elide: Text.ElideRight
            }
            Text {
                property int parameterCount: parameterListModel.parameterCount()? parameterListModel.parameterCount(): 0
                id: paramCountText
                anchors.right: parent.right
                anchors.rightMargin: (parent.height - contentHeight) / 2
                anchors.verticalCenter: parent.verticalCenter
                text: parameterCount == 0? qsTr("No parameters"):
                      parameterCount == 1? qsTr("%1 parameter").arg(parameterCount):
                                           qsTr("%1 parameters").arg(parameterCount)
                font.family: Constants.font
                color: Constants.contentColor1
                font.pixelSize: parent.height * 0.6
            }
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: Constants.borderColor
            }
        }
        Item {
            width: root.width
            height: root.height - titleAndParamCount.height
            clip: true
            ListView {
                id: parameterListView
                anchors.fill: parent
                interactive: true
                boundsBehavior: Flickable.StopAtBounds
                model: root.parameterListModel
                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                    width: 15
                    visible: parameterListView.contentHeight > parameterListView.height
                    background: Rectangle {
                        anchors.fill: parent
                        color: Constants.backgroundColor
                    }
                    contentItem: Rectangle {
                        radius: 2
                        color: scrollBar.hovered? Qt.darker(Constants.currentElementColor): Constants.mouseOverElementColor
                    }
                }
                delegate: Rectangle {
                    id: delegate
                    width: root.width
                    height: 20
                    visible: !hidden
                    color: index % 2 == 0? Constants.backgroundColor: Constants.backgroundColor2
                    Item {
                        id: left_
                        width: parent.width * 0.75
                        height: parent.height
                        anchors.left: parent.left
                        onWidthChanged: {
                            paramText.getDisplayText();
                        }
                        Item {
                            id: automationIndicator
                            width: height
                            height: parent.height
                            AutomationIcon {
                                anchors.centerIn: parent
                                visible: automatable
                                fillColor: Constants.mouseOverElementColor
                            }
                        }
                        Text {
                            id: paramText
                            anchors.left: automationIndicator.right
                            function getDisplayText() {
                                text = name;
                                if(contentWidth > parent.width) {
                                    text = shortName;
                                }
                            }
                            Component.onCompleted: {
                                getDisplayText();
                            }
                            font.family: Constants.font
                            color: Constants.contentColor1
                        }
                    }
                    Item {
                        id: right_
                        width: delegate.width - left_.width
                        anchors.right: parent.right
                        anchors.rightMargin: scrollBar.visible? scrollBar.width: 0
                        height: parent.height
                        MCtrl.Slider {
                            id: slider
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: (!showAsList) && (!showAsSwitch)
                            snapMode: discrete? Slider.SnapAlways: Slider.NoSnap
                            stepSize: discrete? 1.0 / step: 0.0
                            handle.width: 10
                            handle.height: 16
                            leftPadding: 0
                            rightPadding: 0
                            value: value
                            from: minValue
                            to: maxValue
                        }
                        MCtrl.Switch {
                            visible: showAsSwitch
                            anchors.right: parent.right
                            anchors.rightMargin: 2
                            width: height * 1.5
                            height: parent.height
                            checked: value
                        }
                        MCtrl.ComboBox {
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: showAsList
                            model: visible? list: step
                            textRole: "text"
                            valueRole: "value"
                        }
                        Item {
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: false
                            MCtrl.Switch {
                                anchors.right: parent.right
                                height: parent.height
                                width: height * 2
                            }
                        }
                    }
                }
            }
        }
    }
}
