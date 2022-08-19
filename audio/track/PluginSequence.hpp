#ifndef MUSEC_AUDIO_TRACK_PLUGINSEQUENCE
#define MUSEC_AUDIO_TRACK_PLUGINSEQUENCE

#include "audio/plugin/IPlugin.hpp"
#include "audio/driver/ASIODriver.hpp"

#include <memory>
#include <vector>

namespace Musec
{
namespace Audio
{
namespace Track
{
using PluginSequence = std::vector<std::shared_ptr<Musec::Audio::Plugin::IPlugin>>;

using PluginSequenceIterator = typename PluginSequence::iterator;

using PluginSequenceConstIterator = typename PluginSequence::const_iterator;
}
}
}

#endif //MUSEC_AUDIO_TRACK_PLUGINSEQUENCE
