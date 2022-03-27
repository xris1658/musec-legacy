#ifndef MUSEC_AUDIO_TRACK_PLUGINSEQUENCE
#define MUSEC_AUDIO_TRACK_PLUGINSEQUENCE

#include "audio/plugin/IPlugin.hpp"
#include "audio/driver/ASIODriver.hpp"

#include <list>
#include <memory>
#include <vector>

namespace Musec
{
namespace Audio
{
namespace Track
{
struct PluginSequence
{
public:
    PluginSequence();
    void onMaximumBufferSizeChanged(long maximumBufferSize);
    long currentBufferSize;
    std::list<std::unique_ptr<Musec::Audio::Plugin::IPlugin<double>>> list_;
    std::vector<double> buffer_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_PLUGINSEQUENCE
