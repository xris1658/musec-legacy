#ifndef MUSEC_UI_UI
#define MUSEC_UI_UI

#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QTranslator>

namespace Musec
{
namespace UI
{
extern QQmlApplicationEngine* engine;

extern QQuickWindow* splashWindow;

extern QQuickWindow* mainWindow;

extern QQuickWindow* optionsWindow;

extern QObject* strings;

void setIcon(QWindow* window);

double refreshRateInHertz();
}
}

#endif // MUSEC_UI_UI
