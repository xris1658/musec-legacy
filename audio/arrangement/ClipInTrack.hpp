#ifndef MUSEC_AUDIO_ARRANGEMENT_CLIPINTRACK
#define MUSEC_AUDIO_ARRANGEMENT_CLIPINTRACK

#include <memory>

namespace Musec
{
namespace Audio
{
namespace Arrangement
{
template<typename ClipType, typename TimePointType>
struct ClipInTrack
{
    TimePointType startTime_;
    TimePointType endTime_;
    std::shared_ptr<ClipType> clip_;
    TimePointType clipStartsAt_;
};
}
}
}

#endif //MUSEC_AUDIO_ARRANGEMENT_CLIPINTRACK
