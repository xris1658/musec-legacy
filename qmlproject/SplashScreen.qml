import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import Musec 1.0

Window {
    id: splashScreen
    flags: Qt.SplashScreen
    property EventBridge eventBridge: EventBridge
    property Strings strings: Strings
    title: strings.applicationName

    property alias bootText: text_.text
    signal setBootText(newBootText: string)

    onSetBootText: (newBootText) => {
        bootText = strings[newBootText];
    }

    width: 384
    height: 576
    visible: true

    Component.onCompleted: {
        Objects.splashScreen = splashScreen;
    }
    signal initDialog()
    signal closeDialog()

    onCloseDialog: {
        close();
    }

    Image {
        source: "images/Splashscreen.png"
        anchors.fill: parent
        Item {
            width: parent.width
            height: 60
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 36
            Text {
                id: text_
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                text: qsTr("Starting...")
                color: Constants.contentColor2
                font.family: Constants.font
                font.pixelSize: 15
                font.preferShaping: false // Disables ligature
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                // When ligature is enabled and the font is "Noto Sans CJK SC",
                // "config" will show as "con器g". It might be a bug of
                // Unicode of "器" is U+5668.
                // Unicode of "ﬁ" (ligature of "fi") is U+FB01.
                // For some reason, the word "ligature" is absent from the document,
                // making it a little harder to turn off ligature.
            }
        }
    }
}

