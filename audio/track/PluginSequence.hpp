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
template<typename SampleType>
using PluginSequence = std::list<std::shared_ptr<Musec::Audio::Plugin::IPlugin<SampleType>>>;
}
}
}

#endif //MUSEC_AUDIO_TRACK_PLUGINSEQUENCE
