#include "VST3Plugin.hpp"

#include "audio/host/MusecVST3Host.hpp"

#include <stdexcept>

namespace Musec::Audio::Plugin
{
template<typename SampleType> VST3Plugin<SampleType>::VST3Plugin():
    VST3Plugin::Base()
{
    //
}

template<typename SampleType> VST3Plugin<SampleType>::VST3Plugin(const QString& path, int classIndex):
    VST3Plugin::Base(path)
{
    auto pluginInitProc = Musec::Native::getExport<Musec::Base::VST3PluginInitProc>(*this, "InitDll");
    auto pluginFactoryProc = Musec::Native::getExport<Musec::Base::VST3PluginFactoryProc>(*this, "GetPluginFactory");
    if(!pluginFactoryProc)
    {
        auto error = GetLastError();
        switch (error)
        {
            case ERROR_PROC_NOT_FOUND:
                break;
            default:
                break;
        }
        throw std::invalid_argument("");
    }
    if(pluginInitProc)
    {
        if(!pluginInitProc())
        {
            // 初始化出现问题
            throw std::runtime_error("");
        }
    }
    factory_ = pluginFactoryProc();
    if(classIndex >= factory_->countClasses())
    {
        throw std::invalid_argument("");
    }
    Steinberg::PClassInfo classInfo;
    factory_->getClassInfo(classIndex, &classInfo);
    if(!factory_->createInstance(classInfo.cid,
                                 IAudioProcessor::iid,
                                 reinterpret_cast<void**>(&effect_)))
    {
        throw std::invalid_argument("");
    }
}

template<typename SampleType> VST3Plugin<SampleType>::~VST3Plugin() noexcept
{
    if(factory_)
    {
        factory_->release();
        auto pluginExitProc = Musec::Native::getExport<Musec::Base::VST3PluginExitProc>(*this, "ExitDll");
        if(pluginExitProc)
        {
            pluginExitProc();
        }
    }
}

template<typename SampleType> Steinberg::Vst::IAudioProcessor* VST3Plugin<SampleType>::effect() const
{
    return effect_;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::initialize(double sampleRate, std::int32_t maxSampleCount)
{
    Steinberg::Vst::IComponent* component = nullptr;
    effect_->queryInterface(Steinberg::Vst::IComponent::iid,
                            reinterpret_cast<void**>(&component));
    auto& host = MusecVST3Host::instance();
    component->initialize(&host);
    Steinberg::Vst::ProcessSetup setup;
    processData_.processMode = setup.processMode = Steinberg::Vst::ProcessModes::kRealtime;
    setup.sampleRate = sampleRate;
    setup.maxSamplesPerBlock = maxSampleCount;
    processData_.numInputs = 1;
    processData_.numOutputs = 1;
    input_.numChannels = 2;
    output_.numChannels = 2;
    processData_.inputs = &input_;
    processData_.outputs = &output_;
    if constexpr(std::is_same_v<SampleType, float>)
    {
        processData_.symbolicSampleSize = setup.symbolicSampleSize = Steinberg::Vst::SymbolicSampleSizes::kSample32;
        input_.channelBuffers32 = channelBuffer_.data();
        output_.channelBuffers32 = channelBuffer_.data() + 2;
    }
    else if constexpr(std::is_same_v<SampleType, double>)
    {
        processData_.symbolicSampleSize = setup.symbolicSampleSize = Steinberg::Vst::SymbolicSampleSizes::kSample64;
        input_.channelBuffers64 = channelBuffer_.data();
        output_.channelBuffers64 = channelBuffer_.data() + 2;
    }
    effect_->setupProcessing(setup);
    return true;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::uninitialize()
{
    return true;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::startProcessing()
{
    effect_->setProcessing(true);
    return true;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::stopProcessing()
{
    effect_->setProcessing(false);
    return true;
}

template<typename SampleType>
void VST3Plugin<SampleType>::process(std::array<SampleType*, 2> input, std::array<SampleType*, 2> output)
{
    // FIXME: 从外部获取要处理的采样数
    std::int32_t sampleCount = 0;
    channelBuffer_[0] = input[0];
    channelBuffer_[1] = input[1];
    channelBuffer_[2] = output[0];
    channelBuffer_[3] = output[1];
    effect_->process(processData_);
}

extern template class VST3Plugin<float>;
extern template class VST3Plugin<double>;
}
