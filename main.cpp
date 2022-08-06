#include "controller/AppController.hpp"
#include "controller/ConfigController.hpp"
#include "controller/LoggingController.hpp"
#include "entities/EntitiesInitializer.hpp"
#include "event/EventBase.hpp"
#include "event/ObjectCreateListener.hpp"
#include "event/MainWindowEvent.hpp"
#include "event/SplashScreen.hpp"
#include "i18n/I18N.hpp"
#include "model/ModelInitializer.hpp"
#include "native/Native.hpp"
#include "ui/FontUtility.hpp"
#include "ui/UI.hpp"

#include "ModuleConnector.hpp"

// Qt
#include <QApplication>
#include <QCoreApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QQmlApplicationEngine>
#if QT_VERSION_MAJOR < 6
#include <QTextCodec>
#endif
#include <QDebug>
#include <QUrl>
#include <QtGlobal>

int main(int argc, char* argv[]) try
{
    using namespace Musec::Event;
    using namespace Musec::UI;
    // 保证在不同缩放比例下不会出现界面缩放导致界面错乱的问题.
    // 日后需要对 QML 代码进行大量修改，以支持高 DPI.
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi);
    // 本程序的 QML 代码中使用 Qt.Labs 中的颜色对话框
    // (ColorDialog: Qt.Labs.platform)，在 QML Runtime 中
    // 使用 QColorDialog (Qt Widgets), 而生成应用程序时，
    // 若应用不带 Qt Widgets 模块, 则先尝试回退至 QML 自立实现
    // DefaultColorDialog.qml. 若失败, 则在控制台中提示用户
    // 没有添加颜色对话框的实现.
    // 为保证平台实现一致, 在项目文件中添加 Qt Widgets 组件
    // 支持, 并将 QGuiApplicaiton 改为其派生类 QApplication.
    // (不需要添加 <QColorDialog> 头文件.)
    QApplication app(argc, argv);
    Musec::Controller::LoggingController::AppLogger();
#if QT_VERSION_MAJOR < 6
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QFontDatabase fontDB;
#endif
    FontUtility::loadFonts();
    Musec::Entities::EntitiesInitializer::initialize();
    Musec::Model::ModelInitializer::initialize();
    Musec::connectFacility();
    QQmlApplicationEngine theEngine;
    engine = &theEngine;
    theEngine.addImportPath("qrc:/");
    theEngine.addImportPath("qrc:/qmlproject/imports");
    LoadQmlComponentListener& loadQmlComponentListener = LoadQmlComponentListener::instance();
    QObject::connect(&theEngine, &QQmlApplicationEngine::objectCreated,
                     &loadQmlComponentListener, &LoadQmlComponentListener::onObjectCreated);
    // 显示启动屏
    theEngine.load(QUrl("qrc:/qmlproject/SplashScreen.qml"));
    splashWindow = qobject_cast<QQuickWindow*>(theEngine.rootObjects()[0]);
    strings = splashWindow->property("strings").value<QObject*>(); assert(strings);
    SplashScreen splashScreenEventHandler;
    MainWindowEvent mainWindow(splashScreenEventHandler);
    mainWindowEvents = &mainWindow;
    splashScreenEventHandler.onInitDialog();
    // 等待启动屏工作线程通知，然后加载翻译
    auto& promiseStart = Musec::Controller::loadTranslationPromiseStart();
    const auto& translation = promiseStart.get_future().get();
    // 告知启动屏工作线程翻译加载完成
    auto& promiseEnd = Musec::Controller::loadTranslationPromiseEnd();
    QTranslator theTranslator;
    Musec::I18N::translator = &theTranslator;
    promiseEnd.set_value(Musec::I18N::loadTranslationFile(translation));
    auto ret = app.exec();
    return ret;
}
catch(Musec::Native::ErrorCodeType errorCode)
{
    qFatal("%u", errorCode);
    std::terminate();
}