#ifndef MUSEC_CONTROLLER_AUDIOENGINECONTROLLER
#define MUSEC_CONTROLLER_AUDIOENGINECONTROLLER

#include "audio/base/AudioBufferView.hpp"
#include "audio/engine/Project.hpp"
#include "entities/CompleteTrack.hpp"
#include "model/TrackListModel.hpp"

#include <pluginterfaces/vst/ivstprocesscontext.h>

#include <clap/events.h>

namespace Musec
{
namespace Controller
{
namespace AudioEngineController
{
extern double cpuUsage;

Musec::Audio::Engine::Project& AppProject();
Musec::Model::TrackListModel& AppTrackListModel();
void initializeFacility();
void insertTrack(std::size_t index, const Musec::Entities::CompleteTrack& track);
void appendTrack(const Musec::Entities::CompleteTrack& track);
double getCurrentSampleRate();
long getMaxBlockSize();
long getCurrentBlockSize();
long getInputLatency();
long getOutputLatency();
double getCurrentTempo();
void fillProcessContext(Steinberg::Vst::ProcessContext& processContext);
void fillEventTransport(clap_event_transport& eventTransport);

template<typename SampleType>
Musec::Audio::Base::AudioBufferView<SampleType> dummyBufferView()
{
    static std::vector<SampleType> dummyBuffer(getMaxBlockSize());
    return Musec::Audio::Base::AudioBufferView<SampleType>(dummyBuffer.data(), dummyBuffer.size());
}
}
}
}

#endif //MUSEC_CONTROLLER_AUDIOENGINECONTROLLER
