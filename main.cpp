#include "controller/AppController.hpp"
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

// Qt
#include <QApplication>
#include <QCoreApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtGlobal>
#if QT_VERSION_MAJOR < 6
#include <QTextCodec>
#endif

int main(int argc, char* argv[]) try
{
    using namespace Musec::Event;
    using namespace Musec::UI;
    // By default, Musec in high DPI screen is a mess for now. So we have to do this to make the
    // GUI consistent on every device.
    // To make sure our GUI doesn't act weird in high DPI screens, QML code needs to be renovated.
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi);
    // QML code uses color dialog in Qt.Labs (ColorDialog: Qt.Labs.platform).
    // QML Runtime uses `QColorDialog` in Qt Widgets first.
    // As for the application, if Qt Widgets is not included, then the application will fall back
    // to a QML implementation called DefaultColorDialog.qml. If this fails, a message will
    // be printed in the console, informing that color dialog implementation is absent.
    // We'd like to use `QColorDialog` in Qt Widgets, so we brought Qt Widgets support in,
    // and replaced `QCoreApplication` with `QApplication`.
    // (#include <QColorDialog> is not needed.)
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
    QQmlApplicationEngine theEngine;
    engine = &theEngine;
    theEngine.addImportPath("qrc:/");
    theEngine.addImportPath("qrc:/qmlproject/imports");
    LoadQmlComponentListener& loadQmlComponentListener = LoadQmlComponentListener::instance();
    QObject::connect(&theEngine, &QQmlApplicationEngine::objectCreated,
                     &loadQmlComponentListener, &LoadQmlComponentListener::onObjectCreated);
    theEngine.load(QUrl("qrc:/qmlproject/SplashScreen.qml"));
    splashWindow = qobject_cast<QQuickWindow*>(theEngine.rootObjects()[0]);
    strings = splashWindow->property("strings").value<QObject*>();
    SplashScreen splashScreenEventHandler;
    MainWindowEvent mainWindowEvent(splashScreenEventHandler);
    mainWindowEvents = &mainWindowEvent;
    splashScreenEventHandler.onInitDialog();
    auto& promiseStart = Musec::Controller::loadTranslationPromiseStart();
    const auto& translation = promiseStart.get_future().get();
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