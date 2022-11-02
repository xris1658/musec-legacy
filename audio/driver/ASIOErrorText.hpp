#ifndef MUSEC_AUDIO_DRIVER_ASIOERRORTEXT
#define MUSEC_AUDIO_DRIVER_ASIOERRORTEXT

#include "audio/driver/ASIODriverForward.hpp"

namespace Musec
{
namespace Audio
{
namespace Driver
{
QString getASIOErrorText(ASIOError error);
}
}
}

#endif //MUSEC_AUDIO_DRIVER_ASIOERRORTEXT
