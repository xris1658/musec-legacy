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
VstIntPtr pluginVST2Callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);

class VST2Plugin:
    public Musec::Native::WindowsLibraryRAII,
    public Musec::Audio::Plugin::IPlugin
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
    VstProcessLevels processLevel_ = kVstProcessLevelUser;
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
