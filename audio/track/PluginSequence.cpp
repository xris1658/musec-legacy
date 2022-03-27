#include "PluginSequence.hpp"

namespace Musec::Audio::Track
{
PluginSequence::PluginSequence()
{
    auto& driver = Musec::Audio::Driver::AppASIODriver();
    if(driver)
    {
        onMaximumBufferSizeChanged(currentBufferSize);
    }
}

void PluginSequence::onMaximumBufferSizeChanged(long maximumBufferSize)
{
    if(maximumBufferSize > currentBufferSize)
    {
        buffer_ = std::vector<double>(maximumBufferSize * 2);
        currentBufferSize = maximumBufferSize;
    }
}
}