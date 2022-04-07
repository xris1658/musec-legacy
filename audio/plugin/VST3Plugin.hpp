#ifndef MUSEC_AUDIO_PLUGIN_VST3PLUGIN
#define MUSEC_AUDIO_PLUGIN_VST3PLUGIN

#include "audio/plugin/IPlugin.hpp"
#include "base/PluginBase.hpp"
#include "native/WindowsLibraryRAII.hpp"

#include <pluginterfaces/vst/ivstaudioprocessor.h>

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using SpeakerArrangements = std::vector<Steinberg::Vst::SpeakerArrangement>;
template<typename SampleType>
class VST3Plugin:
    public Musec::Native::WindowsLibraryRAII,
    public Musec::Audio::Plugin::IPlugin<SampleType>
{
    using Base = Musec::Native::WindowsLibraryRAII;
    using IPluginInterface = Musec::Audio::Plugin::IPlugin<SampleType>;
    using SampleTypePointers = std::vector<SampleType*>;
public: // ctor & dtor
    VST3Plugin();
    VST3Plugin(const QString& path, int classIndex);
    ~VST3Plugin() noexcept override;
public:
    Steinberg::Vst::IAudioProcessor* effect() const;
public: // IDevice interfaces
    uint8_t inputCount() const override;
    uint8_t outputCount() const override;
    void process(const Audio::Base::AudioBufferViews<SampleType>& inputs,
        const Audio::Base::AudioBufferViews<SampleType>& outputs) override;
public: // IPlugin interfaces
    bool initialize(double sampleRate, std::int32_t sampleCount) override;
    bool uninitialize() override;
    bool startProcessing() override;
    bool stopProcessing() override;
public:
    const SpeakerArrangements& inputSpeakerArrangements();
    const SpeakerArrangements& outputSpeakerArrangements();
private:
    void rawToProcessData();
private:
    Steinberg::IPluginFactory* factory_ = nullptr;
    Steinberg::Vst::IComponent* component_ = nullptr;
    Steinberg::Vst::IAudioProcessor* effect_ = nullptr;
    // 调用 process 函数时将缓冲区信息填入此处，交给插件处理
    Steinberg::Vst::ProcessData processData_;
    // 调用 process 函数时将输入缓冲区信息填入此处，交给插件处理
    std::vector<Steinberg::Vst::AudioBusBuffers> inputs_;
    // 调用 process 函数时将输出缓冲区信息填入此处，交给插件处理
    std::vector<Steinberg::Vst::AudioBusBuffers> outputs_;
    std::vector<SampleType*> inputRaw_;
    std::vector<SampleType*> outputRaw_;
    SpeakerArrangements inputSpeakerArrangements_;
    SpeakerArrangements outputSpeakerArrangements_;
};

extern template class VST3Plugin<float>;
extern template class VST3Plugin<double>;
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3PLUGIN
