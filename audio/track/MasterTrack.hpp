#ifndef MUSEC_AUDIO_TRACK_MASTERTRACK
#define MUSEC_AUDIO_TRACK_MASTERTRACK

#include "audio/track/AudioTrack.hpp"

namespace Musec
{
namespace Audio
{
namespace Track
{
template<typename SampleType>
class MasterTrack: public AudioTrack<SampleType>
{
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_MASTERTRACK
