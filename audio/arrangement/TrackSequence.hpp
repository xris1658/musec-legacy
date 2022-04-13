#ifndef MUSEC_AUDIO_ARRANGEMENT_TRACKSEQUENCE
#define MUSEC_AUDIO_ARRANGEMENT_TRACKSEQUENCE

#include "ClipInTrack.hpp"

#include "audio/media/AudioSequence.hpp"
#include "audio/media/MIDISequence.hpp"

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Arrangement
{
template<typename ClipType, typename TimePointType>
using TrackSequence = std::vector<Musec::Audio::Arrangement::ClipInTrack<ClipType, TimePointType>>;

using AudioTrackSequence = TrackSequence<Musec::Audio::Media::AudioSequence, double>;
using MIDITrackSequence = TrackSequence<Musec::Audio::Media::MIDISequence, std::int64_t>;

template<typename ClipType, typename TimePointType>
std::shared_ptr<ClipType> getClipAtTime(const TrackSequence<ClipType, TimePointType>& trackSequence, TimePointType timePoint);
}
}
}

#endif //MUSEC_AUDIO_ARRANGEMENT_TRACKSEQUENCE
