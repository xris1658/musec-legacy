#ifndef MUSEC_AUDIO_PLUGIN_VST2PLUGIN
#define MUSEC_AUDIO_PLUGIN_VST2PLUGIN

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

struct VST2ParameterInfo
{
    VstParameterProperties properties;
    // According to the VST2 document, the length of parameter name, display value and unit
    // (NULL terminator not included) are all kVstMaxParamStrLen (= 8).
    // Some vendors (e.g. FabFilter) completely ignored this and have some parameter names of up to 35 chars.
    // (NULL terminator not included; the name of that parameter is "Loudness Integrated Peak Time Scale"
    // in FabFilter Pro-L 2.)
    char name[36];
    // At least one of the FabFilter plugins have one or more parameter displays of 24 chars.
    char display[25];
    // kVstMaxParamStrLen (= 8) + 1 might suffice, but here we just give it more bytes in case some vendors exceeds
    // that limit. In addition, we use this figure for memory alignment (36 + 25 + 23 = 84; 104 + 84 + 4 = 192).
    char unit[23];
    float value;
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
    ~VST2Plugin() override;
public:
    AEffect* effect() const;
public:
    std::uint8_t inputCount() const override;
    std::uint8_t outputCount() const override;
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
    bool startProcessing() override;
    bool stopProcessing() override;
    bool getBypass() const override;
    QString getName() const override;
    void process(const Audio::Base::AudioBufferViews<SampleType>& inputs,
        const Audio::Base::AudioBufferViews<SampleType>& outputs) override;
    bool hasUI() override;
    Musec::Base::PluginFormat pluginFormat() override;
private:
    AEffect* effect_ = nullptr;
    std::vector<SampleType*> inputsRaw_;
    std::vector<SampleType*> outputsRaw_;
    bool bypass_ = true;
    bool activated_ = false;
    QWindow* window_ = nullptr;
    Musec::Base::FixedSizeMemoryBlock paramBlock_ = {0};
};

extern template class VST2Plugin<float>;
extern template class VST2Plugin<double>;
}
}

}

#endif //MUSEC_AUDIO_PLUGIN_VST2PLUGIN
