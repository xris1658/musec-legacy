#include "EventBase.hpp"

namespace Musec::Event
{
QObject* eventBridge = nullptr;
EventHandler* eventHandler = nullptr;

MainWindow* mainWindowEvents = nullptr;

EventHandler& singletonEventHandler(QObject* eventBridge)
{
    static EventHandler eventHandler(eventBridge);
    return eventHandler;
}
}
