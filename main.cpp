// 项目头文件
#include "controller/AppController.hpp"
#include "entities/EntitiesInitializer.hpp"
#include "event/EventBase.hpp"
#include "event/ObjectCreateListener.hpp"
#include "event/MainWindow.hpp"
#include "event/SplashScreen.hpp"
#include "model/ModelInitializer.hpp"
#include "ui/FontUtility.hpp"
#include "ui/UI.hpp"

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

#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    using namespace Musec::Event;
    using namespace Musec::UI;
    // 保证在不同缩放比例下不会出现界面缩放导致界面错乱的问题.
    // 日后需要对 QML 代码进行大量修改，以支持高 DPI.
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi);
    // 本程序的 QML 代码中使用 Qt.Labs 中的颜色对话框
    // (ColorDialog: Qt.Labs.platform)，在 QML Runtime 中
    // 使用 QColorDialog (Qt Widgets), 而生成应用程序时，
    // 若应用不带 Qt Widgets 模块, 则回退至 QML 自立实现
    // DefaultColorDialog.qml.
    // 为保证平台实现一致, 在项目文件中添加 Qt Widgets 组件
    // 支持, 并将 QGuiApplicaiton 改为其派生类 QApplication.
    // (不需要添加 <QColorDialog> 头文件.)
    QApplication app(argc, argv);
    Musec::Controller::createLogger();
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
    auto list = theEngine.importPathList();
    std::vector<std::string> listString;
    listString.reserve(list.size());
    for(auto& path: list)
    {
        listString.emplace_back(path.toStdString());
    }
    LoadQmlComponentListener& loadQmlComponentListener = LoadQmlComponentListener::instance();
    QObject::connect(&theEngine, &QQmlApplicationEngine::objectCreated,
                     &loadQmlComponentListener, &LoadQmlComponentListener::onObjectCreated);
    // 显示启动屏
    theEngine.load(QUrl("qrc:///qmlproject/SplashScreen.qml"));
    splashWindow = qobject_cast<QQuickWindow*>(theEngine.rootObjects()[0]);
    SplashScreen splashScreenEventHandler;
    MainWindow mainWindow(splashScreenEventHandler);
    mainWindowEvents = &mainWindow;
    splashScreenEventHandler.onInitDialog();
    auto ret = app.exec();
    return ret;
}
