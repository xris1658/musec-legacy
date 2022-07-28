#include "Project.hpp"

#include "audio/host/CLAPEvents.hpp"
#include "audio/host/MusecVST3Host.hpp"
#include "audio/plugin/VST2Plugin.hpp"
#include "audio/track/AudioTrack.hpp"
#include "audio/track/InstrumentTrack.hpp"
#include "audio/track/MIDITrack.hpp"
#include "controller/AudioEngineController.hpp"
#include "ui/PluginWindow.hpp"

#include <array>
#include <future>
#include <thread>

namespace Musec::Audio::Engine
{
Project::Project(int reserveTrackCount):
    mutex_(),
    audioBufferPool_(
        sizeof(float) * Musec::Controller::AudioEngineController::getMaxBlockSize() * 2, reserveTrackCount
    ),
    audioBuffer_(),
    masterTrackAudioBuffer_(audioBufferPool_.memoryBlockSize()),
    pluginGraph_(),
    tracks_(),
    masterTrack_(),
    masterTrackGain_(), masterTrackPanning_(),
    masterTrackControls_(),
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
    auto seq = masterTrack_.getPluginSequences();
    seq.emplace_back();
    masterTrack_.setPluginSequences(std::move(seq));
}

Project::~Project()
{
    this->clear();
}

std::size_t Project::trackCount() const noexcept
{
    return tracks_.size();
}

Project::TrackRef Project::at(std::size_t index)
{
    return TrackRef
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

Project::TrackRef Project::operator[](std::size_t index)
{
    return TrackRef
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

Project::MasterTrackRef Project::masterTrackRef()
{
    return MasterTrackRef
    {
        masterTrack_,
        masterTrackGain_,
        masterTrackPanning_,
        masterTrackMute(),
        masterTrackSolo(),
        masterTrackInvertPhase(),
        masterTrackArmRecording()
    };
}

void Project::insertTrack(std::size_t index, const Musec::Entities::CompleteTrack& track)
{
    std::lock_guard<std::mutex> lg(mutex_);
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
}

void Project::eraseTrack(std::size_t index)
{
    std::lock_guard<std::mutex> lg(mutex_);
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
        Musec::UI::destroyPluginWindow(iterator->second);
        pluginAndWindow_.erase(iterator);
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

const Musec::Base::FixedSizeMemoryBlock& Project::masterTrackAudioBuffer() const
{
    return masterTrackAudioBuffer_;
}

// 此函数在 ASIOBufferSwitch 回调中调用，因此无需调整优先级。
// 如果不由 ASIO 驱动调用，或许应该按照 ASIO 文档提到的，将优先级调成实时 + MMCSS。
void Project::process()
{
    std::lock_guard<std::mutex> lg(mutex_);
    auto& processContext = Musec::Audio::Host::MusecVST3Host::AppProcessContext();
    auto& eventTransport = Musec::Audio::Host::AppCLAPEventTransport();
    Musec::Controller::AudioEngineController::fillProcessContext(processContext);
    Musec::Controller::AudioEngineController::fillEventTransport(eventTransport);
    masterTrackAudioBuffer_.init();
    std::size_t currentBlockSize = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    std::array<Musec::Audio::Base::AudioBufferView<float>, 2> audioBufferViews;
    // Musec::Audio::Base::AudioBufferViews<float> audioBufferViews(
    //     2, Musec::Audio::Base::AudioBufferView<float>()
    // );
    Musec::Audio::Base::AudioBufferViews<float> masterTrackAudioBufferViews {
        Musec::Audio::Base::AudioBufferView<float>(reinterpret_cast<float*>(masterTrackAudioBuffer_.data()), currentBlockSize),
        Musec::Audio::Base::AudioBufferView<float>(reinterpret_cast<float*>(masterTrackAudioBuffer_.data()) + currentBlockSize, currentBlockSize)
    };
    for (int i = 0; i < trackCount(); ++i)
    {
        auto& track = tracks_[i];
        audioBufferViews[0] = {audioBuffer_[i].get(),                    currentBlockSize};
        audioBufferViews[1] = {audioBuffer_[i].get() + currentBlockSize, currentBlockSize};
       for(auto& bufferView: audioBufferViews)
       {
           bufferView.init();
       }
        if (track->trackType() == Musec::Audio::Track::TrackType::kInstrumentTrack)
        {
            auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(track);
            const auto& instrument = instrumentTrack->getInstrument();
            if (instrument)
            {
                instrument->process(nullptr, 0, audioBufferViews.data(), 2);
            }
            const auto& audioEffectPlugins = instrumentTrack->getAudioEffectPluginSequences()[0];
            for (const auto& audioEffect: audioEffectPlugins)
            {
                audioEffect->process(audioBufferViews.data(), 2, audioBufferViews.data(), 2);
            }
        }
        else if (track->trackType() == Musec::Audio::Track::TrackType::kAudioTrack)
        {
            auto audioTrack = std::static_pointer_cast<Musec::Audio::Track::AudioTrack>(track);
            const auto& plugins = audioTrack->getPluginSequences()[0];
            for (const auto& audioEffect: plugins)
            {
                audioEffect->process(audioBufferViews.data(), 2, audioBufferViews.data(), 2);
            }
        }
        // 按帧操作。
        // 逐列操纵，因此缓冲区较大时可能出现缓存未命中的问题，日后需要优化。
        for (auto j = 0; j < currentBlockSize; ++j)
        {
            for (auto k = 0; k < masterTrackAudioBufferViews.size(); ++k)
            {
                if(!trackMute_[i])
                {
                    if(trackInvertPhase_[i])
                    {
                        masterTrackAudioBufferViews[k][j] -= audioBufferViews[k][j];
                    }
                    else
                    {
                        masterTrackAudioBufferViews[k][j] += audioBufferViews[k][j];
                    }
                }
            }
        }
    }
    // ll. 246-247
    for (auto j = 0; j < currentBlockSize; ++j)
    {
        for (auto i = 0; i < masterTrackAudioBufferViews.size(); ++i)
        {
            if(masterTrackMute())
            {
                masterTrackAudioBufferViews[i][j] = 0.0;
            }
            else
            {
                if(masterTrackInvertPhase())
                {
                    masterTrackAudioBufferViews[i][j] *= -1;
                }
            }
        }
    }
    const auto& masterTrackAudioEffects = masterTrack_.getPluginSequences()[0];
    for(const auto& audioEffect: masterTrackAudioEffects)
    {
        audioEffect->process(masterTrackAudioBufferViews.data(), 2, masterTrackAudioBufferViews.data(), 2);
    }
}

void Project::clear()
{
    std::lock_guard<std::mutex> lg(mutex_);
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

std::bitset<4>::reference Project::masterTrackMute()
{
    return masterTrackControls_[0];
}

std::bitset<4>::reference Project::masterTrackSolo()
{
    return masterTrackControls_[1];
}

std::bitset<4>::reference Project::masterTrackInvertPhase()
{
    return masterTrackControls_[2];
}

std::bitset<4>::reference Project::masterTrackArmRecording()
{
    return masterTrackControls_[3];
}
}
