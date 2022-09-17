#include "MIDIClockController.hpp"

#include "event/EventBase.hpp"
#include "event/EventHandler.hpp"
#include "ui/UI.hpp"

namespace Musec::Controller::MIDIClockController
{
Musec::Audio::Engine::MIDIClock<Musec::Controller::MIDIClockController::AppPPQ>& AppMIDIClock()
{
    static Musec::Audio::Engine::MIDIClock<Musec::Controller::MIDIClockController::AppPPQ> ret(updateArrangementPositionInUI);
//    std::thread& clockThread = ret.clockThread();
//    if(clockThread.joinable())
//    {
//        clockThread.detach();
//    }
    return ret;
}

void updateArrangementPositionInUI(Musec::Audio::Base::TimePoint<Musec::Controller::MIDIClockController::AppPPQ> time)
{
    Musec::Event::eventHandler->updateArrangementPosition(time.count());
}
}
