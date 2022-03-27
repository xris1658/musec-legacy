#include "AudioEngineController.hpp"

namespace Musec::Controller::AudioEngineController
{
Musec::Audio::Track::MasterTrack& AppMasterTrack()
{
    static Musec::Audio::Track::MasterTrack ret;
    return ret;
}

void initializeFacility()
{
    Musec::Audio::Driver::AppASIODriver();
    AppMasterTrack();
}
}