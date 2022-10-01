#include "MIDIInputDevice.hpp"

#include "native/MIDIInputDeviceImpl.hpp"

namespace Musec::Audio::MIDI
{
QList<MIDIInputDeviceInfo> enumerateMIDIInputDeviceInfo()
{
    return Musec::Native::enumerateMIDIInputDeviceInfo();
}
}