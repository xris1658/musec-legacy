cmake_minimum_required(VERSION 3.14)

project(Musec VERSION  0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets REQUIRED)

include_directories(C:/asiosdk_2.3.3_2019-06-14)

set(vst3sdk_SOURCE_DIR D:/apps/vst3sdk)
add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_path(SQLITE_MODERN_CPP_INCLUDE_DIRS "sqlite_modern_cpp.h")
find_package(yaml-cpp CONFIG REQUIRED)

include_directories(
        "D:/apps/vst3sdk"
        "D:/apps/vst3sdk/public.sdk/source"
        "D:/apps/vst3sdk/pluginterfaces"
        ${SQLITE_MODERN_CPP_INCLUDE_DIRS}
)

set(PROJECT_SOURCES
    # 源文件
    main.cpp
    audio/driver/ASIOCallback.cpp
    audio/driver/ASIODriver.cpp
    audio/host/MusecVST3Host.cpp
    base/PluginBase.cpp
    controller/AppController.cpp
    controller/AssetDirectoryController.cpp
    controller/PluginController.cpp
    controller/PluginSettingsController.cpp
    dao/AssetDAO.cpp
    dao/AssetDirectoryDAO.cpp
    dao/ConfigDAO.cpp
    dao/DatabaseDAO.cpp
    dao/PluginDAO.cpp
    dao/PluginDirectoryDAO.cpp
    event/EventBase.cpp
    event/EventHandler.cpp
    event/MainWindow.cpp
    event/SplashScreen.cpp
    event/SplashScreenWorkerThread.cpp
    model/ASIODriverListModel.cpp
    model/AssetDirectoryListModel.cpp
    model/ModelInitializer.cpp
    model/PluginListModel.cpp
    native/Native.cpp
    native/WindowsLibraryRAII.cpp
    ui/FontUtility.cpp
    ui/UI.cpp
    # 头文件
    audio/driver/ASIOCallback.hpp
    audio/driver/ASIODriver.hpp
    audio/driver/ASIODriverStreamInfo.hpp
    audio/driver/IASIODriver.hpp
    audio/host/MusecVST3Host.hpp
    base/AssetDirectoryBase.hpp
    base/Base.hpp
    base/Constants.hpp
    base/PluginBase.hpp
    controller/AppController.hpp
    controller/AssetDirectoryController.hpp
    controller/PluginController.hpp
    controller/PluginSettingsController.hpp
    dao/AssetDAO.hpp
    dao/AssetDirectoryDAO.hpp
    dao/ConfigDAO.hpp
    dao/DatabaseDAO.hpp
    dao/PluginDAO.hpp
    dao/PluginDirectoryDAO.hpp
    event/EventBase.hpp
    event/EventHandler.hpp
    event/MainWindow.hpp
    event/SplashScreen.hpp
    event/SplashScreenForward.hpp
    event/SplashScreenWorkerThread.hpp
    model/ASIODriverListModel.hpp
    model/AssetDirectoryListModel.hpp
    model/ModelBase.hpp
    model/ModelInitializer.hpp
    model/PluginListModel.hpp
    native/Native.hpp
    native/WindowsLibraryRAII.hpp
    ui/FontUtility.hpp
    ui/UI.hpp
    util/FunctorInitializer.hpp
    Musec.qrc
    )

if(${QT_VERSION} GREATER_EQUAL 6)
    qt_add_executable(Musec
                      MANUAL_FINALIZATION
                      ${PROJECT_SOURCES}
                      )
else()
    if(ANDROID)
        add_library(Musec
                    SHARED
                    ${PROJECT_SOURCES}
                    )
    else()
        add_executable(Musec ${PROJECT_SOURCES})
    endif()
endif()

target_compile_definitions(Musec
                           PRIVATE
                           $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
                           )

target_link_libraries(Musec
                      PRIVATE
                      Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Widgets
                      sdk
                      unofficial::sqlite3::sqlite3
                      yaml-cpp)

set_target_properties(Musec PROPERTIES
                      MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
                      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                      MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                      )

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(Musec)
    qt_finalize_executable(Musec)
endif()
