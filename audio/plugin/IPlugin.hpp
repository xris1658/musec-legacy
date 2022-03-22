#ifndef MUSEC_AUDIO_PLUGIN_IPLUGIN
#define MUSEC_AUDIO_PLUGIN_IPLUGIN

#include <array>
#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
template<typename SampleType>
class IPlugin
{
public:
    virtual ~IPlugin() {}
public:
    virtual bool initialize(double sampleRate, std::int32_t maxSampleCount) = 0;
    virtual bool uninitialize() = 0;
    virtual bool startProcessing() = 0;
    virtual bool stopProcessing() = 0;
    virtual void process(std::array<SampleType*, 2> input, std::array<SampleType*, 2> output) = 0;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_IPLUGIN
