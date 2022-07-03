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
                    ${SQLITE_MODERN_CPP_INCLUDE_DIRS})

add_subdirectory(${VST3SDK_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)

add_custom_target(GenerateLocaleBase COMMAND ${Qt5_DIR}/../../../bin/lupdate.exe ${CMAKE_SOURCE_DIR}/Musec.pro -ts ${CMAKE_SOURCE_DIR}/i18n/base.ts)
add_custom_target(GenerateLocaleCN COMMAND ${Qt5_DIR}/../../../bin/lupdate.exe ${CMAKE_SOURCE_DIR}/Musec.pro -ts ${CMAKE_SOURCE_DIR}/i18n/Musec_zh_CN.ts)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_path(SQLITE_MODERN_CPP_INCLUDE_DIRS "sqlite_modern_cpp.h")
#find_path(FFTWPP_INCLUDE_DIRS "fftwpp/Array.cc")
find_package(yaml-cpp
CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(avcpp CONFIG REQUIRED)

add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}} -DUNICODE -D_UNICODE)

add_compile_options(/W4 /Zc:wchar_t)

set(RESOURCE_FILE Musec_resource.rc)

qt5_add_resources(QT_RESOURCES ./Musec.qrc)

set(PROJECT_SOURCES
    audio/arrangement/ClipInTrack.hpp
    audio/arrangement/TrackSequence.hpp
    audio/base/AudioBufferView.hpp
    audio/base/AudioBufferView.cpp
    audio/base/Automation.hpp
    audio/base/Chrono.hpp
    audio/base/Constants.hpp
    audio/base/TempoAutomation.hpp
    audio/base/TimeSignature.cpp
    audio/base/TimeSignature.hpp
    audio/base/TimeSignatureSequence.hpp
    audio/device/IDevice.hpp
    audio/device/IDevice.hpp
    audio/driver/ASIOCallback.cpp
    audio/driver/ASIOCallback.hpp
    audio/driver/ASIODriver.cpp
    audio/driver/ASIODriver.hpp
    audio/driver/ASIODriverForward.hpp
    audio/driver/ASIODriverStreamInfo.hpp
    audio/driver/Literals.hpp
    audio/driver/Literals.cpp
    audio/engine/Graph.hpp
    audio/engine/MIDIClock.cpp
    audio/engine/MIDIClock.hpp
    audio/engine/Project.cpp
    audio/engine/Project.hpp
    audio/host/MusecVST3Host.cpp
    audio/host/MusecVST3Host.hpp
    audio/media/AudioSequence.cpp
    audio/media/AudioSequence.hpp
    audio/media/MIDISequence.cpp
    audio/media/MIDISequence.hpp
    audio/plugin/IPlugin.hpp
    audio/plugin/ShellPluginId.cpp
    audio/plugin/VST2Plugin.cpp
    audio/plugin/VST2Plugin.hpp
    audio/plugin/VST3Plugin.cpp
    audio/plugin/VST3Plugin.hpp
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
    concurrent/ThreadPool.cpp
    concurrent/ThreadPool.hpp
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
    event/MainWindow.cpp
    event/MainWindow.hpp
    event/ObjectCreateListener.cpp
    event/ObjectCreateListener.hpp
    event/SplashScreen.cpp
    event/SplashScreen.hpp
    event/SplashScreenForward.hpp
    event/SplashScreenWorkerThread.cpp
    event/SplashScreenWorkerThread.hpp
    math/Integration.cpp
    math/Integration.hpp
    math/QuadraticFunction.cpp
    math/QuadraticFunction.hpp
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
    model/ModelBase.hpp
    model/ModelInitializer.cpp
    model/ModelInitializer.hpp
    model/PluginListModel.cpp
    model/PluginListModel.hpp
    model/PluginSequenceModel.cpp
    model/PluginSequenceModel.hpp
    model/TrackListModel.cpp
    model/TrackListModel.hpp
    native/Native.cpp
    native/Native.hpp
    native/WindowsLibraryRAII.cpp
    native/WindowsLibraryRAII.hpp
    ui/FontUtility.cpp
    ui/FontUtility.hpp
    ui/MessageDialog.cpp
    ui/MessageDialog.hpp
    ui/Render.cpp
    ui/Render.hpp
    ui/UI.cpp
    ui/UI.hpp
    util/Endian.cpp
    util/Endian.hpp
    util/Literal.cpp
    util/Literal.hpp
    ui/PluginWindow.cpp
    ui/PluginWindow.hpp)

set(TEST_COMMON_SOURCES
    test/common/PluginWindowThread.cpp
    test/common/PluginWindowThread.hpp)

set(VST2PLUGINTEST_SOURCES
    test/VST2PluginTestWithQt/main.cpp)

set(VST3PLUGINTEST_SOURCES
    test/VST3PluginTestWithQt/main.cpp)

add_executable(Musec main.cpp ${PROJECT_SOURCES} ${RESOURCE_FILE} ${QT_RESOURCES})

add_executable(VST2PluginTestWithQt ${PROJECT_SOURCES} ${TEST_COMMON_SOURCES} ${VST2PLUGINTEST_SOURCES})

add_executable(VST3PluginTestWithQt ${PROJECT_SOURCES} ${TEST_COMMON_SOURCES} ${VST3PLUGINTEST_SOURCES})

target_compile_definitions(Musec
                           PRIVATE
                           $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
                           )

#target_include_directories(Musec PRIVATE ${FFTWPP_INCLUDE_DIRS})

target_link_libraries(Musec
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets
                      version # Windows API
                      base pluginterfaces sdk_common sdk_hosting # VST3
                      unofficial::sqlite3::sqlite3
                      yaml-cpp
                      fmt::fmt
                      spdlog::spdlog
                      avcpp::FFmpeg
                      avcpp::avcpp-static)

target_link_libraries(VST2PluginTestWithQt
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets
                      base pluginterfaces sdk_common sdk_hosting
                      unofficial::sqlite3::sqlite3
                      yaml-cpp
                      fmt::fmt
                      spdlog::spdlog
                      avcpp::FFmpeg
                      avcpp::avcpp-static)

target_link_libraries(VST3PluginTestWithQt
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets
                      base pluginterfaces sdk_common sdk_hosting
                      unofficial::sqlite3::sqlite3
                      yaml-cpp
                      fmt::fmt
                      spdlog::spdlog
                      avcpp::FFmpeg
                      avcpp::avcpp-static)

string(REPLACE "/" "\\" CMAKE_SOURCE_DIR_BACKSLASH ${CMAKE_SOURCE_DIR})
string(REPLACE "/" "\\" CMAKE_CURRENT_BINARY_DIR_BACKSLASH ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET Musec PRE_BUILD COMMAND ${Qt5_DIR}/../../../bin/lrelease.exe ${CMAKE_SOURCE_DIR}/i18n/Musec_zh_CN.ts)

add_custom_command(TARGET Musec POST_BUILD COMMAND copy /B /Y /V ${CMAKE_SOURCE_DIR_BACKSLASH}\\i18n\\Musec_zh_CN.qm ${CMAKE_CURRENT_BINARY_DIR_BACKSLASH}\\Musec_zh_CN.qm)

file(COPY Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
