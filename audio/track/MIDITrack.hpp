#ifndef MUSEC_AUDIO_TRACK_MIDITRACK
#define MUSEC_AUDIO_TRACK_MIDITRACK

#include "audio/track/PluginSequence.hpp"
#include "audio/track/ITrack.hpp"

#include <list>

namespace Musec
{
namespace Audio
{
namespace Track
{
class MIDITrack: public Musec::Audio::Track::ITrack
{
public:
    const std::list<Musec::Audio::Track::PluginSequence<double>>& getPluginSequences() const;
    void setPluginSequences(const std::list<Musec::Audio::Track::PluginSequence<double>>& pluginSequences);
    void setPluginSequences(std::list<Musec::Audio::Track::PluginSequence<double>>&& pluginSequences);

    TrackType trackType() const override;

private:
    std::list<Musec::Audio::Track::PluginSequence<double>> pluginSequences_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_MIDITRACK
