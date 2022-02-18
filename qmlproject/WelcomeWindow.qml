import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    flags: Qt.Dialog
    id: root
    title: qsTr("Musec")
    color: Constants.backgroundColor
    visibility: Qt.WindowModal
    visible: true
    width: 600
    height: 400
    Column {
        StackLayout {
            id: stackLayout
            width: root.width
            height: root.height - pageIndicatorRect.height - buttons.height
            Rectangle {
                anchors.fill: parent
                anchors.margins: 20
                color: Constants.backgroundColor
                Column {
                    spacing: 20
                    Text {
                        text: qsTr("欢迎使用 Musec")
                        font.family: "Noto Sans CJK SC Bold"
                        color: Constants.contentColor1
                        font.pixelSize: 20
                    }
                    Text {
                        text: qsTr("点击“下一步”可以开始进行初始设置。")
                        font: Constants.font
                        color: Constants.contentColor1
                    }
                   Column {
                       spacing: 5
                       Text {
                           text: qsTr("如果您曾经导出了软件设置，可以进行导入。")
                           font: Constants.font
                           color: Constants.contentColor1
                       }
                       MCtrl.Button {
                           text: qsTr("导入设置文件(&S)...")
                       }
                   }
                }
            }
            Rectangle {
                anchors.fill: parent
                anchors.margins: 20
                color: Constants.backgroundColor
                Column {
                    spacing: 20
                    Text {
                        text: qsTr("设置外观")
                        font.family: "Noto Sans CJK SC Bold"
                        color: Constants.contentColor1
                        font.pixelSize: 20
                    }
                    Grid {
                        columns: 2
                        spacing: 10
                        Text {
                            width: 100
                            text: qsTr("语言 / Language")
                            font.family: Constants.font
                            color: Constants.contentColor2
                            horizontalAlignment: Text.AlignRight
                            wrapMode: Text.Wrap
                        }
                        Row {
                            spacing: -1
                            Repeater {
                                model: ["简体中文", "正體中文", "English"]
                                MCtrl.Button {
//                                    width: 100
                                    text: modelData
                                }
                            }
                        }
                        Text {
                            width: 100
                            text: qsTr("配色方案")
                            font.family: Constants.font
                            color: Constants.contentColor2
                            horizontalAlignment: Text.AlignRight
                        }
                        Row {
                            spacing: -1
                            Repeater {
                                model: ["亮色", "暗色", "高对比度"]
                                MCtrl.Button {
                                    width: 100
                                    height: 80
                                    text: modelData
                                }
                            }
                        }
                    }
                }
            }
        }
        Item {
            id: pageIndicatorRect
            width: root.width
            height: 30
            PageIndicator {
                id: pageIndicator
                count: stackLayout.count
                currentIndex: stackLayout.currentIndex
                anchors.centerIn: parent
                height: parent.height
                interactive: false
                delegate: Rectangle {
                    implicitWidth: 8
                    implicitHeight: 8
                    radius: width / 2
                    color: Constants.contentColor2
                    opacity: index == pageIndicator.currentIndex? 1: 0.5
                }
            }
        }

        Rectangle {
            id: buttons
            color: Constants.backgroundColor2
            width: root.width
            height: 40
            Grid {
                rows: 1
                spacing: 5
                anchors.right: parent.right
                anchors.rightMargin: spacing
                anchors.verticalCenter: parent.verticalCenter
                horizontalItemAlignment: Grid.AlignRight
                verticalItemAlignment: Grid.AlignVCenter
                MCtrl.Button {
                    id: buttonPrev
                    text: qsTr("\u25c2 上一步(&P)")
                    height: 30
                    font.family: Constants.font
                    enabled: stackLayout.currentIndex != 0
                    onClicked: {
                        --stackLayout.currentIndex;
                    }
                }
                MCtrl.Button {
                    id: buttonNext
                    text: qsTr("下一步(&N) \u25b8")
                    height: 30
                    font.family: Constants.font
                    enabled: stackLayout.currentIndex != stackLayout.count - 1
                    onClicked: {
                        ++stackLayout.currentIndex;
                    }
                }
                MCtrl.Button {
                    id: buttonCancel
                    text: qsTr("取消(&C)")
                    height: 30
                    font.family: Constants.font
                    onClicked: {
                        Qt.quit();
                    }
                }
            }
        }
    }
}
