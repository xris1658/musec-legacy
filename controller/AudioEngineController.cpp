#include "AudioEngineController.hpp"

#include "audio/driver/ASIODriver.hpp"
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

Musec::Audio::Host::VST3ComponentHandler AppVST3ComponentHandler()
{
    return Musec::Audio::Host::VST3ComponentHandler::instance();
}

void initializeFacility()
{
    Musec::Audio::Driver::AppASIODriver();
    AppProject();
    AppTrackListModel();
    AppVST3ComponentHandler();
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
    double ret;
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
    return 88200; // 我记得 FlexASIO 的最大采样数是这个
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
}
