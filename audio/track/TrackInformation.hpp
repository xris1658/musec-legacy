#ifndef MUSEC_AUDIO_TRACK_TRACKINFORMATION
#define MUSEC_AUDIO_TRACK_TRACKINFORMATION

#include "base/Color.hpp"

#include <QString>

#include <array>

namespace Musec
{
namespace Audio
{
namespace Track
{
struct TrackInformation
{
    QString name_;
    Musec::Base::Color color_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_TRACKINFORMATION
