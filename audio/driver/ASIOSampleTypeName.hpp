#ifndef MUSEC_AUDIO_DRIVER_ASIOSAMPLETYPENAME
#define MUSEC_AUDIO_DRIVER_ASIOSAMPLETYPENAME

#include "audio/driver/ASIODriverForward.hpp"

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Driver
{
QString getASIOSampleTypeName(ASIOSampleType type);
}
}
}

#endif //MUSEC_AUDIO_DRIVER_ASIOSAMPLETYPENAME
