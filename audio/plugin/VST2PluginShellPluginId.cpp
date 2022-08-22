#include "VST2PluginShellPluginId.hpp"

#include "audio/plugin/VST2Plugin.hpp"

namespace Musec::Audio::Plugin
{
VST2PluginShellPluginId::VST2PluginShellPluginId()
{
    //
}

VST2PluginShellPluginId::~VST2PluginShellPluginId() noexcept
{
    //
}

VST2PluginShellPluginId& VST2PluginShellPluginId::instance()
{
    static VST2PluginShellPluginId ret;
    return ret;
}

VstInt32 VST2PluginShellPluginId::getId() const
{
    return id_;
}

bool VST2PluginShellPluginId::idShouldBeZero() const
{
    return idShouldBeZero_;
}

AEffect* VST2PluginShellPluginId::getShellPlugin(VstInt32 id, bool idShouldBeZero,
    VST2PluginEntryProc pluginEntryProc)
{
    std::lock_guard<std::mutex> lg(mutex_);
    id_ = id;
    idShouldBeZero_ = idShouldBeZero;
    return pluginEntryProc(pluginVST2Callback);
}
}
