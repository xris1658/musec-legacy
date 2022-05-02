#ifndef MUSEC_CONTROLLER_AUDIOENGINECONTROLLER
#define MUSEC_CONTROLLER_AUDIOENGINECONTROLLER

#include "audio/engine/Project.hpp"
#include "audio/host/VST3ComponentHandler.hpp"
#include "entities/CompleteTrack.hpp"
#include "model/TrackListModel.hpp"

namespace Musec
{
namespace Controller
{
namespace AudioEngineController
{
Musec::Audio::Engine::Project& AppProject();
Musec::Model::TrackListModel& AppTrackListModel();
Musec::Audio::Host::VST3ComponentHandler AppVST3ComponentHandler();
void initializeFacility();
void insertTrack(std::size_t index, const Musec::Entities::CompleteTrack& track);
void appendTrack(const Musec::Entities::CompleteTrack& track);
double getCurrentSampleRate();
}
}
}

#endif //MUSEC_CONTROLLER_AUDIOENGINECONTROLLER
