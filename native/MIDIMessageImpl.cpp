#include "MIDIMessageImpl.hpp"

#include "audio/midi/MIDIMessage.hpp"

#include <winrt/Windows.Devices.Midi.h>
#include <winrt/Windows.Storage.Streams.h>

namespace Musec::Native
{
using namespace Musec::Audio::MIDI;

MIDIMessageImpl::MIDIMessageImpl(winrt::Windows::Devices::Midi::IMidiMessage* message):
    message_(message)
{
}

MIDIMessageType MIDIMessageImpl::type() const
{
    using namespace Musec::Audio::MIDI;
    using namespace winrt::Windows::Devices::Midi;
    switch(auto type = message_->Type())
    {
    case MidiMessageType::None:
        return MIDIMessageType::NoneMessage;
    case MidiMessageType::NoteOn:
        return MIDIMessageType::NoteOnMessage;
    case MidiMessageType::NoteOff:
        return MIDIMessageType::NoteOffMessage;
    case MidiMessageType::PolyphonicKeyPressure:
        return MIDIMessageType::PolyKeyPressureMessage;
    case MidiMessageType::ControlChange:
        return MIDIMessageType::ControlChangeMessage;
    case MidiMessageType::ChannelPressure:
        return MIDIMessageType::ChannelPressureMessage;
    case MidiMessageType::PitchBendChange:
        return MIDIMessageType::PitchBendMessage;
    case MidiMessageType::ProgramChange:
        return MIDIMessageType::ProgramChangeMessage;
    case MidiMessageType::SystemExclusive:
        return MIDIMessageType::SystemExclusiveMessage;
    default:
        return MIDIMessageType::UnknownMessage;
    }
}

Musec::Audio::MIDI::SteadyClockDuration MIDIMessageImpl::timestamp() const
{
    return std::chrono::duration_cast<SteadyClockDuration>(message_->Timestamp());
}

const std::byte* MIDIMessageImpl::rawData() const
{
    // `IMidiMessage::RawData()` returns an `IBuffer` that is an incomplete type
    // forward-declared in `Windows.Devices.Midi.h`.
    // To actually use that, we have to `#include <winrt/Windows.Storage.Streams.h>` before
    // member functions of `IBuffer` can be used.
    return reinterpret_cast<std::byte*>(message_->RawData().data());
}

std::size_t MIDIMessageImpl::rawDataSize() const
{
    return message_->RawData().Length();
}

const winrt::Windows::Devices::Midi::IMidiMessage* MIDIMessageImpl::message() const
{
    return message_;
}
}