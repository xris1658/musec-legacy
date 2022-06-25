#include "UI.hpp"

#include <future>
#include <string>

namespace Musec::UI
{
QQmlApplicationEngine* engine = nullptr;
QQuickWindow* splashWindow = nullptr;
QQuickWindow* mainWindow = nullptr;
QQuickWindow* optionsWindow = nullptr;
QObject* strings = nullptr;
QTranslator* translator = nullptr;

void setIcon(QWindow* window)
{
    window->setIcon(mainWindow->icon());
}

bool loadTranslation(const QString& name)
{
    QString translationFileName("Musec_%1.qm");
    return translator->load(translationFileName.arg(name), QCoreApplication::applicationDirPath())
        && QCoreApplication::installTranslator(translator);
}
}
