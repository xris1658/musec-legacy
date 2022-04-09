#ifndef MUSEC_AUDIO_PLUGIN_VST3PLUGIN
#define MUSEC_AUDIO_PLUGIN_VST3PLUGIN

#include "audio/plugin/IPlugin.hpp"
#include "base/PluginBase.hpp"
#include "native/WindowsLibraryRAII.hpp"

#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>

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
    Steinberg::Vst::IComponent* component() const;
    Steinberg::Vst::IEditController* editController() const;
public: // IDevice interfaces
    uint8_t inputCount() const override;
    uint8_t outputCount() const override;
    void process(const Audio::Base::AudioBufferViews<SampleType>& inputs,
        const Audio::Base::AudioBufferViews<SampleType>& outputs) override;
public: // IPlugin interfaces
    bool activate() override;
    bool deactivate() override;
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
    Steinberg::Vst::IEditController* editController_ = nullptr;
private:
    // IAudioProcessor::process 函数调用的实参
    Steinberg::Vst::ProcessData processData_;
    // 调用 process 函数时将 data 赋值给 processData_
    std::vector<Steinberg::Vst::AudioBusBuffers> inputs_;
    // 调用 process 函数时将 data 赋值给 processData_
    std::vector<Steinberg::Vst::AudioBusBuffers> outputs_;
    // 输入音频缓冲区的原始数组
    std::vector<SampleType*> inputRaw_;
    // 输出音频缓冲区的原始数组
    std::vector<SampleType*> outputRaw_;
    // 各个总线的输入的扬声器布局
    SpeakerArrangements inputSpeakerArrangements_;
    // 各个总线输出的扬声器布局
    SpeakerArrangements outputSpeakerArrangements_;
};

extern template class VST3Plugin<float>;
extern template class VST3Plugin<double>;
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3PLUGIN
