#ifndef MUSEC_NATIVE_VST2PLUGIN
#define MUSEC_NATIVE_VST2PLUGIN

#include "base/PluginBase.hpp"
#include "native/WindowsLibraryRAII.hpp"

#include <pluginterfaces/vst2.x/aeffect.h>
#include <pluginterfaces/vst2.x/aeffectx.h>
#include <base/thread/include/flock.h>

#include <QString>

namespace Musec
{
namespace Native
{
VstIntPtr pluginVST2Callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
class ShellPluginId
{
private:
    ShellPluginId();
public:
    ShellPluginId(const ShellPluginId&) = delete;
    ShellPluginId(ShellPluginId&&) = delete;
    ~ShellPluginId() noexcept;
public:
    static ShellPluginId& instance();
public:
    VstInt32 getId() const;
    bool idShouldBeZero() const;
public:
    AEffect* getShellPlugin(VstInt32 id,
                            bool idShouldBeZero,
                            Musec::Base::VST2PluginEntryProc pluginEntryProc);
private:
    VstInt32 id_ = 0;
    bool idShouldBeZero_ = true;
    Steinberg::Base::Thread::FLock mutex_ = "shlPlgn";
};

class VST2Plugin: public Musec::Native::WindowsLibraryRAII
{
    using Base = Musec::Native::WindowsLibraryRAII;
public:
    VST2Plugin();
    VST2Plugin(const QString& path, bool scanPlugin = false, VstInt32 shellPluginId = 0);
    ~VST2Plugin() noexcept override;
public:
    AEffect* effect() const;
private:
    AEffect* effect_ = nullptr;
};
}
}

#endif //MUSEC_NATIVE_VST2PLUGIN
