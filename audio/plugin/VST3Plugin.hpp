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
template<typename SampleType>
class VST3Plugin:
    public Musec::Native::WindowsLibraryRAII,
    public Musec::Audio::Plugin::IPlugin<SampleType>
{
    using Base = Musec::Native::WindowsLibraryRAII;
public:
    VST3Plugin();
    VST3Plugin(const QString& path, int classIndex);
    ~VST3Plugin() noexcept override;
public:
    Steinberg::Vst::IAudioProcessor* effect() const;
public:
    bool initialize(double sampleRate, std::int32_t sampleCount) override;
    bool uninitialize() override;
    bool startProcessing() override;
    bool stopProcessing() override;
    void process(std::array<SampleType*, 2> input, std::array<SampleType*, 2> output) override;
private:
    Steinberg::IPluginFactory* factory_ = nullptr;
    Steinberg::Vst::IAudioProcessor* effect_ = nullptr;
    Steinberg::Vst::ProcessData processData_;
    Steinberg::Vst::AudioBusBuffers input_;
    Steinberg::Vst::AudioBusBuffers output_;
    std::array<SampleType*, 4> channelBuffer_ = {nullptr};
};

extern template class VST3Plugin<float>;
extern template class VST3Plugin<double>;
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3PLUGIN
