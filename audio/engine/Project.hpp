#ifndef MUSEC_AUDIO_ENGINE_PROJECT
#define MUSEC_AUDIO_ENGINE_PROJECT

#include "audio/engine/Graph.hpp"
#include "audio/plugin/IPlugin.hpp"
#include "audio/plugin/VST2PluginPool.hpp"
#include "audio/track/AudioTrack.hpp"
#include "audio/track/ITrack.hpp"
#include "audio/util/Util.hpp"
#include "base/FixedSizeMemoryPool.hpp"
#include "entities/CompleteTrack.hpp"

#include <QWindow>

#include <bitset>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

namespace Musec
{
namespace Audio
{
namespace Engine
{
constexpr int initialReserveTrackCount = 16;
class Project
{
    using MasterTrackControlType = std::bitset<5>;
public:
    struct TrackRef
    {
        std::shared_ptr<Musec::Audio::Track::ITrack>& track;
        double& gain;
        double& panning;
        std::vector<bool>::reference trackMute;
        std::vector<bool>::reference trackSolo;
        std::vector<bool>::reference trackInvertPhase;
        std::vector<bool>::reference trackArmRecording;
        std::vector<bool>::reference trackMonoDownMix;
    };
    struct MasterTrackRef
    {
        Musec::Audio::Track::AudioTrack& masterTrack;
        double& gain;
        double& panning;
        MasterTrackControlType::reference trackMute;
        MasterTrackControlType::reference trackSolo;
        MasterTrackControlType::reference trackInvertPhase;
        MasterTrackControlType::reference trackArmRecording;
        MasterTrackControlType::reference trackMonoDownMix;
    };
public:
    Project(std::size_t audioBufferSizeInFrame, int reserveTrackCount = initialReserveTrackCount);
    Project(const Project&) = delete;
    Project(Project&&) = delete;
    ~Project();
public:
    std::size_t trackCount() const noexcept;
    TrackRef at(std::size_t index);
    TrackRef operator[](std::size_t index);
    MasterTrackRef masterTrackRef();
public:
    void insertTrack(std::size_t index, const Musec::Entities::CompleteTrack& track);
    void eraseTrack(std::size_t index);
    void clear();
public:
    void addPluginWindowMapping(void* plugin, QWindow* window);
    void removePluginWindowMapping(void* plugin);
    void setPluginWindowSize(void* plugin, int width, int height);
public:
    void addVST2Plugin(AEffect* plugin);
    void removeVST2Plugin(AEffect* plugin);
public:
    void process();
    const Musec::Base::FixedSizeMemoryBlock& masterTrackAudioBuffer() const;
public:
    void reallocateAudioBuffer(std::size_t audioBufferSizeInFrame);
private:
    MasterTrackControlType::reference masterTrackMute();
    MasterTrackControlType::reference masterTrackSolo();
    MasterTrackControlType::reference masterTrackInvertPhase();
    MasterTrackControlType::reference masterTrackArmRecording();
    MasterTrackControlType::reference masterTrackMonoDownMix();
private:
    void vst2PluginIdleFunc();
private:
    bool vst2PluginIdleFuncRunning_ = true;
    std::mutex mutex_;
    Musec::Base::FixedSizeMemoryPool audioBufferPool_;
    std::vector<std::shared_ptr<float>> audioBuffer_;
    Musec::Base::FixedSizeMemoryBlock masterTrackAudioBuffer_;
    Musec::Audio::Engine::Graph<std::shared_ptr<Musec::Audio::Plugin::IPlugin>> pluginGraph_;
    std::vector<std::shared_ptr<Musec::Audio::Track::ITrack>> tracks_;
    std::vector<Musec::Audio::Track::TrackType> trackTypes_;
    Musec::Audio::Track::AudioTrack masterTrack_;
    double masterTrackGain_;
    double masterTrackPanning_;
    MasterTrackControlType masterTrackControls_;
    std::vector<double> gain_;
    std::vector<double> panning_;
    std::vector<bool> trackMute_;
    std::vector<bool> trackSolo_;
    std::vector<bool> trackInvertPhase_;
    std::vector<bool> trackArmRecording_;
    std::vector<bool> trackMonoDownMix_;
    std::map<void*, QWindow*> pluginAndWindow_;
    Musec::Audio::Plugin::VST2PluginPool vst2PluginPool_;
};
}
}
}

#endif //MUSEC_AUDIO_ENGINE_PROJECT
