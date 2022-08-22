#ifndef MUSEC_AUDIO_PLUGIN_VST2PLUGINSHELLPLUGINID
#define MUSEC_AUDIO_PLUGIN_VST2PLUGINSHELLPLUGINID

#include "audio/plugin/VST2PluginForward.hpp"

namespace Musec
{
namespace Audio
{
namespace Plugin
{
class VST2PluginShellPluginId
{
private:
    VST2PluginShellPluginId();
public:
    VST2PluginShellPluginId(const VST2PluginShellPluginId&) = delete;
    VST2PluginShellPluginId(VST2PluginShellPluginId&&) = delete;
    ~VST2PluginShellPluginId() noexcept;
public:
    static VST2PluginShellPluginId& instance();
public:
    VstInt32 getId() const;
    bool idShouldBeZero() const;
public:
    AEffect* getShellPlugin(VstInt32 id,
        bool idShouldBeZero,
        Musec::Audio::Plugin::VST2PluginEntryProc pluginEntryProc);
private:
    VstInt32 id_ = 0;
    bool idShouldBeZero_ = true;
    std::mutex mutex_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST2PLUGINSHELLPLUGINID
