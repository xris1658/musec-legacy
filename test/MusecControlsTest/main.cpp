#include <QCoreApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    engine.addImportPath("qrc:/qmlproject/imports");
    QQuickWindow::setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);
    engine.load("qrc:/test/MusecControlsTest.qml");
    return app.exec();
}