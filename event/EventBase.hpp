#ifndef MUSEC_EVENT_EVENTBASE
#define MUSEC_EVENT_EVENTBASE

#include "event/EventHandler.hpp"

#include "event/MainWindow.hpp"

#include <QObject>

namespace Musec
{
namespace Event
{
// EventBridge.qml
extern QObject* eventBridge;
extern Musec::Event::EventHandler* eventHandler;

// 主窗口的事件收发
extern MainWindow* mainWindowEvents;

Musec::Event::EventHandler& singletonEventHandler(QObject* eventBridge);
}
}

#endif // MUSEC_EVENT_EVENTBASE
