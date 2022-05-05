#ifndef MUSEC_AUDIO_TRACK_AUDIOTRACK
#define MUSEC_AUDIO_TRACK_AUDIOTRACK

#include "audio/track/PluginSequence.hpp"
#include "audio/track/ITrack.hpp"

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Track
{
class AudioTrack: public Musec::Audio::Track::ITrack
{
public:
    AudioTrack();
    ~AudioTrack() override;
public:
    const std::vector<Musec::Audio::Track::PluginSequence<float>>& getPluginSequences() const;
    void setPluginSequences(const std::vector<Musec::Audio::Track::PluginSequence<float>>& pluginSequences);
    void setPluginSequences(std::vector<Musec::Audio::Track::PluginSequence<float>>&& pluginSequences);
public:
    virtual TrackType trackType() const override;

    void clear();

private:
    std::vector<Musec::Audio::Track::PluginSequence<float>> pluginSequences_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_AUDIOTRACK
