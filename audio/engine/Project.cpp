#include "Project.hpp"

#include "controller/AudioEngineController.hpp"
#include "audio/track/AudioTrack.hpp"
#include "audio/track/InstrumentTrack.hpp"
#include "audio/track/MIDITrack.hpp"

namespace Musec::Audio::Engine
{
Project::Project()
{

}

Project::~Project() noexcept
{

}

std::size_t Project::trackCount() const noexcept
{
    return tracks_.size();
}

Project::CompleteTrackRef Project::at(std::size_t index)
{
    return CompleteTrackRef
    {
        tracks_.at(index),
        gain_[index],
        panning_[index],
        trackMute_[index],
        trackSolo_[index],
        trackInvertPhase_[index],
        trackArmRecording_[index]
    };
}

Project::CompleteTrackRef Project::operator[](std::size_t index)
{
    return CompleteTrackRef
    {
        tracks_[index],
        gain_[index],
        panning_[index],
        trackMute_[index],
        trackSolo_[index],
        trackInvertPhase_[index],
        trackArmRecording_[index]
    };
}

void Project::insertTrack(std::size_t index, const Musec::Entities::CompleteTrack& track)
{
    std::shared_ptr<Musec::Audio::Track::ITrack> trackPointer = nullptr;
    switch(track.getTrackType())
    {
    case Musec::Entities::CompleteTrack::TrackType::AudioTrack:
        trackPointer = std::make_shared<Musec::Audio::Track::AudioTrack>();
        break;
    case Musec::Entities::CompleteTrack::TrackType::MIDITrack:
        trackPointer = std::make_shared<Musec::Audio::Track::MIDITrack>();
        break;
    case Musec::Entities::CompleteTrack::TrackType::InstrumentTrack:
        trackPointer = std::make_shared<Musec::Audio::Track::InstrumentTrack>();
        break;
    default:
        break;
    }
    if(!trackPointer)
    {
        return;
    }
    trackPointer->setTrackInformation(Musec::Audio::Track::TrackInformation {track.getTrackName(), track.getTrackColor(), track.getHeight()});
    tracks_.insert(tracks_.begin() + index, std::move(trackPointer));
    gain_.insert(gain_.begin() + index, track.getGain());
    panning_.insert(panning_.begin() + index, track.getPanning());
    trackMute_.insert(trackMute_.begin(), track.isTrackMute());
    trackSolo_.insert(trackSolo_.begin(), track.isTrackSolo());
    trackInvertPhase_.insert(trackInvertPhase_.begin(), track.isTrackInvertPhase());
    trackArmRecording_.insert(trackArmRecording_.begin(), track.isTrackArmRecording());
    return;
}

void Project::eraseTrack(std::size_t index)
{
    tracks_.erase(tracks_.begin() + index);
    gain_.erase(gain_.begin() + index);
    panning_.erase(panning_.begin() + index);
    trackMute_.erase(trackMute_.begin() + index);
    trackSolo_.erase(trackSolo_.begin() + index);
    trackInvertPhase_.erase(trackInvertPhase_.begin() + index);
    trackArmRecording_.erase(trackArmRecording_.begin() + index);
}

void Project::addPluginWindowMapping(void* plugin, QWindow* window)
{
    pluginAndWindow_[plugin] = window;
}

void Project::removePluginWindowMapping(void* plugin)
{
    pluginAndWindow_.erase(plugin);
}

void Project::setPluginWindowSize(void* plugin, int width, int height)
{
    auto iterator = pluginAndWindow_.find(plugin);
    if(iterator != pluginAndWindow_.end())
    {
        auto pluginWindow = iterator->second;
        pluginWindow->setWidth(width);
        pluginWindow->setHeight(height);
    }
}
}
