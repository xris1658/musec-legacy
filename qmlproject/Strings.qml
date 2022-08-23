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

    readonly property string asioErrorOkText:                           qsTr("The operation is done succesfully.")
    readonly property string asioErrorSuccessText:                      qsTr("The operation is done succesfully.")
    readonly property string asioErrorNotPresentText:                   qsTr("There is no hardware input or output devices, or the devices are not available.")
    readonly property string asioErrorHardwareMalfunctionText:          qsTr("The current hardware is malfunctioning.")
    readonly property string asioErrorInvalidParameterText:             qsTr("The current parameter is invalid. You might need to check settings of the audio driver.")
    readonly property string asioErrorInvalidModeText:                  qsTr("The mode of the hardware is invalid.")
    readonly property string asioErrorSamplePositionIsNotAdvancingText: qsTr("The hardware can't return the sample position.")
    readonly property string asioErrorNoClockText:                      qsTr("The sample rate selected is not supported by the hardware.")
    readonly property string asioErrorNoMemoryText:                     qsTr("The available memory space is not enough to complete current operation.")
    readonly property string asioErrorUnknownText:                      qsTr("Unknown error. Unfortunately, that's all we know.")

    readonly property string loadTranslationFailedText:                 "The specified translation cannot be loaded. The program will show things in English."

    readonly property string optionSaveRebootPromptText:                qsTr("The option changes is saved. A restart is required to apply.")

}
