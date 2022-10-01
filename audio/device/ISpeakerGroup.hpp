#ifndef MUSEC_AUDIO_DEVICE_ISPEAKERGROUP
#define MUSEC_AUDIO_DEVICE_ISPEAKERGROUP

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Device
{
// TODO
enum SpeakerType
{
    SpeakerCustom = -1,
    SpeakerInvalid,
    SpeakerMono,
    SpeakerLeft,
    SpeakerRight
};

// TODO
enum SpeakerGroupType
{
    SpeakerGroupCustom = -1,
    SpeakerGroupMono,
    SpeakerGroupStereo
};

class ISpeakerGroup
{
public:
    virtual QString name() const = 0;
    virtual std::uint8_t speakerCount() const = 0;
    virtual SpeakerGroupType type() const = 0;
    virtual SpeakerType speakerAt(std::uint8_t index) const = 0;
    virtual QString speakerNameAt(std::uint8_t index) const = 0;
    virtual bool isMain() const = 0;
};
}
}
}

#endif //MUSEC_AUDIO_DEVICE_ISPEAKERGROUP
