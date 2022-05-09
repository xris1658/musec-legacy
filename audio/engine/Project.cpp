#include "Project.hpp"

#include "controller/AudioEngineController.hpp"
#include "audio/track/AudioTrack.hpp"
#include "audio/track/InstrumentTrack.hpp"
#include "audio/track/MIDITrack.hpp"
#include "ui/PluginWindow.hpp"

namespace Musec::Audio::Engine
{
Project::Project(int reserveTrackCount):
    audioBufferPool_(
        sizeof(float) * Musec::Controller::AudioEngineController::getMaxBlockSize() * 2, reserveTrackCount
    ),
    audioBuffer_(),
    masterTrackAudioBuffer_(audioBufferPool_.memoryBlockSize()),
    pluginGraph_(),
    tracks_(),
    masterTrack_(),
    gain_(), panning_(),
    trackMute_(), trackSolo_(), trackInvertPhase_(), trackArmRecording_(),
    pluginAndWindow_()
{
    audioBuffer_.reserve(reserveTrackCount);
    tracks_.reserve(reserveTrackCount);
    gain_.reserve(reserveTrackCount);
    trackMute_.reserve(reserveTrackCount);
    trackSolo_.reserve(reserveTrackCount);
    trackInvertPhase_.reserve(reserveTrackCount);
    trackArmRecording_.reserve(reserveTrackCount);
}

Project::~Project()
{
    pluginGraph_.clear();
    tracks_.clear();
    masterTrack_.setPluginSequences({});
    pluginAndWindow_.clear();
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
    {
        audioBuffer_.emplace_back(
            std::reinterpret_pointer_cast<float>(audioBufferPool_.lendMemoryBlock())
        );
        auto audioTrack = std::make_shared<Musec::Audio::Track::AudioTrack>();
        auto pluginSequences = audioTrack->getPluginSequences();
        pluginSequences.emplace_back();
        audioTrack->setPluginSequences(std::move(pluginSequences));
        trackPointer = audioTrack;
        break;
    }
    case Musec::Entities::CompleteTrack::TrackType::MIDITrack:
    {
        audioBuffer_.emplace_back(std::shared_ptr<float>(nullptr));
        trackPointer = std::make_shared<Musec::Audio::Track::MIDITrack>();
        break;
    }
    case Musec::Entities::CompleteTrack::TrackType::InstrumentTrack:
    {
        audioBuffer_.emplace_back(
            std::reinterpret_pointer_cast<float>(audioBufferPool_.lendMemoryBlock())
        );
        auto instrumentTrack = std::make_shared<Musec::Audio::Track::InstrumentTrack>();
        auto pluginSequences = instrumentTrack->getAudioEffectPluginSequences();
        pluginSequences.emplace_back();
        instrumentTrack->setAudioEffectPluginSequences(std::move(pluginSequences));
        trackPointer = instrumentTrack;
        break;
    }
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
    audioBuffer_.erase(audioBuffer_.begin() + index);
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
    auto iterator = pluginAndWindow_.find(plugin);
    if(iterator != pluginAndWindow_.end())
    {
        auto pluginWindow = iterator->second;
        Musec::UI::destroyPluginWindow(pluginWindow);
    }
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

const Musec::Audio::Track::AudioTrack& Project::masterTrack() const
{
    return masterTrack_;
}

void Project::process()
{
    masterTrackAudioBuffer_.init();
    std::size_t currentBlockSize = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    Musec::Audio::Base::AudioBufferViews<float> audioBufferViews(
        2, Musec::Audio::Base::AudioBufferView<float>()
    );
    Musec::Audio::Base::AudioBufferViews<float> masterTrackAudioBufferViews{
        Musec::Audio::Base::AudioBufferView<float>(reinterpret_cast<float*>(masterTrackAudioBuffer_.data()), currentBlockSize),
        Musec::Audio::Base::AudioBufferView<float>(reinterpret_cast<float*>(masterTrackAudioBuffer_.data() + currentBlockSize), currentBlockSize)
    };
    for(int i = 0; i < trackCount(); ++i)
    {
        auto& track = tracks_[i];
        audioBufferViews = {
            {audioBuffer_[i].get(), currentBlockSize},
            {audioBuffer_[i].get() + currentBlockSize, currentBlockSize}
        };
        audioBufferViews[0].init();
        audioBufferViews[1].init();
        if(track->trackType() == Musec::Audio::Track::TrackType::kInstrumentTrack)
        {
            auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(track);
            const auto& audioEffectPlugins = instrumentTrack->getAudioEffectPluginSequences()[0];
            for(const auto& audioEffect: audioEffectPlugins)
            {
                audioEffect->process(audioBufferViews, audioBufferViews);
            }
        }
        else if(track->trackType() == Musec::Audio::Track::TrackType::kAudioTrack)
        {
            auto audioTrack = std::static_pointer_cast<Musec::Audio::Track::AudioTrack>(track);
            const auto& plugins = audioTrack->getPluginSequences()[0];
            for(const auto& audioEffect: plugins)
            {
                audioEffect->process(audioBufferViews, audioBufferViews);
            }
        }
    }
    // const auto& masterTrackAudioEffects = masterTrack_.getPluginSequences()[0];
    // for(const auto& audioEffect: masterTrackAudioEffects)
    // {
    //     audioEffect->process(masterTrackAudioBufferViews, masterTrackAudioBufferViews);
    // }
}

void Project::clear()
{
    pluginGraph_.clear();
    audioBuffer_.clear();
    tracks_.clear();
    masterTrack_.clear();
    gain_.clear();
    panning_.clear();
    trackMute_.clear();
    trackSolo_.clear();
    trackInvertPhase_.clear();
    trackArmRecording_.clear();
    pluginAndWindow_.clear();
}
}
