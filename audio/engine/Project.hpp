#ifndef MUSEC_AUDIO_ENGINE_PROJECT
#define MUSEC_AUDIO_ENGINE_PROJECT

#include "audio/arrangement/TrackSequence.hpp"
#include "audio/engine/Graph.hpp"
#include "audio/plugin/IPlugin.hpp"
#include "audio/track/AudioTrack.hpp"
#include "audio/track/ITrack.hpp"
#include "base/FixedSizeMemoryPool.hpp"
#include "entities/CompleteTrack.hpp"

#include <QWindow>

#include <bitset>
#include <map>
#include <memory>
#include <mutex>
#include <variant>
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
    };
    struct MasterTrackRef
    {
        Musec::Audio::Track::AudioTrack& masterTrack;
        double& gain;
        double& panning;
        std::bitset<4>::reference trackMute;
        std::bitset<4>::reference trackSolo;
        std::bitset<4>::reference trackInvertPhase;
        std::bitset<4>::reference trackArmRecording;
    };
public:
    Project(int reserveTrackCount = initialReserveTrackCount);
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
    void process();
    const Musec::Base::FixedSizeMemoryBlock& masterTrackAudioBuffer() const;
private:
    std::bitset<4>::reference masterTrackMute();
    std::bitset<4>::reference masterTrackSolo();
    std::bitset<4>::reference masterTrackInvertPhase();
    std::bitset<4>::reference masterTrackArmRecording();
private:
    std::mutex mutex_;
    Musec::Base::FixedSizeMemoryPool audioBufferPool_;
    std::vector<std::shared_ptr<float>> audioBuffer_;
    Musec::Base::FixedSizeMemoryBlock masterTrackAudioBuffer_;
    Musec::Audio::Engine::Graph<std::shared_ptr<Musec::Audio::Plugin::IPlugin<float>>> pluginGraph_;
    std::vector<std::shared_ptr<Musec::Audio::Track::ITrack>> tracks_;
    std::vector<Musec::Audio::Track::TrackType> trackTypes_;
    Musec::Audio::Track::AudioTrack masterTrack_;
    double masterTrackGain_;
    double masterTrackPanning_;
    std::bitset<4> masterTrackControls_;
    std::vector<double> gain_;
    std::vector<double> panning_;
    std::vector<bool> trackMute_;
    std::vector<bool> trackSolo_;
    std::vector<bool> trackInvertPhase_;
    std::vector<bool> trackArmRecording_;
    std::map<void*, QWindow*> pluginAndWindow_;
};
}
}
}

#endif //MUSEC_AUDIO_ENGINE_PROJECT
