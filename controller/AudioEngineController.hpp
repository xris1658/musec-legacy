#ifndef MUSEC_CONTROLLER_AUDIOENGINECONTROLLER
#define MUSEC_CONTROLLER_AUDIOENGINECONTROLLER

#include "audio/track/MasterTrack.hpp"

namespace Musec
{
namespace Controller
{
namespace AudioEngineController
{
Musec::Audio::Track::MasterTrack<double>& AppMasterTrack();

void initializeFacility();
}
}
}

#endif //MUSEC_CONTROLLER_AUDIOENGINECONTROLLER
