#ifndef MUSEC_NATIVE_MIDIINPUTDEVICEIMPL
#define MUSEC_NATIVE_MIDIINPUTDEVICEIMPL

#include "audio/midi/MIDIInputDevice.hpp"

#include <winrt/Windows.Devices.Midi.h>

#include <QList>

namespace Musec
{
namespace Native
{
using namespace Musec::Audio::MIDI;

QList<MIDIInputDeviceInfo> enumerateMIDIInputDeviceInfo();

class MIDIInputDeviceImpl
{
public:
    MIDIInputDeviceImpl();
    explicit MIDIInputDeviceImpl(const MIDIInputDeviceInfo& info);
    MIDIInputDeviceImpl(const MIDIInputDeviceImpl&) = delete;
    MIDIInputDeviceImpl& operator=(const MIDIInputDeviceImpl&) = delete;
    MIDIInputDeviceImpl(MIDIInputDeviceImpl&&) noexcept;
    MIDIInputDeviceImpl& operator=(MIDIInputDeviceImpl&&) noexcept;
    ~MIDIInputDeviceImpl();
public:
    QString deviceId() const;
private:
    winrt::Windows::Devices::Midi::MidiInPort midiInPort_;
};
}
}

#endif //MUSEC_NATIVE_MIDIINPUTDEVICEIMPL
