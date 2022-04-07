#include "AudioEngineController.hpp"

#include "audio/driver/ASIODriver.hpp"

namespace Musec::Controller::AudioEngineController
{
Musec::Audio::Track::MasterTrack<double>& AppMasterTrack()
{
    static Musec::Audio::Track::MasterTrack<double> ret;
    return ret;
}

void initializeFacility()
{
    Musec::Audio::Driver::AppASIODriver();
    AppMasterTrack();
}
}
