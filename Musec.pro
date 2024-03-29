QT += quick
QT += widgets

CONFIG += c++17
CONFIG += resources_big
CONFIG += qtquickcompiler
CONFIG += lrelease embed_translations
CONFIG += precompile_header
CONFIG += console

CONFIG(debug, debug | release) {
    CONFIG += qml_debug
}

QMAKE_CXXFLAGS += /Zc:wchar_t /W4 /Zc:__cplusplus

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Variables
ASIOSDK_DIR        = C:/asiosdk_2.3.3_2019-06-14
VST3SDK_DIR        = D:/apps/vst3sdk
VCPKG_DIR          = D:/apps/vcpkg
CLAP_DIR           = D:/apps/clap
CLAP_HELPER_DIR    = D:/apps/clap-helpers
message(ASIO SDK directory: $$ASIOSDK_DIR)
message( VST SDK direcotry: $$VST3SDK_DIR)
message(   vcpkg directory: $$VCPKG_DIR)
message(CLAP SDK directory: $$CLAP_DIR)
# Variables

INCLUDEPATH += $$VCPKG_DIR/installed/x64-windows/include \
               $$VST3SDK_DIR \
               $$ASIOSDK_DIR \
               $$CLAP_DIR/include \
               $$CLAP_HELPER_DIR/include

CONFIG(debug, debug | release) {
    QMAKE_LIBDIR += \
        $$VCPKG_DIR/installed/x64-windows/debug/lib \
        $$VST3SDK_DIR/build/lib/Debug
}

CONFIG(release, debug | release) {
    QMAKE_LIBDIR += \
        $$VCPKG_DIR/installed/x64-windows/lib \
        $$VST3SDK_DIR/build/lib/Release
}

message(Library directory: $$QMAKE_LIBDIR)

DEFINES += _CRT_SECURE_NO_WARNINGS

PRECOMPILED_HEADER = pch.hpp

HEADERS += \
    audio/arrangement/ClipInTrack.hpp \
    audio/arrangement/TrackSequence.hpp \
    audio/base/AudioBufferView.hpp \
    audio/base/Automation.hpp \
    audio/base/Channel.hpp \
    audio/base/Chrono.hpp \
    audio/base/Constants.hpp \
    audio/base/TempoAutomation.hpp \
    audio/base/TimeSignature.hpp \
    audio/base/TimeSignatureSequence.hpp \
    audio/device/IDevice.hpp \
    audio/device/IDevice.hpp \
    audio/device/IChannelGroup.hpp \
    audio/device/IChannelGroupCollection.hpp \
    audio/driver/ASIOCallback.hpp \
    audio/driver/ASIOChannelGroup.hpp \
    audio/driver/ASIODriver.hpp \
    audio/driver/ASIODriverBasicInfo.hpp \
    audio/driver/ASIOErrorText.hpp \
    audio/driver/ASIODriverForward.hpp \
    audio/driver/ASIOSampleTypeName.hpp \
    audio/engine/MIDIClock.hpp \
    audio/engine/Project.hpp \
    audio/host/CLAPEvents.hpp \
    audio/host/CLAPHost.hpp \
    audio/host/VST3Host.hpp \
    audio/media/AudioSequence.hpp \
    audio/media/MIDISequence.hpp \
    audio/midi/MIDIInputDevice.hpp \
    audio/midi/MIDIMessage.hpp \
    audio/midi/MIDIMessageCast.hpp \
    audio/plugin/CLAPPlugin.hpp \
    audio/plugin/CLAPPluginParameter.hpp \
    audio/plugin/CLAPSpeakerGroup.hpp \
    audio/plugin/CLAPSpeakerGroupCollection.hpp \
    audio/plugin/CLAPUtils.hpp \
    audio/plugin/IParameter.hpp \
    audio/plugin/IPlugin.hpp \
    audio/plugin/VST3Plugin.hpp \
    audio/plugin/VST3ComponentHandler.hpp \
    audio/plugin/VST3ParameterChanges.hpp \
    audio/plugin/VST3ParameterValueQueue.hpp \
    audio/plugin/VST3PluginParameter.hpp \
    audio/plugin/VST3PluginPlugFrame.hpp \
    audio/plugin/VST3SpeakerGroup.hpp \
    audio/plugin/VST3SpeakerGroupCollection.hpp \
    audio/plugin/VST3Stream.hpp \
    audio/track/AudioTrack.hpp \
    audio/track/InstrumentTrack.hpp \
    audio/track/ITrack.hpp \
    audio/track/MIDITrack.hpp \
    audio/track/PluginSequence.hpp \
    audio/track/TrackInformation.hpp \
    audio/util/Util.hpp \
    base/AssetDirectoryBase.hpp \
    base/Base.hpp \
    base/CircularDeque.hpp \
    base/Color.hpp \
    base/Constants.hpp \
    base/FileBase.hpp \
    base/FixedSizeMemoryPool.hpp \
    base/FolderBase.hpp \
    base/PluginBase.hpp \
    base/QmlBase.hpp \
    concurrent/ThreadId.hpp \
    concurrent/ThreadPool.hpp \
    controller/AppController.hpp \
    controller/ASIODriverController.hpp \
    controller/AssetController.hpp \
    controller/AssetDirectoryController.hpp \
    controller/AudioEngineController.hpp \
    controller/ConfigController.hpp \
    controller/GeneralSettingsController.hpp \
    controller/LoggingController.hpp \
    controller/MIDIClockController.hpp \
    controller/PluginController.hpp \
    controller/PluginSettingsController.hpp \
    dao/AssetDirectoryDAO.hpp \
    dao/ConfigDAO.hpp \
    dao/DatabaseDAO.hpp \
    dao/LoggingDAO.hpp \
    dao/PluginCacheDAO.hpp \
    dao/PluginDAO.hpp \
    dao/PluginDirectoryDAO.hpp \
    entities/CompleteTrack.hpp \
    entities/EntitiesInitializer.hpp \
    entities/Plugin.hpp \
    event/EventBase.hpp \
    event/EventHandler.hpp \
    event/MainWindowEvent.hpp \
    event/ObjectCreateListener.hpp \
    event/SplashScreen.hpp \
    event/SplashScreenForward.hpp \
    event/SplashScreenWorkerThread.hpp \
    i18n/I18N.hpp \
    math/Constants.hpp \
    math/Integration.hpp \
    math/QuadraticFunction.hpp \
    model/ASIOChannelInfoListModel.hpp \
    model/ASIODriverListModel.hpp \
    model/AssetDirectoryListModel.hpp \
    model/AudioTrackSequenceModel.hpp \
    model/AutomationModel.hpp \
    model/FileListModel.hpp \
    model/FolderListModel.hpp \
    model/MIDIInputDeviceListModel.hpp \
    model/ModelBase.hpp \
    model/ModelInitializer.hpp \
    model/PluginListModel.hpp \
    model/PluginParameterListModel.hpp \
    model/PluginParameterValueListModel.hpp \
    model/PluginSequenceModel.hpp \
    model/TempoAutomationModel.hpp \
    model/TrackListModel.hpp \
    model/TranslationFileModel.hpp \
    model/VolumeFaderScaleModel.hpp \
    native/ASIODriverImpl.hpp \
    native/CompilerSpecific.hpp \
    native/Library.hpp \
    native/LibraryImpl.hpp \
    native/MIDIInputDeviceImpl.hpp \
    native/MIDIMessageImpl.hpp \
    native/Native.hpp \
    native/WinRTForward.hpp \
    ui/FontUtility.hpp \
    ui/MessageDialog.hpp \
    ui/PluginWindow.hpp \
    ui/Render.hpp \
    ui/UI.hpp \
    util/Endian.hpp \
    util/FileHash.hpp \
    util/Literal.hpp \
    util/Stopwatch.hpp

SOURCES += \
    main.cpp \
    audio/base/AudioBufferView.cpp \
    audio/base/Automation.cpp \
    audio/base/TimeSignature.cpp \
    audio/device/IChannelGroup.cpp \
    audio/driver/ASIOCallback.cpp \
    audio/driver/ASIOChannelGroup.cpp \
    audio/driver/ASIODriver.cpp \
    audio/driver/ASIOErrorText.cpp \
    audio/driver/ASIOSampleTypeName.cpp \
    audio/engine/Project.cpp \
    audio/host/CLAPEvents.cpp \
    audio/host/CLAPHost.cpp \
    audio/host/VST3Host.cpp \
    audio/media/AudioSequence.cpp \
    audio/media/MIDISequence.cpp \
    audio/midi/MIDIInputDevice.cpp \
    audio/midi/MIDIMessage.cpp \
    audio/midi/MIDIMessageCast.cpp \
    audio/plugin/CLAPPlugin.cpp \
    audio/plugin/CLAPPluginParameter.cpp \
    audio/plugin/CLAPSpeakerGroup.cpp \
    audio/plugin/CLAPSpeakerGroupCollection.cpp \
    audio/plugin/VST3Plugin.cpp \
    audio/plugin/VST3ComponentHandler.cpp \
    audio/plugin/VST3MemoryStream.cpp \
    audio/plugin/VST3ParameterChanges.cpp \
    audio/plugin/VST3ParameterValueQueue.cpp \
    audio/plugin/VST3PluginParameter.cpp \
    audio/plugin/VST3PluginPlugFrame.cpp \
    audio/plugin/VST3SpeakerGroup.cpp \
    audio/plugin/VST3SpeakerGroupCollection.cpp \
    audio/plugin/VST3Stream.cpp \
    audio/track/AudioTrack.cpp \
    audio/track/InstrumentTrack.cpp \
    audio/track/MIDITrack.cpp \
    audio/util/Util.cpp \
    base/Color.cpp \
    base/FixedSizeMemoryPool.cpp \
    base/PluginBase.cpp \
    concurrent/ThreadId.cpp \
    concurrent/ThreadPool.cpp \
    controller/AppController.cpp \
    controller/ASIODriverController.cpp \
    controller/AssetController.cpp \
    controller/AssetDirectoryController.cpp \
    controller/AudioEngineController.cpp \
    controller/ConfigController.cpp \
    controller/GeneralSettingsController.cpp \
    controller/LoggingController.cpp \
    controller/MIDIClockController.cpp \
    controller/PluginController.cpp \
    controller/PluginSettingsController.cpp \
    dao/AssetDirectoryDAO.cpp \
    dao/ConfigDAO.cpp \
    dao/DatabaseDAO.cpp \
    dao/LoggingDAO.cpp \
    dao/PluginCacheDAO.cpp \
    dao/PluginDAO.cpp \
    dao/PluginDirectoryDAO.cpp \
    entities/CompleteTrack.cpp \
    entities/EntitiesInitializer.cpp \
    entities/Plugin.cpp \
    event/EventBase.cpp \
    event/EventHandler.cpp \
    event/MainWindowEvent.cpp \
    event/ObjectCreateListener.cpp \
    event/SplashScreen.cpp \
    event/SplashScreenWorkerThread.cpp \
    i18n/I18N.cpp \
    math/Integration.cpp \
    math/QuadraticFunction.cpp \
    model/ASIOChannelInfoListModel.cpp \
    model/ASIODriverListModel.cpp \
    model/AssetDirectoryListModel.cpp \
    model/AudioTrackSequenceModel.cpp \
    model/AutomationModel.cpp \
    model/FileListModel.cpp \
    model/FolderListModel.cpp \
    model/MIDIInputDeviceListModel.cpp \
    model/ModelInitializer.cpp \
    model/PluginListModel.cpp \
    model/PluginParameterListModel.cpp \
    model/PluginParameterValueListModel.cpp \
    model/PluginSequenceModel.cpp \
    model/TempoAutomationModel.cpp \
    model/TrackListModel.cpp \
    model/TranslationFileModel.cpp \
    model/VolumeFaderScaleModel.cpp \
    native/ASIODriverImpl.cpp \
    native/Library.cpp \
    native/MIDIInputDeviceImpl.cpp \
    native/MIDIMessageImpl.cpp \
    native/Native.cpp \
    ui/FontUtility.cpp \
    ui/MessageDialog.cpp \
    ui/PluginWindow.cpp \
    ui/Render.cpp \
    ui/UI.cpp \
    util/Endian.cpp \
    util/FileHash.cpp \
    util/Literal.cpp

win32: SOURCES += \
    native/LibraryImplWin32.cpp

RESOURCES += \
    Musec.qrc

LIBS += \
    sqlite3.lib \  # SQLite 3
    AdvAPI32.lib \ # Registry API
    Shell32.lib \  # SHGetFolderPathW
    ole32.lib \
    version.lib \  # Musec::Native::getProductVersion
    # WinRT API
    windowsapp.lib \
    # VST3 SDK
    sdk.lib \
    base.lib \
    pluginterfaces.lib \
    sdk_common.lib \
    sdk_hosting.lib \
    ## FFmpeg
    avcodec.lib \
    avdevice.lib \
    avfilter.lib \
    avformat.lib \
    avutil.lib \
    swresample.lib \
    swscale.lib \
    # avcpp
    avcpp.lib

TRANSLATIONS += i18n/Musec_zh_CN.ts

CONFIG(debug, debug | release) {
    LIBS += yaml-cppd.lib \
    fmtd.lib \
    spdlogd.lib
}
CONFIG(release, debug | release) {
    LIBS += yaml-cpp.lib \
    fmt.lib \
    spdlog.lib
}

RC_FILE = ./Musec_resource.rc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qmlproject/imports

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = qmlproject/mockup

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
#msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

# Change the exception handling mode Release uses
# Reference:
# http://www.databaseforum.info/2/9/f69800e14add61dd.html
# https://docs.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-160
QMAKE_CXXFLAGS_RELEASE -= /EHsc
QMAKE_CXXFLAGS_RELEASE += /EHs
QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= /EHsc
QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO += /EHs
