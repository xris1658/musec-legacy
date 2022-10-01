#ifndef MUSEC_NATIVE_MIDIMESSAGEIMPL
#define MUSEC_NATIVE_MIDIMESSAGEIMPL

#include "audio/midi/MIDIMessage.hpp"

namespace winrt::Windows::Devices::Midi
{
struct IMidiMessage;
}

namespace Musec
{
namespace Native
{
using namespace Musec::Audio::MIDI;
using winrt::Windows::Devices::Midi::IMidiMessage;

class MIDIMessageImpl
{
public:
    MIDIMessageImpl(IMidiMessage* message);
public:
    Musec::Audio::MIDI::MIDIMessageType type() const;
    Musec::Audio::MIDI::SteadyClockDuration timestamp() const;
    const std::byte* rawData() const;
    std::size_t rawDataSize() const;
    const IMidiMessage* message() const;
private:
    winrt::Windows::Devices::Midi::IMidiMessage* message_;
};
}
}

#endif //MUSEC_NATIVE_MIDIMESSAGEIMPL
