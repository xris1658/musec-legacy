#include "IChannelGroup.hpp"

namespace Musec::Audio::Device
{
int IChannelGroup::channelCount(Musec::Audio::Base::ChannelGroupType channelGroupType)
{
    switch(channelGroupType)
    {
    case Musec::Audio::Base::ChannelGroupType::NoChannel:
        return 0;
    case Musec::Audio::Base::ChannelGroupType::Mono:
        return 1;
    case Musec::Audio::Base::ChannelGroupType::Stereo:
        return 2;
    default:
        return Musec::Audio::Base::InvalidChannelCount;
    }
}

bool IChannelGroup::isChannelInGroup(Musec::Audio::Base::ChannelType channelType, Musec::Audio::Base::ChannelGroupType channelGroupType)
{
    switch(channelGroupType)
    {
    case Musec::Audio::Base::ChannelGroupType::Mono:
        return channelType == Musec::Audio::Base::ChannelType::Center;
    case Musec::Audio::Base::ChannelGroupType::Stereo:
        return channelType == Musec::Audio::Base::ChannelType::Left
               || channelType == Musec::Audio::Base::ChannelType::Right;
    default:
        return false;
    }
}
}