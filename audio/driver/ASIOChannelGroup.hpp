#ifndef MUSEC_AUDIO_DRIVER_ASIOCHANNELGROUP
#define MUSEC_AUDIO_DRIVER_ASIOCHANNELGROUP

namespace Musec
{
namespace Audio
{
namespace Driver
{
class ASIOChannelGroup
{
public:
    enum Direction: std::uint8_t
    {
        Input,
        Output
    };
    static constexpr long InvalidChannelIndex = -1;
public:
    ASIOChannelGroup();
    ASIOChannelGroup(Direction direction, long channelCount);
    ASIOChannelGroup(const ASIOChannelGroup& rhs);
    ASIOChannelGroup(ASIOChannelGroup&& rhs) noexcept;
    ASIOChannelGroup& operator=(const ASIOChannelGroup& rhs);
    ASIOChannelGroup& operator=(ASIOChannelGroup&& rhs) noexcept;
    ~ASIOChannelGroup() noexcept;
public:
    Direction direction() const;
    long channelCount() const;
    long channelIndex(long index) const;
    void setChannelIndex(long index, long channelIndex);
public:
    void swap(ASIOChannelGroup& rhs) noexcept;
private:
    Direction direction_ = Input;
    long channelCount_ = 0;
    long* channelIndices_ = nullptr;
};
}
}
}

#endif //MUSEC_AUDIO_DRIVER_ASIOCHANNELGROUP
