#include "Project.hpp"

#include "audio/host/CLAPEvents.hpp"
#include "audio/host/VST3Host.hpp"
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
    trackTypes_(),
    masterTrack_(),
    masterTrackGain_(1.0), masterTrackPanning_(),
    masterTrackControls_(),
    gain_(), panning_(),
    trackMute_(), trackSolo_(), trackInvertPhase_(), trackArmRecording_(), trackMonoDownMix_(),
    pluginAndWindow_(),
    vst2PluginPool_(),
    panLaw_(Musec::Audio::Util::DefaultPanLaw),
    compensate_(true),
    getScale_(&Musec::Audio::Util::fromPanning<float, Musec::Audio::Util::PanLaw::kN3>),
    getCompensate_(&Musec::Audio::Util::compensateFromPanLaw<float, Musec::Audio::Util::PanLaw::kN3>)
{
    static std::thread thread([this]() { vst2PluginIdleFunc(); });
    if(thread.joinable())
    {
        thread.detach();
    }
    audioBuffer_.reserve(reserveTrackCount);
    tracks_.reserve(reserveTrackCount);
    trackTypes_.reserve(reserveTrackCount);
    gain_.reserve(reserveTrackCount);
    trackMute_.reserve(reserveTrackCount);
    trackSolo_.reserve(reserveTrackCount);
    trackInvertPhase_.reserve(reserveTrackCount);
    trackArmRecording_.reserve(reserveTrackCount);
    trackMonoDownMix_.reserve(reserveTrackCount);
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
        trackArmRecording_[index],
        trackMonoDownMix_[index]
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
        trackArmRecording_[index],
        trackMonoDownMix_[index]
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
        masterTrackArmRecording(),
        masterTrackMonoDownMix()
    };
}

void Project::insertTrack(std::size_t index, const Musec::Entities::CompleteTrack& track)
{
    std::lock_guard<std::mutex> lg(mutex_);
    std::shared_ptr<Musec::Audio::Track::ITrack> trackPointer = nullptr;
    auto entityTrackType = track.getTrackType();
    Musec::Audio::Track::TrackType trackType;
    switch(entityTrackType)
    {
    case Musec::Entities::CompleteTrack::TrackType::AudioTrack:
    {
        trackType = Musec::Audio::Track::TrackType::kAudioTrack;
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
        trackType = Musec::Audio::Track::TrackType::kMIDITrack;
        audioBuffer_.emplace_back(std::shared_ptr<float>(nullptr));
        trackPointer = std::make_shared<Musec::Audio::Track::MIDITrack>();
        break;
    }
    case Musec::Entities::CompleteTrack::TrackType::InstrumentTrack:
    {
        trackType = Musec::Audio::Track::TrackType::kInstrumentTrack;
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
    trackTypes_.insert(trackTypes_.begin() + index, trackType);
    gain_.insert(gain_.begin() + index, track.getGain());
    panning_.insert(panning_.begin() + index, track.getPanning());
    trackMute_.insert(trackMute_.begin(), track.isTrackMute());
    trackSolo_.insert(trackSolo_.begin(), track.isTrackSolo());
    trackInvertPhase_.insert(trackInvertPhase_.begin(), track.isTrackInvertPhase());
    trackArmRecording_.insert(trackArmRecording_.begin(), track.isTrackArmRecording());
    trackMonoDownMix_.insert(trackMonoDownMix_.begin(), track.isTrackMonoDownMix());
}

void Project::eraseTrack(std::size_t index)
{
    std::lock_guard<std::mutex> lg(mutex_);
    audioBuffer_.erase(audioBuffer_.begin() + index);
    tracks_.erase(tracks_.begin() + index);
    trackTypes_.erase(trackTypes_.begin() + index);
    gain_.erase(gain_.begin() + index);
    panning_.erase(panning_.begin() + index);
    trackMute_.erase(trackMute_.begin() + index);
    trackSolo_.erase(trackSolo_.begin() + index);
    trackInvertPhase_.erase(trackInvertPhase_.begin() + index);
    trackArmRecording_.erase(trackArmRecording_.begin() + index);
    trackMonoDownMix_.erase(trackMonoDownMix_.begin() + index);
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

void Project::addVST2Plugin(AEffect* plugin)
{
    vst2PluginPool_.addPlugin(plugin);
}

void Project::removeVST2Plugin(AEffect* plugin)
{
    vst2PluginPool_.removePlugin(plugin);
}

const Musec::Base::FixedSizeMemoryBlock& Project::masterTrackAudioBuffer() const
{
    return masterTrackAudioBuffer_;
}

// If the application uses ASIO, then this function is called in the ASIOBufferSwitch callback,
// thus setting the priority of this thread is not needed (because it's done by the driver).
// If not, the priority of this thread might be set to realtime + MMCSS.
void Project::process()
{
    std::lock_guard<std::mutex> lg(mutex_);
    Musec::Controller::AudioEngineController::fillPluginContext();
    masterTrackAudioBuffer_.init();
    std::size_t currentBlockSize = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    std::array<Musec::Audio::Base::AudioBufferView<float>, 2> audioBufferViews;
    std::array<Musec::Audio::Base::AudioBufferView<float>, 2> masterTrackAudioBufferViews {
        Musec::Audio::Base::AudioBufferView<float>(reinterpret_cast<float*>(masterTrackAudioBuffer_.data()), currentBlockSize),
        Musec::Audio::Base::AudioBufferView<float>(reinterpret_cast<float*>(masterTrackAudioBuffer_.data()) + currentBlockSize, currentBlockSize)
    };
    for (int i = 0; i < trackCount(); ++i)
    {
        auto& track = tracks_[i];
        audioBufferViews[0] = {audioBuffer_[i].get(),                    currentBlockSize};
        audioBufferViews[1] = {audioBuffer_[i].get() + currentBlockSize, currentBlockSize};
        // TODO: Add audio arm recording here
        for(auto& bufferView: audioBufferViews)
        {
            bufferView.init();
        }
        if(trackTypes_[i] == Musec::Audio::Track::TrackType::kInstrumentTrack)
        {
            // The `if` expression is used to get the type of the track, so it's okay not to use `dynamic_cast`
            auto instrumentTrack = reinterpret_cast<Musec::Audio::Track::InstrumentTrack*>(track.get());
            const auto& instrument = instrumentTrack->getInstrument();
            if (instrument && (instrument->processing()))
            {
                instrument->process(nullptr, 0, audioBufferViews.data(), 2);
            }
            const auto& audioEffectPlugins = instrumentTrack->getAudioEffectPluginSequences()[0];
            for (const auto& audioEffect: audioEffectPlugins)
            {
                if(audioEffect->processing())
                {
                    audioEffect->process(audioBufferViews.data(), 2, audioBufferViews.data(), 2);
                }
            }
        }
        else if (trackTypes_[i] == Musec::Audio::Track::TrackType::kAudioTrack)
        {
            auto audioTrack = reinterpret_cast<Musec::Audio::Track::AudioTrack*>(track.get());
            const auto& plugins = audioTrack->getPluginSequences()[0];
            for (const auto& audioEffect: plugins)
            {
                if(!audioEffect->getBypass())
                {
                    audioEffect->process(audioBufferViews.data(), 2, audioBufferViews.data(), 2);
                }
            }
        }
        // Operate frame-wise, so traverse by column is used.
        // The compiler might optimize this to traversing by row. This is a common case of
        // single-thread optimization.
        // If not optimized, then it might miss the CPU cache.
        for (auto j = 0; j < currentBlockSize; ++j)
        {
            if(trackMonoDownMix_[i])
            {
                audioBufferViews[0][j] = audioBufferViews[1][j] =
                    (audioBufferViews[0][j] + audioBufferViews[1][j]) * 0.5;
            }
            auto scales = getScale_(panning_[i]);
            if(compensate_)
            {
                scales.left *= getCompensate_();
                scales.right *= getCompensate_();
            }
            audioBufferViews[0][j] *= scales.left;
            audioBufferViews[1][j] *= scales.right;
            for (auto k = 0; k < masterTrackAudioBufferViews.size(); ++k)
            {
                // The buffer is filled with 0 before this.
                if(!trackMute_[i])
                {
                    audioBufferViews[k][j] *= gain_[i];
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
    const auto& masterTrackAudioEffects = masterTrack_.getPluginSequences()[0];
    for(const auto& audioEffect: masterTrackAudioEffects)
    {
        if(audioEffect->processing())
        {
            audioEffect->process(masterTrackAudioBufferViews.data(), 2, masterTrackAudioBufferViews.data(), 2);
        }
    }
    for (auto j = 0; j < currentBlockSize; ++j)
    {
        if(masterTrackMonoDownMix())
        {
            masterTrackAudioBufferViews[0][j] = masterTrackAudioBufferViews[1][j] =
                (masterTrackAudioBufferViews[0][j] + masterTrackAudioBufferViews[1][j]) * 0.5;
        }
        auto scales = getScale_(masterTrackPanning_);
        if(compensate_)
        {
            scales.left *= getCompensate_();
            scales.right *= getCompensate_();
        }
        masterTrackAudioBufferViews[0][j] *= scales.left;
        masterTrackAudioBufferViews[1][j] *= scales.right;
        for (auto& masterTrackAudioBufferView : masterTrackAudioBufferViews)
        {
            if(masterTrackMute())
            {
                masterTrackAudioBufferView[j] = 0.0;
            }
            else
            {
                masterTrackAudioBufferView[j] *= masterTrackGain_;
                if(masterTrackInvertPhase())
                {
                    masterTrackAudioBufferView[j] *= -1;
                }
            }
        }
    }
}

void Project::clear()
{
    std::lock_guard<std::mutex> lg(mutex_);
    vst2PluginIdleFuncRunning_ = false;
    pluginGraph_.clear();
    audioBuffer_.clear();
    tracks_.clear();
    trackTypes_.clear();
    masterTrack_.clear();
    gain_.clear();
    panning_.clear();
    trackMute_.clear();
    trackSolo_.clear();
    trackInvertPhase_.clear();
    trackArmRecording_.clear();
    pluginAndWindow_.clear();
}

Musec::Audio::Util::PanLaw Project::getPanLaw()
{
    return panLaw_;
}

bool Project::isCompensate()
{
    return compensate_;
}

void Project::setPanLaw(Musec::Audio::Util::PanLaw panLaw, bool compensate)
{
    using namespace Musec::Audio::Util;
    panLaw_ = panLaw;
    compensate_ = compensate;
    if(panLaw_ == PanLaw::k0)
    {
        getScale_ = &fromPanning<float, PanLaw::k0>;
        getCompensate_ = &compensateFromPanLaw<float, PanLaw::k0>;
    }
    else if(panLaw_ == PanLaw::kN6)
    {
        getScale_ = &fromPanning<float, PanLaw::kN6>;
        getCompensate_ = &compensateFromPanLaw<float, PanLaw::kN6>;
    }
    else/* if(panLaw_ == PanLaw::kN3)*/
    {
        getScale_ = &fromPanning<float, PanLaw::kN3>;
        getCompensate_ = &compensateFromPanLaw<float, PanLaw::kN3>;
    }
}

Project::MasterTrackControlType::reference Project::masterTrackMute()
{
    return masterTrackControls_[0];
}

Project::MasterTrackControlType::reference Project::masterTrackSolo()
{
    return masterTrackControls_[1];
}

Project::MasterTrackControlType::reference Project::masterTrackInvertPhase()
{
    return masterTrackControls_[2];
}

Project::MasterTrackControlType::reference Project::masterTrackArmRecording()
{
    return masterTrackControls_[3];
}

Project::MasterTrackControlType::reference Project::masterTrackMonoDownMix()
{
    return masterTrackControls_[4];
}

void Project::vst2PluginIdleFunc()
{
    while(vst2PluginIdleFuncRunning_)
    {
        vst2PluginPool_.doIdle();
        // This time is determined by watching the dB meter display of Serum, a VST2 synthesizer.
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
}
