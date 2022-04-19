#ifndef MUSEC_AUDIO_TRACK_TRACKINFORMATION
#define MUSEC_AUDIO_TRACK_TRACKINFORMATION

#include "base/Color.hpp"

#include <QColor>
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
    QString name;
    QColor color;
    int height;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_TRACKINFORMATION
