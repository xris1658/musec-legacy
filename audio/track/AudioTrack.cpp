#include "AudioTrack.hpp"

namespace Musec::Audio::Track
{
void AudioTrack::process()
{
    std::array<double*, 2> buffer = {buffer_.data(), buffer_.data() + currentBufferSize};
    auto& list = PluginSequence::list_;
    for(auto& item: list)
    {
        item->process(buffer, buffer);
    }
}
}