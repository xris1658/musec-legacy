#ifndef MUSEC_AUDIO_PLUGIN_VST2PLUGIN
#define MUSEC_AUDIO_PLUGIN_VST2PLUGIN

#include "audio/plugin/IPlugin.hpp"
#include "base/PluginBase.hpp"
#include "native/WindowsLibraryRAII.hpp"

#include <pluginterfaces/vst2.x/aeffect.h>
#include <pluginterfaces/vst2.x/aeffectx.h>
#include <base/thread/include/flock.h>

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Plugin
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

template<typename SampleType>
class VST2Plugin:
    public Musec::Native::WindowsLibraryRAII,
    public Musec::Audio::Plugin::IPlugin<SampleType>
{
    using Base = Musec::Native::WindowsLibraryRAII;
    using PluginInterface = Musec::Audio::Plugin::IPlugin<SampleType>;
public:
    VST2Plugin(const QString& path, bool scanPlugin = false, VstInt32 shellPluginId = 0);
    ~VST2Plugin() noexcept override;
public:
    AEffect* effect() const;
public:
    std::uint8_t inputCount() const override;
    std::uint8_t outputCount() const override;
public:
    bool initialize(double sampleRate, std::int32_t maxSampleCount) override;
    bool uninitialize() override;
    bool activate() override;
    bool deactivate() override;
    bool startProcessing() override;
    bool stopProcessing() override;
    void process(const Audio::Base::AudioBufferViews<SampleType>& inputs,
        const Audio::Base::AudioBufferViews<SampleType>& outputs) override;
private:
    AEffect* effect_ = nullptr;
    std::vector<SampleType*> inputsRaw_;
    std::vector<SampleType*> outputsRaw_;
};

extern template class VST2Plugin<float>;
extern template class VST2Plugin<double>;
}
}

}

#endif //MUSEC_AUDIO_PLUGIN_VST2PLUGIN
