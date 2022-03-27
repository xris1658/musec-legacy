#ifndef MUSEC_AUDIO_TRACK_ITRACK
#define MUSEC_AUDIO_TRACK_ITRACK

namespace Musec
{
namespace Audio
{
namespace Track
{
class ITrack
{
public:
    virtual ~ITrack() {}
public:
    virtual void process() = 0;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_ITRACK
