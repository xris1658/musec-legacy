#ifndef MUSEC_AUDIO_PLUGIN_VST2PLUGINPOOL
#define MUSEC_AUDIO_PLUGIN_VST2PLUGINPOOL

#include <pluginterfaces/vst2.x/aeffect.h>

#include <atomic>
#include <vector>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
class VST2PluginPool
{
public:
    VST2PluginPool();
    VST2PluginPool(const VST2PluginPool&) = delete;
    VST2PluginPool(VST2PluginPool&&);
    ~VST2PluginPool();
public:
    void addPlugin(AEffect* plugin);
    void removePlugin(AEffect* plugin);
    void clear();
    void doIdle();
private:
    std::vector<AEffect*> plugins_;
    std::atomic_bool lock_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST2PLUGINPOOL
