cmake_minimum_required(VERSION 3.14)

project(Musec VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick Widgets Qml Gui QuickCompiler REQUIRED)

include_directories(${ASIOSDK_PATH}
                    ${CLAP_SOURCE_DIR}/include
                    ${CLAP_HELPERS_DIR}/include
                    ${SQLITE_MODERN_CPP_INCLUDE_DIRS})

add_subdirectory(${VST3SDK_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)

add_custom_target(GenerateLocaleBase COMMAND ${Qt5_DIR}/../../../bin/lupdate.exe ${CMAKE_SOURCE_DIR}/Musec.pro -ts ${CMAKE_SOURCE_DIR}/i18n/base.ts)
add_custom_target(GenerateLocaleCN COMMAND ${Qt5_DIR}/../../../bin/lupdate.exe ${CMAKE_SOURCE_DIR}/Musec.pro -ts ${CMAKE_SOURCE_DIR}/i18n/Musec_zh_CN.ts)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_path(SQLITE_MODERN_CPP_INCLUDE_DIRS "sqlite_modern_cpp.h")
#find_path(FFTWPP_INCLUDE_DIRS "fftwpp/Array.cc")
find_package(yaml-cpp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(avcpp CONFIG REQUIRED)

add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}} -DUNICODE -D_UNICODE)

add_compile_options(/W4 /Zc:wchar_t)

qt5_add_resources(QT_RESOURCES ./Musec.qrc)

set(PROJECT_SOURCES
    audio/arrangement/ClipInTrack.hpp
    audio/arrangement/TrackSequence.hpp
    audio/base/AudioBufferView.cpp
    audio/base/AudioBufferView.hpp
    audio/base/Automation.cpp
    audio/base/Automation.hpp
    audio/base/Chrono.hpp
    audio/base/Constants.hpp
    audio/base/TempoAutomation.hpp
    audio/base/TimeSignature.cpp
    audio/base/TimeSignature.hpp
    audio/base/TimeSignatureSequence.hpp
    audio/device/IDevice.hpp
    audio/device/IDevice.hpp
    audio/device/ISpeakerGroup.hpp
    audio/device/ISpeakerGroupCollection.hpp
    audio/driver/ASIOCallback.cpp
    audio/driver/ASIOCallback.hpp
    audio/driver/ASIODriver.cpp
    audio/driver/ASIODriver.hpp
    audio/driver/ASIODriverBasicInfo.hpp
    audio/driver/ASIODriverForward.hpp
    audio/driver/ASIODriverStreamInfo.hpp
    audio/driver/ASIOSampleTypeName.cpp
    audio/driver/ASIOSampleTypeName.hpp
    audio/driver/Literals.cpp
    audio/driver/Literals.hpp
    audio/engine/Graph.hpp
    audio/engine/MIDIClock.hpp
    audio/engine/Project.cpp
    audio/engine/Project.hpp
    audio/host/CLAPEvents.cpp
    audio/host/CLAPEvents.hpp
    audio/host/CLAPHost.cpp
    audio/host/CLAPHost.hpp
    audio/host/MusecVST3Host.cpp
    audio/host/MusecVST3Host.hpp
    audio/media/AudioSequence.cpp
    audio/media/AudioSequence.hpp
    audio/media/MIDISequence.cpp
    audio/media/MIDISequence.hpp
    audio/midi/MIDIInputDevice.cpp
    audio/midi/MIDIInputDevice.hpp
    audio/midi/MIDIMessage.cpp
    audio/midi/MIDIMessage.hpp
    audio/midi/MIDIMessageCast.cpp
    audio/midi/MIDIMessageCast.hpp
    audio/plugin/CLAPPlugin.cpp
    audio/plugin/CLAPPlugin.hpp
    audio/plugin/CLAPPluginParameter.cpp
    audio/plugin/CLAPPluginParameter.hpp
    audio/plugin/CLAPSpeakerGroup.cpp
    audio/plugin/CLAPSpeakerGroup.hpp
    audio/plugin/CLAPSpeakerGroupCollection.cpp
    audio/plugin/CLAPSpeakerGroupCollection.hpp
    audio/plugin/CLAPUtils.hpp
    audio/plugin/IParameter.hpp
    audio/plugin/IPlugin.hpp
    audio/plugin/VST2Plugin.cpp
    audio/plugin/VST2Plugin.hpp
    audio/plugin/VST2PluginForward.hpp
    audio/plugin/VST2PluginParameter.cpp
    audio/plugin/VST2PluginParameter.hpp
    audio/plugin/VST2PluginPool.cpp
    audio/plugin/VST2PluginPool.hpp
    audio/plugin/VST2PluginShellPluginId.cpp
    audio/plugin/VST2PluginShellPluginId.hpp
    audio/plugin/VST2SpeakerGroup.cpp
    audio/plugin/VST2SpeakerGroup.hpp
    audio/plugin/VST2SpeakerGroupCollection.cpp
    audio/plugin/VST2SpeakerGroupCollection.hpp
    audio/plugin/VST3Plugin.cpp
    audio/plugin/VST3Plugin.hpp
    audio/plugin/VST3PluginComponentHandler.cpp
    audio/plugin/VST3PluginComponentHandler.hpp
    audio/plugin/VST3PluginParameter.cpp
    audio/plugin/VST3PluginParameter.hpp
    audio/plugin/VST3PluginPlugFrame.cpp
    audio/plugin/VST3PluginPlugFrame.hpp
    audio/plugin/VST3SpeakerGroup.cpp
    audio/plugin/VST3SpeakerGroup.hpp
    audio/plugin/VST3SpeakerGroupCollection.cpp
    audio/plugin/VST3SpeakerGroupCollection.hpp
    audio/track/AudioTrack.cpp
    audio/track/AudioTrack.hpp
    audio/track/ITrack.hpp
    audio/track/InstrumentTrack.cpp
    audio/track/InstrumentTrack.hpp
    audio/track/MIDITrack.cpp
    audio/track/MIDITrack.hpp
    audio/track/PluginSequence.hpp
    audio/track/TrackInformation.hpp
    audio/util/Util.cpp
    audio/util/Util.hpp
    base/AssetDirectoryBase.hpp
    base/Base.hpp
    base/CircularDeque.hpp
    base/Color.cpp
    base/Color.hpp
    base/Constants.hpp
    base/FileBase.hpp
    base/FixedSizeMemoryPool.cpp
    base/FixedSizeMemoryPool.hpp
    base/FolderBase.hpp
    base/PluginBase.cpp
    base/PluginBase.hpp
    base/QmlBase.hpp
    concurrent/ButlerThread.cpp
    concurrent/ButlerThread.hpp
    concurrent/ThreadPool.cpp
    concurrent/ThreadPool.hpp
    controller/ASIODriverController.cpp
    controller/ASIODriverController.hpp
    controller/AppController.cpp
    controller/AppController.hpp
    controller/AssetController.cpp
    controller/AssetController.hpp
    controller/AssetDirectoryController.cpp
    controller/AssetDirectoryController.hpp
    controller/AudioEngineController.cpp
    controller/AudioEngineController.hpp
    controller/ConfigController.cpp
    controller/ConfigController.hpp
    controller/GeneralSettingsController.cpp
    controller/GeneralSettingsController.hpp
    controller/LoggingController.cpp
    controller/LoggingController.hpp
    controller/MIDIClockController.cpp
    controller/MIDIClockController.hpp
    controller/PluginController.cpp
    controller/PluginController.hpp
    controller/PluginSettingsController.cpp
    controller/PluginSettingsController.hpp
    dao/AssetDirectoryDAO.cpp
    dao/AssetDirectoryDAO.hpp
    dao/ConfigDAO.cpp
    dao/ConfigDAO.hpp
    dao/DatabaseDAO.cpp
    dao/DatabaseDAO.hpp
    dao/LoggingDAO.cpp
    dao/LoggingDAO.hpp
    dao/PluginDAO.cpp
    dao/PluginDAO.hpp
    dao/PluginDirectoryDAO.cpp
    dao/PluginDirectoryDAO.hpp
    entities/CompleteTrack.cpp
    entities/CompleteTrack.hpp
    entities/EntitiesInitializer.cpp
    entities/EntitiesInitializer.hpp
    entities/Plugin.cpp
    entities/Plugin.hpp
    event/EventBase.cpp
    event/EventBase.hpp
    event/EventHandler.cpp
    event/EventHandler.hpp
    event/MainWindowEvent.cpp
    event/MainWindowEvent.hpp
    event/ObjectCreateListener.cpp
    event/ObjectCreateListener.hpp
    event/SplashScreen.cpp
    event/SplashScreen.hpp
    event/SplashScreenForward.hpp
    event/SplashScreenWorkerThread.cpp
    event/SplashScreenWorkerThread.hpp
    i18n/I18N.cpp
    i18n/I18N.hpp
    math/Constants.hpp
    math/Integration.cpp
    math/Integration.hpp
    math/QuadraticFunction.cpp
    math/QuadraticFunction.hpp
    model/ASIOChannelInfoListModel.cpp
    model/ASIOChannelInfoListModel.hpp
    model/ASIODriverListModel.cpp
    model/ASIODriverListModel.hpp
    model/AssetDirectoryListModel.cpp
    model/AssetDirectoryListModel.hpp
    model/AudioTrackSequenceModel.cpp
    model/AudioTrackSequenceModel.hpp
    model/AutomationModel.cpp
    model/AutomationModel.hpp
    model/FileListModel.cpp
    model/FileListModel.hpp
    model/FolderListModel.cpp
    model/FolderListModel.hpp
    model/MIDIInputDeviceListModel.cpp
    model/MIDIInputDeviceListModel.hpp
    model/ModelBase.hpp
    model/ModelInitializer.cpp
    model/ModelInitializer.hpp
    model/PluginListModel.cpp
    model/PluginListModel.hpp
    model/PluginSequenceModel.cpp
    model/PluginSequenceModel.hpp
    model/TempoAutomationModel.cpp
    model/TempoAutomationModel.hpp
    model/TrackListModel.cpp
    model/TrackListModel.hpp
    model/TranslationFileModel.cpp
    model/TranslationFileModel.hpp
    native/ASIODriverImpl.cpp
    native/ASIODriverImpl.hpp
    native/CompilerSpecific.hpp
    native/MIDIInputDeviceImpl.cpp
    native/MIDIInputDeviceImpl.hpp
    native/MIDIMessageImpl.cpp
    native/MIDIMessageImpl.hpp
    native/Native.cpp
    native/Native.hpp
    native/WinRTForward.hpp
    native/WindowsLibraryRAII.cpp
    native/WindowsLibraryRAII.hpp
    ui/FontUtility.cpp
    ui/FontUtility.hpp
    ui/MessageDialog.cpp
    ui/MessageDialog.hpp
    ui/PluginWindow.cpp
    ui/PluginWindow.hpp
    ui/Render.cpp
    ui/Render.hpp
    ui/UI.cpp
    ui/UI.hpp
    util/Endian.cpp
    util/Endian.hpp
    util/Literal.cpp
    util/Literal.hpp
    util/Stopwatch.hpp
    )

set(RESOURCE_FILE Musec_resource.rc)

set(PROJECT_PCH pch.hpp)

set(TEST_COMMON_SOURCES
    test/common/PluginWindowThread.cpp
    test/common/PluginWindowThread.hpp)

set(AUTOMATIONTEST_SOURCES
    audio/base/Automation.cpp
    audio/base/Automation.hpp
    math/QuadraticFunction.cpp
    math/QuadraticFunction.hpp
    model/AutomationModel.cpp
    model/AutomationModel.hpp
    test/AutomationTest/main.cpp
    test/AutomationTest/AutomationTest.qrc)

set(MUSECCONTROLSTEST_SOURCES
    test/MusecControlsTest/main.cpp
    test/MusecControlsTest/MusecControlsTest.qrc)

set(VST2PLUGINTEST_SOURCES
    test/VST2PluginTestWithQt/main.cpp)

set(VST3PLUGINTEST_SOURCES
    test/VST3PluginTestWithQt/main.cpp)

set(VST3_EXTRA_SOURCES
    ${VST3SDK_SOURCE_DIR}/public.sdk/source/common/memorystream.cpp)

add_executable(Musec main.cpp ${PROJECT_SOURCES} ${RESOURCE_FILE} ${QT_RESOURCES} ${VST3_EXTRA_SOURCES})

target_precompile_headers(Musec PRIVATE ${PROJECT_PCH})

add_executable(AutomationTest ${QT_RESOURCES} ${AUTOMATIONTEST_SOURCES})

add_executable(MusecControlsTest ${QT_RESOURCES} ${MUSECCONTROLSTEST_SOURCES})

add_executable(VST2PluginTestWithQt ${PROJECT_SOURCES} ${TEST_COMMON_SOURCES} ${VST2PLUGINTEST_SOURCES})

add_executable(VST3PluginTestWithQt ${PROJECT_SOURCES} ${TEST_COMMON_SOURCES} ${VST3PLUGINTEST_SOURCES} ${VST3_EXTRA_SOURCES})

target_compile_definitions(Musec
                           PRIVATE
                           $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
                           )

#target_include_directories(Musec PRIVATE ${FFTWPP_INCLUDE_DIRS})

target_link_options(Musec
                    PRIVATE
                    "/MANIFESTDEPENDENCY:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' publicKeyToken='6595b64144ccf1df' language='*' processorArchitecture='*'")

target_link_libraries(Musec
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets
                      version # Windows API
                      windowsapp # WinRT API
                      base pluginterfaces sdk_common sdk_hosting # VST3
                      unofficial::sqlite3::sqlite3
                      yaml-cpp
                      fmt::fmt
                      spdlog::spdlog
                      avcpp::FFmpeg
                      avcpp::avcpp-static)

target_link_libraries(AutomationTest
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(MusecControlsTest
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(VST2PluginTestWithQt
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets
                      version
                      windowsapp
                      base pluginterfaces sdk_common sdk_hosting
                      unofficial::sqlite3::sqlite3
                      yaml-cpp
                      fmt::fmt
                      spdlog::spdlog
                      avcpp::FFmpeg
                      avcpp::avcpp-static)

target_link_libraries(VST3PluginTestWithQt
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets
                      version
                      windowsapp
                      base pluginterfaces sdk_common sdk_hosting
                      unofficial::sqlite3::sqlite3
                      yaml-cpp
                      fmt::fmt
                      spdlog::spdlog
                      avcpp::FFmpeg
                      avcpp::avcpp-static)

add_custom_command(TARGET Musec POST_BUILD COMMAND ${Qt5_DIR}/../../../bin/lrelease.exe ${CMAKE_SOURCE_DIR}/i18n/Musec_zh_CN.ts -qm ${CMAKE_CURRENT_BINARY_DIR}/Musec_zh_CN.qm)

file(COPY Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${Qt5_DIR}/../../../translations/qt_zh_CN.qm DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/translations)
file(COPY ${Qt5_DIR}/../../../../../Tools/QtDesignStudio/qt5_design_studio_reduced_version/translations/qtbase_zh_CN.qm DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/translations)
file(COPY ${Qt5_DIR}/../../../../../Tools/QtDesignStudio/qt5_design_studio_reduced_version/translations/qtquickcontrols_zh_CN.qm DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/translations)
file(COPY ${Qt5_DIR}/../../../../../Tools/QtDesignStudio/qt5_design_studio_reduced_version/translations/qtquickcontrols2_zh_CN.qm DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/translations)