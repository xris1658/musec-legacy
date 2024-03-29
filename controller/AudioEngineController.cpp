#include "AudioEngineController.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "audio/host/CLAPEvents.hpp"
#include "audio/host/VST3Host.hpp"
#include "audio/plugin/CLAPUtils.hpp"
#include "controller/MIDIClockController.hpp"
#include "native/Native.hpp"

namespace Musec::Controller::AudioEngineController
{
Musec::Audio::Engine::Project& AppProject()
{
    static Musec::Audio::Engine::Project ret(0);
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
    return 0;
}

long getMaxBlockSize()
{
    if(Musec::Audio::Driver::AppASIODriver())
    {
        auto ret = Musec::Audio::Driver::getBufferSize().maximumBufferSize;
        return ret;
    }
    return 0; // Maximum sample count of FlexASIO from what I can tell
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
    return 0;
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
    return midiClock.tempoAutomation().empty()?
        midiClock.tempo():
        midiClock.tempoAutomation()(midiClock.getPosition().count());
}

void fillPluginContext()
{
    auto currentTempo = getCurrentTempo();
    auto& processContext = Musec::Audio::Host::VST3Host::AppProcessContext();
    auto& eventTransport = Musec::Audio::Host::AppCLAPEventTransport();
    using Steinberg::Vst::ProcessContext;
    processContext.state = ProcessContext::StatesAndFlags::kSystemTimeValid
        | ProcessContext::StatesAndFlags::kTempoValid;
    if(Musec::Controller::MIDIClockController::AppMIDIClock().playing())
    {
        processContext.state |= ProcessContext::StatesAndFlags::kPlaying;
    }
    processContext.sampleRate = getCurrentSampleRate();
    processContext.systemTime = Musec::Native::currentTimeValueInNanosecond();
    processContext.tempo = currentTempo;
    Musec::Audio::Plugin::fillEventHeader(eventTransport, CLAP_EVENT_TRANSPORT);
    auto& header = eventTransport.header;
    header.time = 0;
    header.flags = 0;
    eventTransport.flags =
        CLAP_TRANSPORT_HAS_TEMPO;
    eventTransport.tempo = currentTempo;
}

double getCpuUsage()
{
    return Musec::Audio::Driver::cpuUsage;
}
}
