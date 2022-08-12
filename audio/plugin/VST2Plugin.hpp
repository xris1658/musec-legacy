#ifndef MUSEC_AUDIO_PLUGIN_VST2PLUGIN
#define MUSEC_AUDIO_PLUGIN_VST2PLUGIN

#include "audio/plugin/IParameter.hpp"
#include "audio/plugin/IPlugin.hpp"
#include "base/FixedSizeMemoryBlock.hpp"
#include "base/PluginBase.hpp"
#include "native/WindowsLibraryRAII.hpp"

#pragma warning(push)
#pragma warning(disable: 4996) // C 风格字符串操纵可能不安全
#include <pluginterfaces/vst2.x/aeffect.h>
#include <pluginterfaces/vst2.x/aeffectx.h>
#pragma warning(pop)

#include <QString>

#include <mutex>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
// VST2 插件的入口函数（VSTPluginMain 或 main）
using VST2PluginEntryProc = AEffect* (*)(audioMasterCallback);

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
                            VST2PluginEntryProc pluginEntryProc);
private:
    VstInt32 id_ = 0;
    bool idShouldBeZero_ = true;
    std::mutex mutex_;
};

class VST2Plugin:
    public Musec::Native::WindowsLibraryRAII,
    public Musec::Audio::Plugin::IPlugin<float>
{
    using SampleType = float;
    using Base = Musec::Native::WindowsLibraryRAII;
public:
    VST2Plugin(const QString& path, bool scanPlugin = false, VstInt32 shellPluginId = 0);
    ~VST2Plugin() override;
public:
    AEffect* effect() const;
public:
    std::uint8_t inputCount() const override;
    std::uint8_t outputCount() const override;
    void process(Musec::Audio::Base::AudioBufferView<SampleType>* input, int inputCount,
        Musec::Audio::Base::AudioBufferView<SampleType>* output, int outputCount) override;
public:
    bool initialize(double sampleRate, std::int32_t maxSampleCount) override;
    bool uninitialize() override;
private:
    bool initializeEditor();
    bool uninitializeEditor();
public:
    bool attachToWindow(QWindow* window) override;
    bool detachWithWindow() override;
    QWindow* window() override;
    bool activate() override;
    bool deactivate() override;
    bool activated() override;
    bool processing() override;
    bool startProcessing() override;
    bool stopProcessing() override;
    bool getBypass() const override;
    QString getName() const override;
    bool hasUI() override;
    Musec::Base::PluginFormat pluginFormat() override;
    int parameterCount() override;
    IParameter& parameter(int index) override;
private:
    AEffect* effect_ = nullptr;
    std::vector<SampleType*> inputsRaw_;
    std::vector<SampleType*> outputsRaw_;
    bool bypass_ = true;
    bool activated_ = false;
    QWindow* window_ = nullptr;
    Musec::Base::FixedSizeMemoryBlock paramBlock_;
};
}
}

}

#endif //MUSEC_AUDIO_PLUGIN_VST2PLUGIN
