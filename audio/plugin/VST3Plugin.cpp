#include "VST3Plugin.hpp"

#include "audio/host/MusecVST3Host.hpp"

#include <stdexcept>

namespace Musec::Audio::Plugin
{
// VST3Plugin ctor & dtor
// ------------------------------------------------------------------------------------------
template<typename SampleType>
VST3Plugin<SampleType>::VST3Plugin():
    VST3Plugin::Base(),
    VST3Plugin::IPluginInterface()
{
    //
}

template<typename SampleType>
VST3Plugin<SampleType>::VST3Plugin(const QString& path, int classIndex):
    VST3Plugin::Base(path),
    VST3Plugin::IPluginInterface()
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
    if(!factory_->createInstance
        (
            classInfo.cid,
                Steinberg::Vst::IAudioProcessor::iid,
                reinterpret_cast<void**>(&effect_)
        )
    )
    {
        throw std::invalid_argument("");
    }
    effect_->queryInterface(Steinberg::Vst::IComponent::iid,
        reinterpret_cast<void**>(&component_));
}

template<typename SampleType>
VST3Plugin<SampleType>::~VST3Plugin() noexcept
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
// ------------------------------------------------------------------------------------------

// VST3Plugin raw pointer type
// ------------------------------------------------------------------------------------------
template<typename SampleType>
Steinberg::Vst::IAudioProcessor* VST3Plugin<SampleType>::effect() const
{
    return effect_;
}
// ------------------------------------------------------------------------------------------

// VST3Plugin IDevice interfaces
// ------------------------------------------------------------------------------------------
template<typename SampleType>
std::uint8_t VST3Plugin<SampleType>::inputCount() const
{
    auto inputCount = component_->getBusCount(Steinberg::Vst::MediaTypes::kAudio, Steinberg::Vst::BusDirections::kInput);
    std::uint8_t ret = 0;
    Steinberg::Vst::BusInfo busInfo {};
    for(int i = 0; i < inputCount; ++i)
    {
        component_->getBusInfo(
            Steinberg::Vst::MediaTypes::kAudio,
            Steinberg::Vst::BusDirections::kInput,
            i, busInfo
        );
        ret += busInfo.channelCount;
    }
    return ret;
}

template<typename SampleType>
std::uint8_t VST3Plugin<SampleType>::outputCount() const
{
    auto outputCount = component_->getBusCount(Steinberg::Vst::MediaTypes::kAudio, Steinberg::Vst::BusDirections::kOutput);
    std::uint8_t ret = 0;
    Steinberg::Vst::BusInfo busInfo;
    for(int i = 0; i < outputCount; ++i)
    {
        component_->getBusInfo(
            Steinberg::Vst::MediaTypes::kAudio,
            Steinberg::Vst::BusDirections::kOutput,
            i, busInfo
        );
        ret += busInfo.channelCount;
    }
    return ret;
}

template<typename SampleType>
void VST3Plugin<SampleType>::process(const Musec::Audio::Base::AudioBufferViews<SampleType>& inputs,
    const Musec::Audio::Base::AudioBufferViews<SampleType>& outputs)
{
    auto inputsSize = inputs.size();
    auto outputsSize = outputs.size();
    for(int i = 0; i < inputsSize; ++i)
    {
        inputRaw_[i] = inputs[i].getSamples();
    }
    for(int i = 0; i < outputsSize; ++i)
    {
        outputRaw_[i] = outputs[i].getSamples();
    }
    rawToProcessData();
    effect_->process(processData_);
}

// ------------------------------------------------------------------------------------------

// VST3Plugin IPlugin interfaces
// ------------------------------------------------------------------------------------------
template<typename SampleType>
bool VST3Plugin<SampleType>::initialize(double sampleRate, std::int32_t maxSampleCount)
{
    auto& host = Musec::Audio::Host::MusecVST3Host::instance();
    component_->initialize(&host);
    Steinberg::Vst::ProcessSetup setup;
    processData_.processMode = setup.processMode = Steinberg::Vst::ProcessModes::kRealtime;
    setup.sampleRate = sampleRate;
    setup.maxSamplesPerBlock = maxSampleCount;
    processData_.numInputs = 1;
    processData_.numOutputs = 1;
    inputs_ = decltype(inputs_)(component_->getBusCount(Steinberg::Vst::MediaTypes::kAudio, Steinberg::Vst::BusDirections::kInput));
    inputSpeakerArrangements_ = SpeakerArrangements(inputs_.size(), VstSpeakerArrangementType::kSpeakerArrEmpty);
    inputSpeakerArrangements_[0] = VstSpeakerArrangementType::kSpeakerArrStereo;
    inputRaw_ = SampleTypePointers(inputCount(), nullptr);
    outputs_ = decltype(outputs_)(component_->getBusCount(Steinberg::Vst::MediaTypes::kAudio, Steinberg::Vst::BusDirections::kOutput));
    outputSpeakerArrangements_ = SpeakerArrangements(outputs_.size(), VstSpeakerArrangementType::kSpeakerArrEmpty);
    outputSpeakerArrangements_[0] = VstSpeakerArrangementType::kSpeakerArrStereo;
    outputRaw_ = SampleTypePointers(outputCount(), nullptr);
    // TODO
    inputs_[0].numChannels = 2;
    outputs_[0].numChannels = 2;
    if constexpr(std::is_same_v<SampleType, float>)
    {
        processData_.symbolicSampleSize = setup.symbolicSampleSize = Steinberg::Vst::SymbolicSampleSizes::kSample32;
    }
    else if constexpr(std::is_same_v<SampleType, double>)
    {
        processData_.symbolicSampleSize = setup.symbolicSampleSize = Steinberg::Vst::SymbolicSampleSizes::kSample64;
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
void VST3Plugin<SampleType>::rawToProcessData()
{
    int rawIt = 0;
    auto inputBusSize = inputs_.size();
    auto outputBusSize = outputs_.size();
    if constexpr(std::is_same_v<SampleType, float>)
    {
        for(int i = 0; i < inputBusSize; ++i)
        {
            inputs_[i].channelBuffers32 = &(inputRaw_[rawIt]);
            rawIt += inputs_[i].numChannels;
        }
        rawIt = 0;
        for(int i = 0; i < outputBusSize; ++i)
        {
            outputs_[i].channelBuffers32 = &(outputRaw_[rawIt]);
            rawIt += outputs_[i].numChannels;
        }
    }
    else if constexpr(std::is_same_v<SampleType, double>)
    {
        for(int i = 0; i < inputBusSize; ++i)
        {
            inputs_[i].channelBuffers64 = &(inputRaw_[rawIt]);
            rawIt += inputs_[i].numChannels;
        }
        rawIt = 0;
        for(int i = 0; i < outputBusSize; ++i)
        {
            outputs_[i].channelBuffers64 = &(outputRaw_[rawIt]);
            rawIt += outputs_[i].numChannels;
        }
    }
}

template<typename SampleType>
const SpeakerArrangements& VST3Plugin<SampleType>::inputSpeakerArrangements()
{
    return inputSpeakerArrangements_;
}

template<typename SampleType>
const SpeakerArrangements& VST3Plugin<SampleType>::outputSpeakerArrangements()
{
    return outputSpeakerArrangements_;
}

template class VST3Plugin<float>;
template class VST3Plugin<double>;
}
