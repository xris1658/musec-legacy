#include "VST3Plugin.hpp"

#include "audio/host/MusecVST3Host.hpp"
#include "controller/AudioEngineController.hpp"

#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/vst/vstspeaker.h>

#include <public.sdk/source/common/memorystream.h>
#include <public.sdk/source/common/memorystream.cpp>

#include <errhandlingapi.h>

#include <stdexcept>

namespace Musec::Audio::Plugin
{
// VST3Plugin ctor & dtor
// ------------------------------------------------------------------------------------------
VST3Plugin::VST3Plugin():
    VST3Plugin::Base(), componentHandler_(this), plugFrame_(this)
{}

VST3Plugin::VST3Plugin(const QString& path, int classIndex):
    VST3Plugin::Base(path), componentHandler_(this), plugFrame_(this)
{
    // macOS 和 Linux 平台的入口和出口函数的名字与 Windows 的不同
    // macOS: bundleEntry 和 bundleExit
    // Linux: ModuleEntry 和 ModuleExit
    auto pluginInitProc = Musec::Native::getExport<Musec::Base::VST3PluginInitProc>(*this, VST3PluginInitName);
    auto pluginFactoryProc = Musec::Native::getExport<Musec::Base::VST3PluginFactoryProc>(*this, "GetPluginFactory");
    if (!pluginFactoryProc)
    {
        throw GetLastError();
    }
    if (pluginInitProc)
    {
        // Windows 平台可省略入口和出口函数的调用，但 macOS 和 Linux 不行
        // https://steinbergmedia.github.io/vst3_dev_portal/pages/Technical+Documentation/VST+Module+Architecture/Loading.html
        if (!pluginInitProc())
        {
            // 初始化出现问题
            throw std::runtime_error("");
        }
    }
    factory_ = pluginFactoryProc();
    audioProcessorStatus_ = VST3AudioProcessorStatus::Factory;
    editControllerStatus_ = VST3EditControllerStatus::Factory;
    auto classCount = factory_->countClasses();
    if (classIndex >= classCount)
    {
        throw std::runtime_error("");
    }
    factory_->getClassInfo(classIndex, &classInfo_);
    auto createComponentResult = factory_->createInstance(classInfo_.cid,
        Steinberg::Vst::IComponent::iid, reinterpret_cast<void**>(&component_));
    if(createComponentResult != Steinberg::kResultOk)
    {
        throw std::runtime_error("Error creating audio processor instance!");
    }
    audioProcessorStatus_ = VST3AudioProcessorStatus::Created;
}

VST3Plugin::~VST3Plugin()
{
    if(editControllerStatus_ == VST3EditControllerStatus::Connected)
    {
        uninitializeEditor();
    }
    if(audioProcessorStatus_ == VST3AudioProcessorStatus::Processing)
    {
        VST3Plugin::stopProcessing();
    }
    if(audioProcessorStatus_ == VST3AudioProcessorStatus::Activated)
    {
        VST3Plugin::deactivate();
    }
    if(audioProcessorStatus_ == VST3AudioProcessorStatus::SetupDone)
    {
        VST3Plugin::uninitialize();
    }
    if(audioProcessorStatus_ == VST3AudioProcessorStatus::Created)
    {
        component_->release();
        audioProcessorStatus_ = VST3AudioProcessorStatus::Factory;
    }
    if (factory_)
    {
        factory_->release();
        audioProcessorStatus_ = VST3AudioProcessorStatus::NoAudioProcessor;
        editControllerStatus_ = VST3EditControllerStatus::NoEditController;
        auto pluginExitProc = Musec::Native::getExport<Musec::Base::VST3PluginExitProc>(*this, VST3PluginExitName);
        if (pluginExitProc)
        {
            pluginExitProc();
        }
    }
}
// ------------------------------------------------------------------------------------------

// VST3Plugin raw pointer type
// ------------------------------------------------------------------------------------------
Steinberg::Vst::IAudioProcessor* VST3Plugin::effect() const
{
    return audioProcessor_;
}

Steinberg::Vst::IComponent* VST3Plugin::component() const
{
    return component_;
}

Steinberg::Vst::IEditController* VST3Plugin::editController() const
{
    return editController_;
}
// ------------------------------------------------------------------------------------------

// VST3Plugin IDevice interfaces
// ------------------------------------------------------------------------------------------
std::uint8_t VST3Plugin::inputCount() const
{
    std::uint8_t ret = 0;
    for (auto& arrangement: inputSpeakerArrangements_)
    {
        ret += Steinberg::Vst::SpeakerArr::getChannelCount(arrangement);
    }
    return ret;
}

std::uint8_t VST3Plugin::outputCount() const
{
    std::uint8_t ret = 0;
    for (auto& arrangement: outputSpeakerArrangements_)
    {
        ret += Steinberg::Vst::SpeakerArr::getChannelCount(arrangement);
    }
    return ret;
}

void VST3Plugin::process(Musec::Audio::Base::AudioBufferView<SampleType>* inputs, int inputCount,
    Musec::Audio::Base::AudioBufferView<SampleType>* outputs, int outputCount)
{
    processData_.numSamples = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    auto inputBusSize = inputs_.size();
    auto outputBusSize = outputs_.size();
    for(auto i = 0; i < inputBusSize; ++i)
    {
        if(i == audioInputBusIndex_)
        {
            for(auto j = 0; j < inputRaws_[i].size(); ++j)
            {
                if(j < inputCount)
                {
                    inputRaws_[i][j] = inputs[j].getSamples();
                }
                else
                {
                    inputRaws_[i][j] = Musec::Controller::AudioEngineController::dummyBufferView<SampleType>().getSamples();
                }
            }
        }
        else
        {
            for(auto j = 0; j < inputRaws_[i].size(); ++j)
            {
                inputRaws_[i][j] = Musec::Controller::AudioEngineController::dummyBufferView<SampleType>().getSamples();
            }
        }
        inputs_[i].numChannels = inputRaws_[i].size();
        inputs_[i].channelBuffers32 = reinterpret_cast<float**>(inputRaws_[i].data());
    }
    for(auto i = 0; i < outputBusSize; ++i)
    {
        if(i == audioOutputBusIndex_)
        {
            for(auto j = 0; j < outputRaws_[i].size(); ++j)
            {
                if(j < outputCount)
                {
                    outputRaws_[i][j] = outputs[j].getSamples();
                }
                else
                {
                    outputRaws_[i][j] = Musec::Controller::AudioEngineController::dummyBufferView<SampleType>().getSamples();
                }
            }
        }
        else
        {
            for(auto j = 0; j < outputRaws_[i].size(); ++j)
            {
                outputRaws_[i][j] = Musec::Controller::AudioEngineController::dummyBufferView<SampleType>().getSamples();
            }
        }
        outputs_[i].numChannels = outputRaws_[i].size();
        outputs_[i].channelBuffers32 = reinterpret_cast<float**>(outputRaws_[i].data());
    }
    processData_.numInputs = inputs_.size();
    processData_.numOutputs = outputs_.size();
    processData_.inputs = inputs_.data();
    processData_.outputs = outputs_.data();
    auto& processContext = Musec::Audio::Host::MusecVST3Host::AppProcessContext();
    processData_.processContext = &processContext;
    audioProcessor_->process(processData_);
    // FIXME：IEditController 的函数只能在 UI 线程调用
    // if(editController_)
    // {
    //     auto paramCount = outputParameterChanges_.getParameterCount();
    //     for(decltype(paramCount) i = 0; i < paramCount; ++i)
    //     {
    //         auto* paramValueQueue = outputParameterChanges_.getParameterData(i);
    //         auto paramId = paramValueQueue->getParameterId();
    //         Steinberg::int32 sampleOffset = 0;
    //         Steinberg::Vst::ParamValue paramValue;
    //         paramValueQueue->getPoint(i, sampleOffset, paramValue);
    //         // FIXME: 由于 IEditController 的函数不能在音频处理线程调用，
    //         //  因此调用 VST3 Host Checker 的相关函数导致堆栈溢出
    //         //  参考：public.sdk/samples/vst/hostchecker/source/hostcheckercontroller.cpp, ll. 734 - 738
    //         std::thread([=]() { editController_->setParamNormalized(paramId, paramValue); }).detach();
    //     }
    // }
}

// ------------------------------------------------------------------------------------------

// VST3Plugin IPlugin interfaces
// ------------------------------------------------------------------------------------------
bool VST3Plugin::initialize(double sampleRate, std::int32_t maxSampleCount)
{
    auto initializeComponentResult = component_->initialize(&Musec::Audio::Host::MusecVST3Host::instance());
    if(initializeComponentResult != Steinberg::kResultOk)
    {
        return false;
    }
    auto queryAudioProcessorResult = component_->queryInterface(
        Steinberg::Vst::IAudioProcessor::iid,
        reinterpret_cast<void**>(&audioProcessor_));
    if(queryAudioProcessorResult != Steinberg::kResultOk)
    {
        return false;
    }
    audioProcessorStatus_ = VST3AudioProcessorStatus::Initialized;
    if(audioProcessor_->canProcessSampleSize(Steinberg::Vst::SymbolicSampleSizes::kSample32)
        == Steinberg::kResultFalse)
    {
        return false;
    }
    initializeEditor();
    audioProcessor_->queryInterface(Steinberg::Vst::IProcessContextRequirements::iid,
        reinterpret_cast<void**>(&processContextRequirements_));
    audioProcessor_->queryInterface(Steinberg::Vst::IAudioPresentationLatency::iid,
        reinterpret_cast<void**>(&audioPresentationLatency_));
    // ProcessSetup -------------------------------------------------------------------------
    Steinberg::Vst::ProcessSetup setup;
    setup.processMode = Steinberg::Vst::ProcessModes::kRealtime;
    setup.symbolicSampleSize = Steinberg::Vst::SymbolicSampleSizes::kSample32;
    setup.maxSamplesPerBlock = maxSampleCount;
    setup.sampleRate = sampleRate;
    // ProcessSetup -------------------------------------------------------------------------
    // ProcessData --------------------------------------------------------------------------
    processData_.inputParameterChanges = &inputParameterChanges_;
    processData_.outputParameterChanges = &outputParameterChanges_;
    processData_.processMode = setup.processMode;
    processData_.symbolicSampleSize = setup.symbolicSampleSize;
    auto inputBusCount = component_->getBusCount(Steinberg::Vst::MediaTypes::kAudio,
        Steinberg::Vst::BusDirections::kInput);
    auto outputBusCount = component_->getBusCount(Steinberg::Vst::MediaTypes::kAudio,
        Steinberg::Vst::BusDirections::kOutput);
    inputRaws_ = decltype(inputRaws_)(inputBusCount);
    outputRaws_ = decltype(outputRaws_)(outputBusCount);
    inputSpeakerArrangements_.resize(inputBusCount, Steinberg::Vst::SpeakerArr::kStereo);
    outputSpeakerArrangements_.resize(outputBusCount, Steinberg::Vst::SpeakerArr::kStereo);
    inputs_ = decltype(inputs_)(inputBusCount);
    outputs_ = decltype(outputs_)(outputBusCount);
    // setupProcessing ------------------------------------------------------------------------------------------------- setupProcessing
    auto setupProcessingResult = audioProcessor_->setupProcessing(setup);
    if (setupProcessingResult != Steinberg::kResultOk
        && setupProcessingResult != Steinberg::kNotImplemented)
    {
        throw std::runtime_error("");
    }
    if(processContextRequirements_)
    {
        processContextRequirement_ = processContextRequirements_->getProcessContextRequirements();
    }
    auto setBusArrangementsResult = audioProcessor_->setBusArrangements(
        inputSpeakerArrangements_.data(), processData_.numInputs,
        outputSpeakerArrangements_.data(), processData_.numOutputs);
    decltype(setBusArrangementsResult) getBusArrangementResult = Steinberg::kResultOk;
    if(setBusArrangementsResult != Steinberg::kNotImplemented
    && setBusArrangementsResult != Steinberg::kResultOk)
    {
        // 再次调用 getBusArrangement，用于宿主程序与插件之间协商总线布局
        // 参考：https://developer.steinberg.help/pages/viewpage.action?pageId=49906849
        // My plug-in is capable of processing all possible channel configurations
        // https://developer.steinberg.help/display/VST/Bus+Arrangement+Setting+Sequences
        for (decltype(outputBusCount) i = 0; i < outputBusCount; ++i)
        {
            getBusArrangementResult = audioProcessor_->getBusArrangement(
                Steinberg::Vst::BusDirections::kOutput, i, outputSpeakerArrangements_[i]
            );
            if (getBusArrangementResult == Steinberg::kNotImplemented)
            {
                break;
            }
        }
        if (getBusArrangementResult != Steinberg::kNotImplemented)
        {
            for (decltype(inputBusCount) i = 0; i < inputBusCount; ++i)
            {
                audioProcessor_->getBusArrangement(
                    Steinberg::Vst::BusDirections::kInput, i, inputSpeakerArrangements_[i]
                );
            }
        }
        setBusArrangementsResult = audioProcessor_->setBusArrangements(
            inputSpeakerArrangements_.data(), inputSpeakerArrangements_.size(), outputSpeakerArrangements_.data(),
            outputSpeakerArrangements_.size());
        if (setBusArrangementsResult != Steinberg::kResultOk)
        {
            return false;
        }
    }
    if(getBusArrangementResult == Steinberg::kResultOk)
    {
        for (decltype(inputBusCount) i = 0; i < inputBusCount; ++i)
        {
            audioProcessor_->getBusArrangement(Steinberg::Vst::BusDirections::kInput, i,
                inputSpeakerArrangements_[i]);
            inputs_[i].numChannels = Steinberg::Vst::SpeakerArr::getChannelCount(inputSpeakerArrangements_[i]);
        }
        for (decltype(outputBusCount) i = 0; i < outputBusCount; ++i)
        {
            audioProcessor_->getBusArrangement(Steinberg::Vst::BusDirections::kOutput, i,
                outputSpeakerArrangements_[i]);
            outputs_[i].numChannels = Steinberg::Vst::SpeakerArr::getChannelCount(outputSpeakerArrangements_[i]);
        }
    }
    else
    {
        throw std::runtime_error("");
    }
    // 本人电脑中的 VST3 插件大多不支持获取 RoutingInfo
    // Steinberg::Vst::RoutingInfo inputRoutingInfo = {0, 0, 0};
    // Steinberg::Vst::RoutingInfo outputRoutingInfo = {0, 0, 0};
    // auto getRoutingInfoResult = component_->getRoutingInfo(inputRoutingInfo, outputRoutingInfo);
    Steinberg::Vst::BusInfo busInfo;
    for(int i = 0; i < inputBusCount; ++i)
    {
        auto getBusInfoResult = component_->getBusInfo(Steinberg::Vst::MediaTypes::kAudio,
            Steinberg::Vst::BusDirections::kInput, i, busInfo
        );
        if(audioInputBusIndex_ == -1
        && getBusInfoResult == Steinberg::kResultOk
        && busInfo.busType == Steinberg::Vst::BusTypes::kMain)
        {
            audioInputBusIndex_ = i;
        }
        inputs_[i].numChannels = busInfo.channelCount;
        inputRaws_[i] = SampleTypePointers(busInfo.channelCount, nullptr);
        component_->activateBus(Steinberg::Vst::MediaTypes::kAudio,
            Steinberg::Vst::BusDirections::kInput, i, true);
    }
    for(int i = 0; i < outputBusCount; ++i)
    {
        auto getBusInfoResult = component_->getBusInfo(Steinberg::Vst::MediaTypes::kAudio,
                Steinberg::Vst::BusDirections::kOutput, i, busInfo);
        if(audioOutputBusIndex_ == -1
        && getBusInfoResult == Steinberg::kResultOk
        && busInfo.busType == Steinberg::Vst::BusTypes::kMain)
        {
            audioOutputBusIndex_ = i;
            component_->activateBus(Steinberg::Vst::MediaTypes::kAudio,
                Steinberg::Vst::BusDirections::kOutput, i, true);
        }
        else if(busInfo.flags & Steinberg::Vst::BusInfo::BusFlags::kDefaultActive)
        {
            component_->activateBus(Steinberg::Vst::MediaTypes::kAudio,
                Steinberg::Vst::BusDirections::kOutput, i, true);
        }
        outputs_[i].numChannels = busInfo.channelCount;
        outputRaws_[i] = SampleTypePointers(busInfo.channelCount, nullptr);
    }
    auto inputEventBusCount = component_->getBusCount(Steinberg::Vst::MediaTypes::kEvent,
        Steinberg::Vst::BusDirections::kInput);
    auto outputEventBusCount = component_->getBusCount(Steinberg::Vst::MediaTypes::kEvent,
        Steinberg::Vst::BusDirections::kOutput);
    for(decltype(inputEventBusCount) i = 0; i < inputEventBusCount; ++i)
    {
        auto getBusInfoResult = component_
            ->getBusInfo(Steinberg::Vst::MediaTypes::kEvent, Steinberg::Vst::BusDirections::kInput, i, busInfo);
        if(getBusInfoResult == Steinberg::kResultOk
            && (busInfo.busType == Steinberg::Vst::BusTypes::kMain
                || busInfo.flags & Steinberg::Vst::BusInfo::BusFlags::kDefaultActive))
        {
            auto activateBusResult = component_->activateBus(
                Steinberg::Vst::MediaTypes::kEvent, Steinberg::Vst::BusDirections::kInput, i, true);
            if(activateBusResult != Steinberg::kResultOk)
            {
                return false;
            }
        }
    }
    for(decltype(outputEventBusCount) i = 0; i < outputEventBusCount; ++i)
    {
        auto getBusInfoResult = component_->getBusInfo(Steinberg::Vst::MediaTypes::kEvent,
            Steinberg::Vst::BusDirections::kOutput, i, busInfo);
        if(getBusInfoResult == Steinberg::kResultOk
            && (busInfo.busType == Steinberg::Vst::BusTypes::kMain
                || busInfo.flags & Steinberg::Vst::BusInfo::BusFlags::kDefaultActive))
        {
            auto activateBusResult = component_->activateBus(
                Steinberg::Vst::MediaTypes::kEvent, Steinberg::Vst::BusDirections::kOutput, i, true);
            if(activateBusResult != Steinberg::kResultOk)
            {
                return false;
            }
        }
    }
    audioProcessorStatus_ = VST3AudioProcessorStatus::SetupDone;
    return true;
}

bool VST3Plugin::uninitialize()
{
    if(editControllerStatus_ == VST3EditControllerStatus::Initialized)
    {
        uninitializeEditor();
    }
    audioProcessorStatus_ = VST3AudioProcessorStatus::Initialized;
    audioProcessor_->release();
    auto terminateComponentResult = component_->terminate();
    if(terminateComponentResult != Steinberg::kResultOk)
    {
        throw std::runtime_error("");
    }
    audioProcessorStatus_ = VST3AudioProcessorStatus::Created;
    auto ret = (terminateComponentResult == Steinberg::kResultOk);
    return ret;
}

bool VST3Plugin::initializeEditor()
{
    Steinberg::TUID controllerId;
    if (component_->getControllerClassId(controllerId) == Steinberg::kResultOk)
    {
        auto createEditControllerInstanceResult = factory_->createInstance(
            controllerId, Steinberg::Vst::IEditController::iid,
            reinterpret_cast<void**>(&editController_));
        if(createEditControllerInstanceResult != Steinberg::kResultOk)
        {
            return false;
        }
        else
        {
            editControllerStatus_ = VST3EditControllerStatus::Created;
        }
    }
    else
    {
        auto queryEditorFromComponentResult = component_->queryInterface(Steinberg::Vst::IEditController::iid,
            reinterpret_cast<void**>(&editController_));
        if (queryEditorFromComponentResult == Steinberg::kResultOk)
        {
            editControllerStatus_ = VST3EditControllerStatus::Created;
            effectAndEditorUnified_ = EffectAndEditorUnified::Unified;
        }
    }
    if (editController_)
    {
        editController_->queryInterface(Steinberg::Vst::IEditController2::iid,
            reinterpret_cast<void**>(&editController2_));
        editController_->queryInterface(Steinberg::Vst::IMidiMapping::iid,
            reinterpret_cast<void**>(&midiMapping_));
        editController_->queryInterface(Steinberg::Vst::IEditControllerHostEditing::iid,
            reinterpret_cast<void**>(&editControllerHostEditing_));
        editController_->queryInterface(Steinberg::Vst::INoteExpressionController::iid,
            reinterpret_cast<void**>(&noteExpressionController_));
        editController_->queryInterface(Steinberg::Vst::IKeyswitchController::iid,
            reinterpret_cast<void**>(&keyswitchController_));
        editController_->queryInterface(Steinberg::Vst::IXmlRepresentationController::iid,
            reinterpret_cast<void**>(&xmlRepresentationController_));
        if(effectAndEditorUnified_ != EffectAndEditorUnified::Unified)
        {
            auto initEditControllerResult = editController_->initialize(&Musec::Audio::Host::MusecVST3Host::instance());
            if(initEditControllerResult != Steinberg::kResultOk)
            {
                throw std::runtime_error("");
            }
        }
        editControllerStatus_ = VST3EditControllerStatus::Initialized;
        editController_->setComponentHandler(&componentHandler_);
        component_->queryInterface(Steinberg::Vst::IConnectionPoint::iid,
                                   reinterpret_cast<void**>(&componentPoint_));
        editController_->queryInterface(Steinberg::Vst::IConnectionPoint::iid,
                                        reinterpret_cast<void**>(&editControllerPoint_));
        if(componentPoint_ && editControllerPoint_)
        {
            componentPoint_->addRef();
            editControllerPoint_->addRef();
            // connect ------------------------------------------------------------------------------------------------- connect
            componentPoint_->connect(editControllerPoint_);
            editControllerPoint_->connect(componentPoint_);
            audioProcessorStatus_ = VST3AudioProcessorStatus::Connected;
            editControllerStatus_ = VST3EditControllerStatus::Connected;
        }
        Steinberg::MemoryStream memoryStream;
        if(component_->getState(&memoryStream) == Steinberg::kResultOk)
        {
            editController_->setComponentState(&memoryStream);
        }
        paramCount_ = editController_->getParameterCount();
        paramBlock_ = Musec::Base::FixedSizeMemoryBlock(sizeof(VST3PluginParameter) * paramCount_);
        auto paramBlockAsArray = reinterpret_cast<VST3PluginParameter*>(paramBlock_.data());
        for(decltype(paramCount_) i = 0; i < paramCount_; ++i)
        {
            paramBlockAsArray[i] = VST3PluginParameter(editController_, i);
        }
        view_ = editController_->createView(Steinberg::Vst::ViewType::kEditor);
        if (view_)
        {
            view_->addRef();
            view_->setFrame(&plugFrame_);
        }
    }
    return true;
}

bool VST3Plugin::uninitializeEditor()
{
    detachWithWindow();
    paramBlock_ = Musec::Base::FixedSizeMemoryBlock();
    paramCount_ = 0;
    if(componentPoint_ && editControllerPoint_)
    {
        using Steinberg::Vst::ConnectionProxy;
        componentPoint_->disconnect(editControllerPoint_);
        editControllerPoint_->disconnect(componentPoint_);
        componentPoint_->release();
        editControllerPoint_->release();
        componentPoint_ = nullptr;
        editControllerPoint_ = nullptr;
        editControllerStatus_ = VST3EditControllerStatus::Initialized;
    }
    Steinberg::tresult terminateEditControllerResult = Steinberg::kResultOk;
    if(editController_ && (effectAndEditorUnified_ != EffectAndEditorUnified::Unified))
    {
        terminateEditControllerResult = editController_->terminate();
        if(terminateEditControllerResult != Steinberg::kResultOk)
        {
            throw std::runtime_error("");
        }
    }
    editControllerStatus_ = VST3EditControllerStatus::Created;
    return true;
}

bool VST3Plugin::activate()
{
    auto ret = (component_->setActive(true));
    if(ret == Steinberg::kResultOk
    || ret == Steinberg::kNotImplemented)
    {
        audioProcessorStatus_ = VST3AudioProcessorStatus::Activated;
        return true;
    }
    return false;
}

bool VST3Plugin::deactivate()
{
    auto ret = component_->setActive(false);
    if(ret == Steinberg::kResultOk
    || ret == Steinberg::kNotImplemented)
    {
        audioProcessorStatus_ = VST3AudioProcessorStatus::SetupDone;
        return true;
    }
    return false;
}

// 或许上 RAII 更合适？
bool VST3Plugin::startProcessing()
{
    if(audioProcessorStatus_ == VST3AudioProcessorStatus::Activated)
    {
        auto ret = audioProcessor_->setProcessing(true);
        if(ret == Steinberg::kResultOk
        // VST3 SDK 中，IAudioProcessor 的实现 AudioEffect
        // 在调用此函数时只返回 kNotImplemented。
        // 一些插件厂商直接套用了 AudioEffect 原来的实现，并且没有改动此处的代码。
        // Steinberg 的文档中没有提醒开发者“需要改动这一处函数”。
        || ret == Steinberg::kNotImplemented)
        {
            audioProcessorStatus_ = VST3AudioProcessorStatus::Processing;
            return true;
        }
    }
    return false;
}

// 或许上 RAII 更合适？
bool VST3Plugin::stopProcessing()
{
    if(audioProcessorStatus_ == VST3AudioProcessorStatus::Processing)
    {
        auto ret = audioProcessor_->setProcessing(false);
        if(ret == Steinberg::kResultOk
        // 参阅 startProcessing 中的说明
        || ret == Steinberg::kNotImplemented)
        {
            audioProcessorStatus_ = VST3AudioProcessorStatus::Activated;
            return true;
        }
    }
    return false;
}

const SpeakerArrangements& VST3Plugin::inputSpeakerArrangements()
{
    return inputSpeakerArrangements_;
}

const SpeakerArrangements& VST3Plugin::outputSpeakerArrangements()
{
    return outputSpeakerArrangements_;
}

bool VST3Plugin::attachToWindow(QWindow* window)
{
    if(view_)
    {
        window_ = window;
        Steinberg::ViewRect viewRect;
        view_->getSize(&viewRect);
        window_->setPosition(viewRect.left, viewRect.top);
        window_->setWidth(viewRect.getWidth());
        window_->setHeight(viewRect.getHeight());
        window_->setTitle(classInfo_.name);
        view_->attached(reinterpret_cast<HWND>(window_->winId()), Steinberg::kPlatformTypeHWND);
        Musec::Controller::AudioEngineController::AppProject().addPluginWindowMapping(audioProcessor_, window_);
        QObject::connect(window_, &QWindow::widthChanged,
            [this](int) { onWindowSizeChanged(); });
        QObject::connect(window_, &QWindow::heightChanged,
            [this](int) { onWindowSizeChanged(); });
        return true;
    }
    return false;
}

bool VST3Plugin::detachWithWindow()
{
    if(!window_)
    {
        return true;
    }
    if(view_)
    {
        view_->removed();
        Musec::Controller::AudioEngineController::AppProject().removePluginWindowMapping(audioProcessor_);
        window_ = nullptr;
        return true;
    }
    return false;
}

const Steinberg::PClassInfo& VST3Plugin::getClassInfo() const
{
    return classInfo_;
}

Steinberg::IPluginFactory* VST3Plugin::factory() const
{
    return factory_;
}

Steinberg::IPlugView* VST3Plugin::getView() const
{
    return view_;
}

void VST3Plugin::onWindowSizeChanged()
{
    if(view_)
    {
        auto x = window_->x();
        auto y = window_->y();
        auto width = window_->width();
        auto height = window_->height();
        auto newRect = Steinberg::ViewRect(x, y, x + width, y + height);
        auto oldRect = Steinberg::ViewRect(); view_->getSize(&oldRect);
        if(view_->checkSizeConstraint(&newRect) != Steinberg::kNotImplemented)
        {
            window_->setX(newRect.left);
            window_->setY(newRect.top);
            window_->setWidth(newRect.getWidth());
            window_->setHeight(newRect.getHeight());
            auto onSizeResult = view_->onSize(&newRect);
        }
        view_->getSize(&newRect);
        window_->setWidth(newRect.getWidth());
        window_->setHeight(newRect.getHeight());
    }
}

bool VST3Plugin::getBypass() const
{
    return this->audioProcessorStatus_ < VST3AudioProcessorStatus::Processing;
}

 QString VST3Plugin::getName() const
{
    if(!factory_)
    {
        return QString();
    }
    return QString(getClassInfo().name);
}

 QWindow* VST3Plugin::window()
{
    return window_;
}

 bool VST3Plugin::hasUI()
{
    return view_;
}

Musec::Base::PluginFormat VST3Plugin::pluginFormat()
{
    return Musec::Base::PluginFormat::FormatVST3;
}

bool VST3Plugin::activated()
{
    return audioProcessorStatus_ >= VST3AudioProcessorStatus::Activated;
}

int VST3Plugin::parameterCount()
{
    return paramCount_;
}

IParameter& VST3Plugin::parameter(int index)
{
    return reinterpret_cast<VST3PluginParameter*>(paramBlock_.data())[index];
}

bool VST3Plugin::processing()
{
    return audioProcessorStatus_ >= VST3AudioProcessorStatus::Processing;
}
}
