import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

// Qt 6 removed `ColorDialog` and `MessageDialog` in `QtQuick.Dialogs`.
// To maintain forward compatibility, `MessageDialog` I implemented by myself is used.
// Fun fact: "Types in Qt.labs modules are not guaranteed to remain
// compatible in future versions" according to Qt Documentation.
Window {
    id: root
    // ui/MessageDialog.hpp
    enum Icon {
        None,
        Question,
        Warning,
        Error,
        Check,
        Info
    }
    property int icon: MessageDialog.Icon.None
    flags: Qt.Dialog
    modality: Qt.ApplicationModal
    color: Constants.backgroundColor
    height: icon.height + dialogButtonBox.height + row.padding * 2
    minimumHeight: row.height + dialogButtonBox.height
    width: Math.max(dialogButtonBox.contentWidth + dialogButtonBox.padding * 2, row.width)
    minimumWidth: row.width
    x: (screen.width - width) / 2
    y: (screen.height - height) / 2
    property string message: "Message Text"
    property alias standardButtons: dialogButtonBox.standardButtons
    signal accepted()
    title: "Title of dialog"
    signal clicked(button: AbstractButton)
    onClicked: {
        //
    }
    Row {
        id: row
        padding: 10
        spacing: 10
        Rectangle {
            id: iconRect
            width: icon == MessageDialog.Icon.None? 0: 64
            height: width
            color: "transparent"
            Image {
                anchors.fill: parent
                source: root.icon == MessageDialog.Icon.Question? "../../../images/question.png":
                        root.icon == MessageDialog.Icon.Warning?  "../../../images/warning.png":
                        root.icon == MessageDialog.Icon.Error?    "../../../images/error.png":
                        root.icon == MessageDialog.Icon.Check?    "../../../images/check.png":
                        root.icon == MessageDialog.Icon.Info?     "../../../images/info.png":
                                                                  ""
                fillMode: Image.PreserveAspectFit
                mipmap: true
            }
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: message
            color: Constants.contentColor1
            font: Constants.font
        }
    }
    DialogButtonBox {
        id: dialogButtonBox
        background: Rectangle {
            color: Constants.backgroundColor2
        }
        alignment: Qt.AlignRight
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: parent.width
        spacing: 5
        padding: 5
        onClicked: {
            root.close();
        }
        delegate: MCtrl.Button {
        }
    }
}
