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
public:
    const std::vector<Musec::Audio::Track::PluginSequence<double>>& getPluginSequences() const;
    void setPluginSequences(const std::vector<Musec::Audio::Track::PluginSequence<double>>& pluginSequences);
    void setPluginSequences(std::vector<Musec::Audio::Track::PluginSequence<double>>&& pluginSequences);
public:
    virtual TrackType trackType() const override;
private:
    std::vector<Musec::Audio::Track::PluginSequence<double>> pluginSequences_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_AUDIOTRACK
