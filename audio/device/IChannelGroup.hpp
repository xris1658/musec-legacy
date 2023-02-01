#ifndef MUSEC_AUDIO_DEVICE_ICHANNELGROUP
#define MUSEC_AUDIO_DEVICE_ICHANNELGROUP

#include "audio/base/Channel.hpp"
#include "native/CompilerSpecific.hpp"

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Device
{
MUSEC_INTERFACE IChannelGroup
{
public:
    virtual QString name() const = 0;
    virtual std::uint8_t channelCount() const = 0;
    virtual Musec::Audio::Base::ChannelGroupType type() const = 0;
    virtual Musec::Audio::Base::ChannelType speakerAt(std::uint8_t index) const = 0;
    virtual QString speakerNameAt(std::uint8_t index) const = 0;
    virtual bool isMain() const = 0;
protected:
    static int channelCount(Musec::Audio::Base::ChannelGroupType channelGroupType);
    static bool isChannelInGroup(Musec::Audio::Base::ChannelType channelType, Musec::Audio::Base::ChannelGroupType channelGroupType);
};
}
}
}

#endif //MUSEC_AUDIO_DEVICE_ICHANNELGROUP
