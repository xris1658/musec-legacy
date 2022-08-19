#include "UI.hpp"

#include <QScreen>

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

double refreshRateInHertz()
{
    return mainWindow->screen()->refreshRate();
}
}
