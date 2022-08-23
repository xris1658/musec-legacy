#ifndef MUSEC_EVENT_EVENTBASE
#define MUSEC_EVENT_EVENTBASE

#include "event/EventHandler.hpp"
#include "event/MainWindowEvent.hpp"

#include <QObject>

namespace Musec
{
namespace Event
{
// EventBridge.qml
extern QObject* eventBridge;
extern Musec::Event::EventHandler* eventHandler;

extern MainWindowEvent* mainWindowEvents;
}
}

#endif // MUSEC_EVENT_EVENTBASE
