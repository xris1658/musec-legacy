#include "UI.hpp"

namespace Musec::UI
{
QQmlApplicationEngine* engine = nullptr;
QQuickWindow* splashWindow = nullptr;
QQuickWindow* mainWindow = nullptr;
QQuickWindow* optionsWindow = nullptr;

void setIcon(QWindow* window)
{
    window->setIcon(mainWindow->icon());
}
}
