#include "VST3Plugin.hpp"

#include "audio/host/MusecVST3Host.hpp"
#include "audio/host/VST3ComponentHandler.hpp"

#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/vst/vstspeaker.h>

#include <stdexcept>

namespace Musec::Audio::Plugin
{
// VST3Plugin ctor & dtor
// ------------------------------------------------------------------------------------------
template<typename SampleType>
VST3Plugin<SampleType>::VST3Plugin():
    VST3Plugin::Base(), VST3Plugin::IPluginInterface()
{
    //
}

template<typename SampleType>
VST3Plugin<SampleType>::VST3Plugin(const QString& path, int classIndex):
    VST3Plugin::Base(path), VST3Plugin::IPluginInterface()
{
    auto pluginInitProc = Musec::Native::getExport<Musec::Base::VST3PluginInitProc>(*this, "InitDll");
    auto pluginFactoryProc = Musec::Native::getExport<Musec::Base::VST3PluginFactoryProc>(*this, "GetPluginFactory");
    if (!pluginFactoryProc)
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
    if (pluginInitProc)
    {
        if (!pluginInitProc())
        {
            // 初始化出现问题
            throw std::runtime_error("");
        }
    }
    factory_ = pluginFactoryProc();
    if (classIndex >= factory_->countClasses())
    {
        throw std::runtime_error("");
    }
    factory_->getClassInfo(classIndex, &classInfo_);
    auto createInstanceResult = factory_->createInstance(classInfo_.cid, Steinberg::Vst::IAudioProcessor_iid,
        reinterpret_cast<void**>(&effect_));
    if (createInstanceResult != Steinberg::kResultOk)
    {
        throw std::runtime_error("Error creating audio processor instance!");
    }
    auto queryComponentResult = factory_->createInstance(classInfo_.cid, Steinberg::Vst::IComponent_iid,
        reinterpret_cast<void**>(&component_));
    if (queryComponentResult != Steinberg::kResultOk)
    {
        throw std::runtime_error("Cannot get IComponent interface!");
    }
    Steinberg::TUID controllerId;
    if (component_->getControllerClassId(controllerId) == Steinberg::kResultOk)
    {
        factory_->createInstance(controllerId, Steinberg::Vst::IEditController_iid,
            reinterpret_cast<void**>(&editController_));
    }
    else
    {
        auto queryEditorFromComponentResult = component_->queryInterface(Steinberg::Vst::IEditController_iid,
            reinterpret_cast<void**>(&editController_));
        if (queryEditorFromComponentResult == Steinberg::kResultOk)
        {
            processorAndEditorUnified_ = true;
        }
    }
    if (editController_)
    {
        editController_->initialize(nullptr);
        editController_->setComponentHandler(&(Musec::Audio::Host::VST3ComponentHandler::instance()));
        view_ = editController_->createView(Steinberg::Vst::ViewType::kEditor);
        view_->addRef();
    }
}

template<typename SampleType>
VST3Plugin<SampleType>::~VST3Plugin() noexcept
{
    if (editController_)
    {
        view_->release();
        editController_->terminate();
        editController_->release();
    }
    if (effect_)
    {
        effect_->release();
    }
    if (component_)
    {
        component_->terminate();
        component_->release();
    }
    if (factory_)
    {
        factory_->release();
        auto pluginExitProc = Musec::Native::getExport<Musec::Base::VST3PluginExitProc>(*this, "ExitDll");
        if (pluginExitProc)
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

template<typename SampleType>
Steinberg::Vst::IComponent* VST3Plugin<SampleType>::component() const
{
    return component_;
}

template<typename SampleType>
Steinberg::Vst::IEditController* VST3Plugin<SampleType>::editController() const
{
    return editController_;
}
// ------------------------------------------------------------------------------------------

// VST3Plugin IDevice interfaces
// ------------------------------------------------------------------------------------------
template<typename SampleType>
std::uint8_t VST3Plugin<SampleType>::inputCount() const
{
    std::uint8_t ret = 0;
    for (auto& arrangement: inputSpeakerArrangements_)
    {
        ret += Steinberg::Vst::SpeakerArr::getChannelCount(arrangement);
    }
    return ret;
}

template<typename SampleType>
std::uint8_t VST3Plugin<SampleType>::outputCount() const
{
    std::uint8_t ret = 0;
    for (auto& arrangement: outputSpeakerArrangements_)
    {
        ret += Steinberg::Vst::SpeakerArr::getChannelCount(arrangement);
    }
    return ret;
}

template<typename SampleType>
void VST3Plugin<SampleType>::process(const Musec::Audio::Base::AudioBufferViews<SampleType>& inputs,
    const Musec::Audio::Base::AudioBufferViews<SampleType>& outputs)
{
    auto inputsSize = inputs.size();
    auto outputsSize = outputs.size();
    assert(inputCount() == inputsSize);
    assert(outputCount() == outputsSize);
    for (int i = 0; i < inputsSize; ++i)
    {
        inputRaw_[i] = inputs[i].getSamples();
    }
    for (int i = 0; i < outputsSize; ++i)
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
    // ProcessSetup -------------------------------------------------------------------------
    Steinberg::Vst::ProcessSetup setup{};
    setup.processMode = Steinberg::Vst::ProcessModes::kRealtime;
    if constexpr(std::is_same_v<SampleType, float>)
    {
        setup.symbolicSampleSize = Steinberg::Vst::SymbolicSampleSizes::kSample32;
    }
    else if constexpr(std::is_same_v<SampleType, double>)
    {
        setup.symbolicSampleSize = Steinberg::Vst::SymbolicSampleSizes::kSample64;
    }
    setup.maxSamplesPerBlock = maxSampleCount;
    setup.sampleRate = sampleRate;
    // ProcessSetup -------------------------------------------------------------------------
    // ProcessData --------------------------------------------------------------------------
    processData_.processMode = setup.processMode;
    processData_.symbolicSampleSize = setup.symbolicSampleSize;
    auto inputBusCount = component_->getBusCount(Steinberg::Vst::MediaTypes::kAudio,
        Steinberg::Vst::BusDirections::kInput);
    auto outputBusCount = component_->getBusCount(Steinberg::Vst::MediaTypes::kAudio,
        Steinberg::Vst::BusDirections::kOutput);
    inputs_ = decltype(inputs_)(inputBusCount);
    outputs_ = decltype(outputs_)(outputBusCount);
    inputSpeakerArrangements_ = decltype(inputSpeakerArrangements_)(inputBusCount, Steinberg::Vst::SpeakerArr::kStereo);
    outputSpeakerArrangements_ = decltype(outputSpeakerArrangements_)(outputBusCount, Steinberg::Vst::SpeakerArr::kStereo);
    Steinberg::Vst::BusInfo busInfo;
    for(decltype(inputBusCount) i = 0; i < inputBusCount; ++i)
    {
        component_->getBusInfo(Steinberg::Vst::MediaTypes::kAudio, Steinberg::Vst::BusDirections::kInput,
            i, busInfo);
        auto channelCount = busInfo.channelCount;
        //effect_->getBusArrangement(Steinberg::Vst::BusDirections::kInput, i, inputSpeakerArrangements_[i]);
    }
    for(decltype(outputBusCount) i = 0; i < outputBusCount; ++i)
    {
        component_->getBusInfo(Steinberg::Vst::MediaTypes::kAudio, Steinberg::Vst::BusDirections::kOutput,
            i, busInfo);
        auto channelCount = busInfo.channelCount;
        //effect_->getBusArrangement(Steinberg::Vst::BusDirections::kOutput, i, outputSpeakerArrangements_[i]);
    }
    auto setBusArrangementsResult = effect_->setBusArrangements(
        inputSpeakerArrangements_.data(), inputBusCount,
        outputSpeakerArrangements_.data(), outputBusCount);
    if (setBusArrangementsResult != Steinberg::kResultOk)
    {
        // 再次调用 getBusArrangement，用于宿主程序与插件之间协商总线布局
        // 参考：https://developer.steinberg.help/pages/viewpage.action?pageId=49906849
        // My plug-in is capable of processing all possible channel configurations
        // https://developer.steinberg.help/display/VST/Bus+Arrangement+Setting+Sequences
        for (decltype(outputBusCount) i = 0; i < outputBusCount; ++i)
        {
            effect_->getBusArrangement(Steinberg::Vst::BusDirections::kOutput, i, outputSpeakerArrangements_[i]);
        }
        for (decltype(inputBusCount) i = 0; i < inputBusCount; ++i)
        {
            // Youlean Loudness Meter 2 会在此处抛出 std::out_of_range
            effect_->getBusArrangement(Steinberg::Vst::BusDirections::kInput, i, inputSpeakerArrangements_[i]);
        }
        effect_->setBusArrangements(inputSpeakerArrangements_.data(), inputSpeakerArrangements_.size(),
            outputSpeakerArrangements_.data(), outputSpeakerArrangements_.size());
        // 不检查此处的结果。因为有些插件本身第一次就成功，结果两次 setBusArrangement 返回的结果均为 kResultFalse
        // FabFilter 插件
    }
    for (decltype(inputBusCount) i = 0; i < inputBusCount; ++i)
    {
        effect_->getBusArrangement(Steinberg::Vst::BusDirections::kInput, i, inputSpeakerArrangements_[i]);
        inputs_[i].numChannels = Steinberg::Vst::SpeakerArr::getChannelCount(inputSpeakerArrangements_[i]);
    }
    for (decltype(outputBusCount) i = 0; i < outputBusCount; ++i)
    {
        effect_->getBusArrangement(Steinberg::Vst::BusDirections::kOutput, i, outputSpeakerArrangements_[i]);
        outputs_[i].numChannels = Steinberg::Vst::SpeakerArr::getChannelCount(outputSpeakerArrangements_[i]);
    }
    // ProcessData ---------------------------------------------------------------------------------
    // Voxengo 插件均在此处崩溃
    effect_->setupProcessing(setup);
    return true;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::uninitialize()
{
    return component_->terminate() == Steinberg::kResultOk;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::activate()
{
    return component_->setActive(true) == Steinberg::kResultOk;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::deactivate()
{
    return component_->setActive(false) == Steinberg::kResultOk;
}

// 或许上 RAII 更合适？
template<typename SampleType>
bool VST3Plugin<SampleType>::startProcessing()
{
    return effect_->setProcessing(true) == Steinberg::kResultOk;
}

// 或许上 RAII 更合适？
template<typename SampleType>
bool VST3Plugin<SampleType>::stopProcessing()
{
    return effect_->setProcessing(false) == Steinberg::kResultOk;
}

template<typename SampleType>
void VST3Plugin<SampleType>::rawToProcessData()
{
    int rawIt = 0;
    auto inputBusSize = inputs_.size();
    auto outputBusSize = outputs_.size();
    if constexpr(std::is_same_v<SampleType, float>)
    {
        for (int i = 0; i < inputBusSize; ++i)
        {
            inputs_[i].channelBuffers32 = &(inputRaw_[rawIt]);
            rawIt += inputs_[i].numChannels;
        }
        rawIt = 0;
        for (int i = 0; i < outputBusSize; ++i)
        {
            outputs_[i].channelBuffers32 = &(outputRaw_[rawIt]);
            rawIt += outputs_[i].numChannels;
        }
    }
    else if constexpr(std::is_same_v<SampleType, double>)
    {
        for (int i = 0; i < inputBusSize; ++i)
        {
            inputs_[i].channelBuffers64 = &(inputRaw_[rawIt]);
            rawIt += inputs_[i].numChannels;
        }
        rawIt = 0;
        for (int i = 0; i < outputBusSize; ++i)
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

template<typename SampleType>
bool VST3Plugin<SampleType>::attachToWindow(QWindow* window)
{
    Steinberg::ViewRect viewRect;
    view_->getSize(&viewRect);
    window->setPosition(viewRect.left, viewRect.top);
    window->setWidth(viewRect.right - viewRect.left);
    window->setHeight(viewRect.bottom - viewRect.top);
    window->setTitle(classInfo_.name);
    view_->attached(reinterpret_cast<HWND>(window->winId()), Steinberg::kPlatformTypeHWND);
    return true;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::detachWithWindow()
{
    view_->removed();
    return true;
}

template<typename SampleType>
const Steinberg::PClassInfo& VST3Plugin<SampleType>::getClassInfo() const
{
    return classInfo_;
}

template<typename SampleType>
Steinberg::IPluginFactory* VST3Plugin<SampleType>::factory() const
{
    return factory_;
}

template<typename SampleType>
Steinberg::IPlugView* VST3Plugin<SampleType>::getView() const
{
    return view_;
}

template<typename SampleType>
bool VST3Plugin<SampleType>::isProcessorAndEditorUnified() const
{
    return processorAndEditorUnified_;
}

template class VST3Plugin<float>;

template class VST3Plugin<double>;
}
