pragma Singleton
import QtQml 2.15

QtObject {
    readonly property string applicationName:                           "Musec"

    readonly property string initConfigText:                            qsTr("Welcome to Musec! Initializing application config...")
    readonly property string loadConfigText:                            qsTr("Loading application config...")
    readonly property string searchingAudioDeviceText:                  qsTr("Looking for audio devices...")
    readonly property string searchingASIODriverText:                   qsTr("Looking for ASIO driver...")
    readonly property string loadingASIODriverText:                     qsTr("Loading ASIO driver...")
    readonly property string openingMainWindowText:                     qsTr("Opening main window...")

    readonly property string enumeratingASIODriverErrorText:              qsTr("An error occurred while looking for ASIO drivers on this computer. The program will run without loading the ASIO driver.")
    readonly property string noASIODriverFoundText:                       qsTr("No ASIO drivers are found on this computer. The program will run without loading the ASIO driver.")
    readonly property string loadASIODriverErrorText:                     qsTr("The ASIO driver cannot be loaded. The program will run without loading the ASIO driver.")
    readonly property string loadASIODriverErrorTextWithErrorCode:        qsTr("The ASIO driver %1 cannot be loaded. The program will run without loading the ASIO driver.\nError code %2: %3")
    readonly property string loadASIODriverErrorTextWithErrorCodeAndInfo: qsTr("The ASIO driver %1 cannot be loaded. The program will run without loading the ASIO driver.\nError code %2: %3\n\nThe error information of the driver is as follows: \n%4")
    readonly property string loadASIODriverErrorTextWithInfo:             qsTr("The ASIO driver %1 cannot be loaded. The program will run without loading the ASIO driver.\n\nThe error information of the driver is as follows: \n%2")
    readonly property string loadASIODriverNoOutputText:                  qsTr("The ASIO driver %1 has no audio output.")

    readonly property string driverWarningTitleText:                    applicationName + " - " + qsTr("Driver Warning")
    readonly property string driverErrorTitleText:                      applicationName + " - " + qsTr("Driver Error")

    readonly property string loadTranslationFailedText:                 "The specified translation cannot be loaded. The program will show things in English."

    readonly property string optionSaveRebootPromptText:                qsTr("The option changes is saved. A restart is required to apply.")

    property string qtVersion
}
