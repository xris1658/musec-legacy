pragma Singleton
import QtQml 2.15

QtObject {
    readonly property string applicationName:                "Musec"

    readonly property string initConfigText:                 qsTr("Welcome to Musec! Initializing application config...")
    readonly property string loadConfigText:                 qsTr("Loading application config...")
    readonly property string searchingAudioDeviceText:       qsTr("Looking for audio devices...")
    readonly property string searchingASIODriverText:        qsTr("Looking for ASIO driver...")
    readonly property string loadingASIODriverText:          qsTr("Loading ASIO driver...")
    readonly property string openingMainWindowText:          qsTr("Opening main window...")

    readonly property string enumeratingASIODriverErrorText: qsTr("An error occurred while looking for ASIO drivers on this computer. The program will run without loading the ASIO driver.")
    readonly property string noASIODriverFoundText:          qsTr("No ASIO drivers are found on this computer. The program will run without loading the ASIO driver.")
    readonly property string loadASIODriverErrorText:        qsTr("The ASIO driver cannot be loaded. The program will run without loading the ASIO driver.")

    readonly property string loadTranslationFailedText:      "The specified translation cannot be loaded. The program will show things in English."

    readonly property string optionSaveRebootPromptText:     qsTr("The option changes is saved. A restart is required to apply.")
}
