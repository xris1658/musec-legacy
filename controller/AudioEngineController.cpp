#include "AudioEngineController.hpp"

#include "audio/driver/ASIODriver.hpp"

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
        return Musec::Audio::Driver::getASIODriverStreamInfo().sampleRate;
    }
    return 44100.0;
}

long getMaxBlockSize()
{
    if(Musec::Audio::Driver::AppASIODriver())
    {
        return Musec::Audio::Driver::getASIODriverStreamInfo().maximumBufferSize;
    }
    return 88200; // 我记得 FlexASIO 的最大采样数是这个
}

long getCurrentBlockSize()
{
    if(Musec::Audio::Driver::AppASIODriver())
    {
        return Musec::Audio::Driver::getASIODriverStreamInfo().preferredBufferSize;
    }
    return 512;
}
}
