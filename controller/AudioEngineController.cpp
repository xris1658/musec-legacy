#include "AudioEngineController.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "audio/plugin/CLAPUtils.hpp"
#include "controller/MIDIClockController.hpp"
#include "native/Native.hpp"

namespace Musec::Controller::AudioEngineController
{
Musec::Audio::Engine::Project& AppProject()
{
    static Musec::Audio::Engine::Project ret;
    return ret;
}

Musec::Model::TrackListModel& AppTrackListModel()
{
    static Musec::Model::TrackListModel ret;
    return ret;
}

void initializeFacility()
{
    Musec::Audio::Driver::AppASIODriver();
    AppProject();
    AppTrackListModel();
}

void insertTrack(std::size_t index, const Entities::CompleteTrack& track)
{
    AppTrackListModel().insertTrack(static_cast<int>(index), track);
}

void appendTrack(const Entities::CompleteTrack& track)
{
    insertTrack(AppProject().trackCount(), track);
}

double getCurrentSampleRate()
{
    if(Musec::Audio::Driver::AppASIODriver())
    {
        return Musec::Audio::Driver::getSampleRate();
    }
    return 44100.0;
}

long getMaxBlockSize()
{
    if(Musec::Audio::Driver::AppASIODriver())
    {
        auto ret = Musec::Audio::Driver::getBufferSize().maximumBufferSize;
        if(ret != 0xcccccccc && ret != 0)
        return ret;
    }
    return 88200; // Maximum sample count of FlexASIO from what I can tell
}

long getCurrentBlockSize()
{
    if(Musec::Audio::Driver::AppASIODriver())
    {
        auto ret = Musec::Audio::Driver::getBufferSize().preferredBufferSize;
        if(ret != 0)
        {
            return ret;
        }
        else
        {
            return Musec::Audio::Driver::getBufferSize().maximumBufferSize;
        }
    }
    return 512;
}

long getInputLatency()
{
    if(Musec::Audio::Driver::AppASIODriver())
    {
        return Musec::Audio::Driver::getLatency().inputLatency;
    }
    return getCurrentBlockSize();
}

long getOutputLatency()
{
    if(Musec::Audio::Driver::AppASIODriver())
    {
        return Musec::Audio::Driver::getLatency().outputLatency;
    }
    return getCurrentBlockSize();
}

double getCurrentTempo()
{
    auto& midiClock = Musec::Controller::MIDIClockController::AppMIDIClock();
    return midiClock.tempoAutomation()(midiClock.getPosition());
}

void fillProcessContext(Steinberg::Vst::ProcessContext& processContext)
{
    using namespace Steinberg;
    using namespace Steinberg::Vst;
    processContext.state =
        ProcessContext::StatesAndFlags::kSystemTimeValid
        | ProcessContext::StatesAndFlags::kTempoValid;
    processContext.sampleRate = getCurrentSampleRate();
    processContext.systemTime = Musec::Native::currentTimeInNanosecond();
    processContext.tempo = getCurrentTempo();
}

void fillEventTransport(clap_event_transport& eventTransport)
{
    Musec::Audio::Plugin::fillEventHeader(eventTransport, CLAP_EVENT_TRANSPORT);
    auto& header = eventTransport.header;
    header.time = 0;
    header.flags = 0;
    eventTransport.flags =
        CLAP_TRANSPORT_HAS_TEMPO;
    eventTransport.tempo = getCurrentTempo();
}

double getCpuUsage()
{
    return Musec::Audio::Driver::cpuUsage;
}
}
