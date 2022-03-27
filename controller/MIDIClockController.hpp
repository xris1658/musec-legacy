#ifndef MUSEC_CONTROLLER_MIDICLOCKCONTROLLER
#define MUSEC_CONTROLLER_MIDICLOCKCONTROLLER

#include "audio/engine/MIDIClock.hpp"

namespace Musec
{
namespace Controller
{
namespace MIDIClockController
{
constexpr std::size_t AppPPQ = 96;

Musec::Audio::Engine::MIDIClock<Musec::Controller::MIDIClockController::AppPPQ>& AppMIDIClock();

void updateArrangementPositionInUI(Musec::Audio::Base::TimePoint<Musec::Controller::MIDIClockController::AppPPQ> time);
}
}
}

#endif //MUSEC_CONTROLLER_MIDICLOCKCONTROLLER
