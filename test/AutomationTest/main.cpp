#include <audio/base/Automation.hpp>
#include <model/AutomationModel.hpp>
#include <model/ModelInitializer.hpp>

#include <QCoreApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include <utility>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Musec::Model::ModelInitializer::initialize();
    QQmlApplicationEngine engine;
    Musec::Audio::Base::Automation automation;
    automation.insertPoint({0, 0.0, 0.0});
    automation.insertPoint({50, 1.0, 0.0});
    automation.insertPoint({100, 0.0, 0.0});
    automation.insertPoint({100, 1.0, 0.0}, 0);
    automation.insertPoint({150, 1.0, 1.0});
    automation.insertPoint({200, 0.0, 1.0});
    automation.insertPoint({250, 1.0, -1.0});
    automation.insertPoint({300, 0.0, -1.0});
    Musec::Model::AutomationModel automationModel;
    automationModel.setAutomation(std::move(automation));
    QQuickWindow* window = nullptr;
    engine.addImportPath("qrc:/");
    engine.addImportPath("qrc:/qmlproject/imports");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        [&engine, pWindow = &window, &automationModel](QObject* object, const QUrl& url)
        {
            if(object == nullptr)
            {
                return;
            }
            *pWindow = qobject_cast<QQuickWindow*>(engine.rootObjects()[0]);
            (*pWindow)->setProperty("automationModel", QVariant::fromValue(&automationModel));
        });
    engine.load("qrc:/test/AutomationTest.qml");
    return app.exec();
}