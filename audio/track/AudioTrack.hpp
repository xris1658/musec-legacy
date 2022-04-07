#ifndef MUSEC_AUDIO_TRACK_AUDIOTRACK
#define MUSEC_AUDIO_TRACK_AUDIOTRACK

#include "audio/track/ITrack.hpp"
#include "audio/track/PluginSequence.hpp"
#include "audio/track/TrackInformation.hpp"

#include <list>

namespace Musec
{
namespace Audio
{
namespace Track
{
template<typename SampleType>
class AudioTrack:
    public Musec::Audio::Track::ITrack
{
public:
    void process() override;
private:
    Musec::Audio::Track::TrackInformation trackInformation_;
    std::list<PluginSequence<SampleType>> pluginSequences_;
    std::list<Musec::Audio::Base::AudioBufferView<SampleType>> bufferViews_;
};

extern template class AudioTrack<float>;
extern template class AudioTrack<double>;
}
}
}

#endif //MUSEC_AUDIO_TRACK_AUDIOTRACK
