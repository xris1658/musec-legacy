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
template<typename SampleType>
using PluginSequence = std::vector<std::shared_ptr<Musec::Audio::Plugin::IPlugin<SampleType>>>;

template<typename SampleType>
using PluginSequenceIterator = typename PluginSequence<SampleType>::iterator;

template<typename SampleType>
using PluginSequenceConstIterator = typename PluginSequence<SampleType>::const_iterator;
}
}
}

#endif //MUSEC_AUDIO_TRACK_PLUGINSEQUENCE
