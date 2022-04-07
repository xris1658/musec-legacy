#include "VST2Plugin.hpp"

namespace Musec::Audio::Plugin
{
ShellPluginId::ShellPluginId()
{
    //
}

ShellPluginId::~ShellPluginId() noexcept
{
    //
}

ShellPluginId& ShellPluginId::instance()
{
    static ShellPluginId ret;
    return ret;
}

VstInt32 ShellPluginId::getId() const
{
    return id_;
}

bool ShellPluginId::idShouldBeZero() const
{
    return idShouldBeZero_;
}

AEffect* ShellPluginId::getShellPlugin(VstInt32 id, bool idShouldBeZero,
    Musec::Base::VST2PluginEntryProc pluginEntryProc)
{
    Steinberg::Base::Thread::FGuard lg(mutex_);
    id_ = id;
    idShouldBeZero_ = idShouldBeZero;
    return pluginEntryProc(pluginVST2Callback);
}
}
