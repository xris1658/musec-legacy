#ifndef MUSEC_AUDIO_TRACK_ITRACK
#define MUSEC_AUDIO_TRACK_ITRACK

#include "audio/track/TrackInformation.hpp"

#include <QObject>

namespace Musec
{
namespace Audio
{
namespace Track
{
enum TrackType
{
    kAudioTrack,
    kMIDITrack,
    kInstrumentTrack
};

class ITrack
{
public:
    virtual ~ITrack() noexcept = default;
public:
    const TrackInformation& getTrackInformation() const
    {
        return trackInformation_;
    }
    void setTrackInformation(const TrackInformation& trackInformation)
    {
        trackInformation_ = trackInformation;
    }
    void setTrackInformation(TrackInformation&& trackInformation)
    {
        trackInformation_ = std::move(trackInformation);
    }
public:
    virtual TrackType trackType() const = 0;
protected:
    TrackInformation trackInformation_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_ITRACK
