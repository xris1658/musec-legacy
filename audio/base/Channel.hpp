#ifndef MUSEC_AUDIO_BASE_CHANNEL
#define MUSEC_AUDIO_BASE_CHANNEL

namespace Musec
{
namespace Audio
{
namespace Base
{
enum class ChannelType
{
    Custom = -2,
    Invalid,
    Center,
    Left,
    Right
};

enum class ChannelGroupType
{
    Custom = -1,
    NoChannel,
    Mono,
    Stereo
};

constexpr int InvalidChannelCount = -1;
}
}
}

#endif //MUSEC_AUDIO_BASE_CHANNEL
