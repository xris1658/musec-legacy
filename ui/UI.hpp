#ifndef MUSEC_UI_UI
#define MUSEC_UI_UI

#include "event/EventHandler.hpp"

#include "event/MainWindow.hpp"

#include <QQuickWindow>
#include <QQmlApplicationEngine>

namespace Musec
{
namespace UI
{
extern QQmlApplicationEngine* engine;
// 启动屏
extern QQuickWindow* splashWindow;
// 主窗口
extern QQuickWindow* mainWindow;
// 选项窗口
extern QQuickWindow* optionsWindow;
}
}

#endif // MUSEC_UI_UI
