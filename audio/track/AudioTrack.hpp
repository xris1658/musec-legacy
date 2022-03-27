#ifndef MUSEC_AUDIO_TRACK_AUDIOTRACK
#define MUSEC_AUDIO_TRACK_AUDIOTRACK

#include "audio/track/ITrack.hpp"
#include "audio/track/PluginSequence.hpp"
#include "audio/track/TrackInformation.hpp"

namespace Musec
{
namespace Audio
{
namespace Track
{
class AudioTrack:
    public Musec::Audio::Track::PluginSequence,
    public Musec::Audio::Track::ITrack
{
public:
    void process() override;
private:
    Musec::Audio::Track::TrackInformation trackInformation_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_AUDIOTRACK
