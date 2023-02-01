#ifndef MUSEC_AUDIO_DEVICE_ICHANNELGROUPCOLLECTION
#define MUSEC_AUDIO_DEVICE_ICHANNELGROUPCOLLECTION

#include "audio/device/IChannelGroup.hpp"
#include "native/CompilerSpecific.hpp"

#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Device
{
MUSEC_INTERFACE IChannelGroupCollection
{
public:
    virtual std::uint8_t channelGroupCount() const = 0;
    virtual const IChannelGroup& channelGroupAt(std::uint8_t index) const = 0;
};
}
}
}

#endif //MUSEC_AUDIO_DEVICE_ICHANNELGROUPCOLLECTION
