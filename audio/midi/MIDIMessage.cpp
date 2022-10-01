#include "MIDIMessage.hpp"

#include "native/MIDIMessageImpl.hpp"

namespace Musec::Audio::MIDI
{
MIDIMessageType MIDIMessage::type() const
{
    return message_->type();
}

SteadyClockDuration MIDIMessage::timestamp() const
{
    return message_->timestamp();
}

const std::byte* MIDIMessage::rawData() const
{
    return message_->rawData();
}

std::size_t MIDIMessage::rawDataSize() const
{
    return message_->rawDataSize();
}

const Musec::Native::MIDIMessageImpl* MIDIMessage::message() const
{
    return message_;
}
}