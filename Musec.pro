QT += quick
QT += widgets
# 1. 直接使用了 COM API，但没有使用 Active Qt
# 2. Qt 6 中的 Active Qt 好像出了点小意外
#QT += axcontainer

CONFIG += c++17
CONFIG += resources_big
CONFIG += qtquickcompiler

QMAKE_CXXFLAGS += /Zc:wchar_t

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += D:/apps/vcpkg/installed/x64-windows/include \
               D:/apps/vst3sdk \
               D:/apps/vst3sdk/pluginterfaces \
               D:/apps/vst3sdk/public.sdk/source \
               C:/asiosdk_2.3.3_2019-06-14

CONFIG(debug, debug | release) {
    QMAKE_LIBDIR += \
        D:/apps/vcpkg/installed/x64-windows/debug/lib \
        D:/apps/vst3sdk/build/debug/lib
}
CONFIG(release, debug | release) {
     QMAKE_LIBDIR += \
        D:/apps/vst3sdk/build/release/lib \
        D:/apps/vcpkg/installed/x64-windows/lib
}

DEFINES += _CRT_SECURE_NO_WARNINGS

HEADERS += \
    audio/base/Automation.hpp \
    audio/base/AutomationWithCurve.hpp \
    audio/base/Chrono.hpp \
    audio/base/TempoAutomation.hpp \
    audio/base/TempoAutomationWithCurve.hpp \
    audio/base/TimeSignature.hpp \
    audio/base/TimeSignatureSequence.hpp \
    audio/driver/ASIODriverForward.hpp \
    audio/driver/ASIOCallback.hpp \
    audio/driver/ASIODriver.hpp \
    audio/driver/ASIODriverStreamInfo.hpp \
    audio/engine/MIDIClock.hpp \
    audio/host/MusecVST3Host.hpp \
    audio/plugin/IPlugin.hpp \
    audio/plugin/VST2Plugin.hpp \
    audio/plugin/VST3Plugin.hpp \
    audio/util/Util.hpp \
    base/AssetDirectoryBase.hpp \
    base/Base.hpp \
    base/Constants.hpp \
    base/PluginBase.hpp \
    base/Pool.hpp \
    controller/AppController.hpp \
    controller/ASIODriverController.hpp \
    controller/AssetDirectoryController.hpp \
    controller/ConfigController.hpp \
    controller/GeneralSettingsController.hpp \
    controller/PluginController.hpp \
    controller/PluginSettingsController.hpp \
    dao/AssetDirectoryDAO.hpp \
    dao/ConfigDAO.hpp \
    dao/DatabaseDAO.hpp \
    dao/LoggingDAO.hpp \
    dao/PluginDAO.hpp \
    dao/PluginDirectoryDAO.hpp \
    dkwtp/Graph.hpp \
    event/EventBase.hpp \
    event/EventHandler.hpp \
    event/MainWindow.hpp \
    event/SplashScreen.hpp \
    event/SplashScreenForward.hpp \
    event/SplashScreenWorkerThread.hpp \
    math/Integration.hpp \
    math/QuadraticFunction.hpp \
    model/ASIODriverListModel.hpp \
    model/AssetDirectoryListModel.hpp \
    model/ModelBase.hpp \
    model/ModelInitializer.hpp \
    model/PluginListModel.hpp \
    native/Native.hpp \
    native/WindowsLibraryRAII.hpp \
    ui/FontUtility.hpp \
    ui/Render.hpp \
    ui/UI.hpp \
    util/FunctorInitializer.hpp \
    util/Literal.hpp

SOURCES += \
    main.cpp \
    audio/base/TimeSignature.cpp \
    audio/driver/ASIOCallback.cpp \
    audio/driver/ASIODriver.cpp \
    audio/engine/MIDIClock.cpp \
    audio/host/MusecVST3Host.cpp \
    audio/plugin/VST2Plugin.cpp \
    audio/plugin/VST3Plugin.cpp \
    audio/util/Util.cpp \
    base/PluginBase.cpp \
    controller/AppController.cpp \
    controller/ASIODriverController.cpp \
    controller/AssetDirectoryController.cpp \
    controller/ConfigController.cpp \
    controller/GeneralSettingsController.cpp \
    controller/PluginController.cpp \
    controller/PluginSettingsController.cpp \
    dao/AssetDirectoryDAO.cpp \
    dao/ConfigDAO.cpp \
    dao/DatabaseDAO.cpp \
    dao/LoggingDAO.cpp \
    dao/PluginDAO.cpp \
    dao/PluginDirectoryDAO.cpp \
    event/EventBase.cpp \
    event/EventHandler.cpp \
    event/MainWindow.cpp \
    event/SplashScreen.cpp \
    event/SplashScreenWorkerThread.cpp \
    math/Integration.cpp \
    math/QuadraticFunction.cpp \
    model/ASIODriverListModel.cpp \
    model/AssetDirectoryListModel.cpp \
    model/ModelInitializer.cpp \
    model/PluginListModel.cpp \
    native/Native.cpp \
    native/WindowsLibraryRAII.cpp \
    ui/FontUtility.cpp \
    ui/Render.cpp \
    ui/UI.cpp \
    util/Literal.cpp

RESOURCES += \
    $$files(qmlproject/*) \
    $$files(qmlproject/imports/Musec) \
    Musec.qrc

LIBS += \
    sqlite3.lib \ # SQLite 3
    sndfile.lib \ # libsndfile
    AdvAPI32.lib \ # 注册表相关 API
    Shell32.lib \ # SHGetFolderPathW
    Mincore.lib \ # QueryInterruptTimePrecise
    ole32.lib \
    # VST3 SDK
    sdk.lib \
    base.lib \
    pluginterfaces.lib \
    sdk_common.lib \
    sdk_hosting.lib

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

RC_ICONS = qmlproject/images/Musec-image-2.ico

VERSION = 0.0.0.1
QMAKE_TARGET_COMPANY = "xris1658"
QMAKE_TARGET_DESCRIPTION = "Musec based on Qt 5.15.2 (MSVC 2019 64-bit)"
QMAKE_TARGET_COPYRIGHT = "Copyright xris1658 2021-2022. All rights reserved."
QMAKE_TARGET_PRODUCT = "Musec"
RC_LANG = 0x0804 # 简体中文（中国）

TRANSLATIONS += \
    Musec_zh_CN.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qmlproject/imports

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = qmlproject/imports

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    $$files(qmlproject/*) \
    $$files(qmlproject/imports/Musec)

#msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
#msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
