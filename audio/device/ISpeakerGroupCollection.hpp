#ifndef MUSEC_AUDIO_DEVICE_ISPEAKERGROUPCOLLECTION
#define MUSEC_AUDIO_DEVICE_ISPEAKERGROUPCOLLECTION

#include "audio/device/ISpeakerGroup.hpp"

#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Device
{
class ISpeakerGroupCollection
{
public:
    virtual std::uint8_t speakerGroupCount() const = 0;
    virtual const ISpeakerGroup& speakerGroupAt(std::uint8_t index) const = 0;
};
}
}
}

#endif //MUSEC_AUDIO_DEVICE_ISPEAKERGROUPCOLLECTION
