#ifndef MUSEC_AUDIO_MIDI_MIDIINPUTDEVICE
#define MUSEC_AUDIO_MIDI_MIDIINPUTDEVICE

#include <QString>
#include <QList>

#include <memory>

namespace Musec
{
namespace Native
{
class MIDIInputDeviceImpl;
}
}

namespace Musec
{
namespace Audio
{
namespace MIDI
{
using MIDIInputDeviceInfo = std::tuple<QString, QString>;

enum MIDIInputDeviceInfoField
{
    NameField = 0,
    IdField
};

QList<MIDIInputDeviceInfo> enumerateMIDIInputDeviceInfo();

class MIDIInputDevice
{
private:
    std::unique_ptr<Musec::Native::MIDIInputDeviceImpl> pImpl_;
    QString name_;
};
}
}
}

#endif //MUSEC_AUDIO_MIDI_MIDIINPUTDEVICE
