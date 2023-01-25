#include "VST3Plugin.hpp"

#include "audio/host/VST3Host.hpp"
#include "controller/AudioEngineController.hpp"
#include "native/Native.hpp"

#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/vst/vstspeaker.h>

// For some reason, memorystream.cpp is not included in sdk_common library, so I have to solve this
// by `#include`ing the source file in another source: `audio/plugin/VST3MemoryStream.cpp`.
#include <public.sdk/source/common/memorystream.h>

#include <errhandlingapi.h>

#include <algorithm>
#include <stdexcept>

namespace Musec::Audio::Plugin
{
// VST3Plugin ctor & dtor
// ------------------------------------------------------------------------------------------

VST3Plugin::VST3Plugin(const QString& path):
    VST3Plugin::Base(path), componentHandler_(this), plugFrame_(this)
{
    if(errorCode())
    {
        throw std::runtime_error("");
    }
    auto pluginInitProc = Musec::Native::Library::getExport<PluginInitProc>(VST3PluginInitName);
    auto pluginFactoryProc = Musec::Native::Library::getExport<PluginFactoryProc>("GetPluginFactory");
    if (!pluginFactoryProc)
    {
        throw std::runtime_error("Error: VST3 Plugin entry not found! This might be not a VST3 plugin.");
    }
    if (pluginInitProc)
    {
        // Init and exit call could be omitted on Windows, but not macOS and Linux
        // https://steinbergmedia.github.io/vst3_dev_portal/pages/Technical+Documentation/VST+Module+Architecture/Loading.html
        if (!pluginInitProc())
        {
            throw std::runtime_error("");
        }
    }
    factory_ = pluginFactoryProc();
    audioProcessorStatus_ = VST3AudioProcessorStatus::Factory;
    editControllerStatus_ = VST3EditControllerStatus::Factory;
}

bool VST3Plugin::createPlugin(const Steinberg::TUID& uid)
{
    if(factory_)
    {
        if(auto createComponentResult = factory_->createInstance(
                uid, Steinberg::Vst::IComponent::iid,
                reinterpret_cast<void**>(&component_)
            );
            createComponentResult == Steinberg::kResultOk)
        {
            audioProcessorStatus_ = VST3AudioProcessorStatus::Created;
            return true;
        }
    }
    return false;
}

VST3Plugin::VST3Plugin(const QString& path, const Steinberg::TUID& uid):
    VST3Plugin(path)
{
    if(!createPlugin(uid))
    {
        throw std::runtime_error("");
    }
}

VST3Plugin::~VST3Plugin()
{
    if(editControllerStatus_ == VST3EditControllerStatus::Connected)
    {
        uninitializeEditController();
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
        component_ = nullptr;
        audioProcessorStatus_ = VST3AudioProcessorStatus::Factory;
    }
    if(editControllerStatus_ == VST3EditControllerStatus::Created)
    {
        editController_->release();
        editController_ = nullptr;
        editControllerStatus_ = VST3EditControllerStatus::Factory;
    }
    if (factory_)
    {
        factory_->release();
        factory_ = nullptr;
        audioProcessorStatus_ = VST3AudioProcessorStatus::NoAudioProcessor;
        editControllerStatus_ = VST3EditControllerStatus::NoEditController;
        auto pluginExitProc = Musec::Native::Library::getExport<PluginExitProc>(VST3PluginExitName);
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
std::uint8_t VST3Plugin::audioInputCount() const
{
    std::uint8_t ret = 0;
    for (auto& arrangement: inputSpeakerArrangements_)
    {
        ret += Steinberg::Vst::SpeakerArr::getChannelCount(arrangement);
    }
    return ret;
}

std::uint8_t VST3Plugin::audioOutputCount() const
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
    auto& processContext = Musec::Audio::Host::VST3Host::AppProcessContext();
    processData_.processContext = &processContext;
    audioProcessor_->process(processData_);
    // FIXME: `IEditController` member functions can only be called in the UI process
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
    //         std::thread([=]() { editController_->setParamNormalized(paramId, paramValue); }).detach();
    //     }
    // }
}

void VST3Plugin::process(const Musec::Audio::Device::AudioProcessData<SampleType>& audioProcessData)
{
    processData_.numSamples = audioProcessData.singleBufferSize;
    for(int i = 0; i < processData_.numInputs; ++i)
    {
        processData_.inputs[i].channelBuffers32 = audioProcessData.inputs[i];
    }
    for(int i = 0; i < processData_.numOutputs; ++i)
    {
        processData_.outputs[i].channelBuffers32 = audioProcessData.outputs[i];
    }
    audioProcessor_->process(processData_);
}

// ------------------------------------------------------------------------------------------

// VST3Plugin IPlugin interfaces
// ------------------------------------------------------------------------------------------
bool VST3Plugin::initialize(double sampleRate, std::int32_t maxSampleCount)
{
    auto initializeComponentResult = component_->initialize(&Musec::Audio::Host::VST3Host::instance());
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
    initializeEditController();
    audioProcessor_->queryInterface(Steinberg::Vst::IProcessContextRequirements::iid,
        reinterpret_cast<void**>(&processContextRequirements_));
    audioProcessor_->queryInterface(Steinberg::Vst::IAudioPresentationLatency::iid,
        reinterpret_cast<void**>(&audioPresentationLatency_));
    // ProcessSetup -------------------------------------------------------------------------
    processSetup_.processMode = Steinberg::Vst::ProcessModes::kRealtime;
    processSetup_.symbolicSampleSize = Steinberg::Vst::SymbolicSampleSizes::kSample32;
    processSetup_.maxSamplesPerBlock = maxSampleCount;
    processSetup_.sampleRate = sampleRate;
    // ProcessSetup -------------------------------------------------------------------------
    // ProcessData --------------------------------------------------------------------------
    processData_.inputParameterChanges = &inputParameterChanges_;
    processData_.outputParameterChanges = &outputParameterChanges_;
    processData_.processMode = processSetup_.processMode;
    processData_.symbolicSampleSize = processSetup_.symbolicSampleSize;
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
    auto setupProcessingResult = audioProcessor_->setupProcessing(processSetup_);
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
    // Just gave up negotiating speaker arrangements
    // Zebra2 and Zebralette simply return `Steinberg::kResultFalse` even if we set the speaker
    // arrangement to what we've retrieved using `getBusArrangement`
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
    // Many VST3 plugins I'm using don't support this
    // Steinberg::Vst::RoutingInfo inputRoutingInfo = {0, 0, 0};
    // Steinberg::Vst::RoutingInfo outputRoutingInfo = {0, 0, 0};
    // auto getRoutingInfoResult = component_->getRoutingInfo(inputRoutingInfo, outputRoutingInfo);
    Steinberg::Vst::BusInfo busInfo;
    inputSpeakerGroupCollection_ = {component_, audioProcessor_, Steinberg::Vst::BusDirections::kInput};
    outputSpeakerGroupCollection_ = {component_, audioProcessor_, Steinberg::Vst::BusDirections::kOutput};
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
        uninitializeEditController();
    }
    inputSpeakerGroupCollection_ = {};
    outputSpeakerGroupCollection_ = {};
    audioProcessorStatus_ = VST3AudioProcessorStatus::Initialized;
    if(audioPresentationLatency_)
    {
        audioPresentationLatency_->release();
        audioPresentationLatency_ = nullptr;
    }
    if(processContextRequirements_)
    {
        processContextRequirements_->release();
        processContextRequirements_ = nullptr;
    }
    audioProcessor_->release();
    audioProcessor_ = nullptr;
    auto terminateComponentResult = component_->terminate();
    if(terminateComponentResult != Steinberg::kResultOk)
    {
        throw std::runtime_error("");
    }
    audioProcessorStatus_ = VST3AudioProcessorStatus::Created;
    auto ret = (terminateComponentResult == Steinberg::kResultOk);
    return ret;
}

bool VST3Plugin::initializeEditController()
{
    Steinberg::TUID controllerId;
    if (auto getControllerResult = component_->getControllerClassId(controllerId);
        getControllerResult == Steinberg::kResultOk)
    {
        auto createEditControllerInstanceResult = factory_->createInstance(
            controllerId, Steinberg::Vst::IEditController::iid,
            reinterpret_cast<void**>(&editController_));
        // Some plugins will simply return `kNoInterface` here, but querying interface from
        // the `IComponent` will retrieve the `IEditController` correctly.
        if(createEditControllerInstanceResult == Steinberg::kResultOk)
        {
            editControllerStatus_ = VST3EditControllerStatus::Created;
        }
    }
    if(!editController_)
    {
        auto queryEditorFromComponentResult = component_->queryInterface(Steinberg::Vst::IEditController::iid,
            reinterpret_cast<void**>(&editController_));
        if (queryEditorFromComponentResult == Steinberg::kResultOk)
        {
            editControllerStatus_ = VST3EditControllerStatus::Created;
            effectAndEditorUnified_ = EffectAndEditorUnified::Unified;
        }
        else
        {
            return false;
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
            auto initEditControllerResult = editController_->initialize(&Musec::Audio::Host::VST3Host::instance());
            if(initEditControllerResult != Steinberg::kResultOk)
            {
                // Initialization of edit controller failed. This means that
                // - The UI cannot be initialized (not a big deal)
                // - Information of parameters cannot be retrieved (that is not right!)
                return false;
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
            new(paramBlockAsArray + i) VST3PluginParameter(editController_, i);
            auto name = paramBlockAsArray[i].name();
        }
        auto firstHiddenParameter = std::stable_partition<VST3PluginParameter*>(
            paramBlockAsArray,
            paramBlockAsArray + paramCount_,
            [](VST3PluginParameter& parameter) { return (parameter.flags() & ParameterFlags::Hidden) == 0; });
            // [](VST3PluginParameter& parameter) { return (parameter.getParameterInfo().flags & ParameterInfo::ParameterFlags::kIsHidden) == 0; });
        auto visibleParameterCount = firstHiddenParameter - paramBlockAsArray;
        try
        {
            view_ = editController_->createView(Steinberg::Vst::ViewType::kEditor);
            if (view_)
            {
                view_->setFrame(&plugFrame_);
            }
            else
            {
                //
            }
        }
        catch(...)
        {
        }
    }
    return true;
}

bool VST3Plugin::uninitializeEditController()
{
    detachWithWindow();
    if(editController2_) { editController2_->release(); editController2_ = nullptr; }
    if(midiMapping_) { midiMapping_->release(); midiMapping_ = nullptr; }
    if(editControllerHostEditing_) { editControllerHostEditing_->release(); editControllerHostEditing_ = nullptr; }
    if(noteExpressionController_) { noteExpressionController_->release(); noteExpressionController_ = nullptr; }
    if(keyswitchController_) { keyswitchController_->release(); keyswitchController_ = nullptr; }
    if(xmlRepresentationController_) { xmlRepresentationController_->release(); xmlRepresentationController_ = nullptr; }
    auto paramBlockAsArray = reinterpret_cast<VST3PluginParameter*>(paramBlock_.data());
    for(decltype(paramCount_) i = 0; i < paramCount_; ++i)
    {
        (paramBlockAsArray + i)->~VST3PluginParameter();
    }
    paramBlock_ = Musec::Base::FixedSizeMemoryBlock();
    paramCount_ = 0;
    if(componentPoint_ && editControllerPoint_)
    {
        componentPoint_->disconnect(editControllerPoint_);
        editControllerPoint_->disconnect(componentPoint_);
        componentPoint_->release();
        componentPoint_ = nullptr;
        editControllerPoint_->release();
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

void VST3Plugin::initializeWindowSizeConnections()
{
    windowSizeConnections_[0] = QObject::connect(window_, &QWindow::widthChanged,
        [this](int) { onWindowSizeChanged(); });
    windowSizeConnections_[1] = QObject::connect(window_, &QWindow::heightChanged,
        [this](int) { onWindowSizeChanged(); });
}

void VST3Plugin::uninitializeWindowSizeConnections()
{
    QObject::disconnect(windowSizeConnections_[0]);
    QObject::disconnect(windowSizeConnections_[1]);
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

// RAII might be better?
bool VST3Plugin::startProcessing()
{
    if(audioProcessorStatus_ == VST3AudioProcessorStatus::Activated)
    {
        auto ret = audioProcessor_->setProcessing(true);
        if(ret == Steinberg::kResultOk
        // In VST3 SDK, this function of `AudioEffect`, the implementation of `IAudioProcessor`
        // simply returns `kNotImplemented`.
        // Some plugin vendors uses that implementation without changing the return value.
        // Seems like many host applications just take this return value as a successful call.
        || ret == Steinberg::kNotImplemented)
        {
            audioProcessorStatus_ = VST3AudioProcessorStatus::Processing;
            for(int i = 0; i < inputs_.size(); ++i)
            {
                inputs_[i].numChannels = inputSpeakerGroupCollection_.speakerGroupAt(i).speakerCount();
            }
            for(int i = 0; i < outputs_.size(); ++i)
            {
                outputs_[i].numChannels = outputSpeakerGroupCollection_.speakerGroupAt(i).speakerCount();
            }
            processData_.numInputs = inputs_.size();
            processData_.numOutputs = outputs_.size();
            processData_.inputs = inputs_.data();
            processData_.outputs = outputs_.data();
            return true;
        }
    }
    return false;
}

// RAII might be better?
bool VST3Plugin::stopProcessing()
{
    if(audioProcessorStatus_ == VST3AudioProcessorStatus::Processing)
    {
        auto ret = audioProcessor_->setProcessing(false);
        if(ret == Steinberg::kResultOk
        // See comments in `startProcessing`
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
        Steinberg::ViewRect viewRect;
        // Some plugins cannot return a correct size here
        window_ = window;
        view_->attached(reinterpret_cast<Musec::Native::WindowType>(window_->winId()), Steinberg::kPlatformTypeHWND);
        Musec::Controller::AudioEngineController::AppProject().addPluginWindowMapping(audioProcessor_, window_);
        initializeWindowSizeConnections();
        view_->getSize(&viewRect);
        window_->setWidth(viewRect.getWidth());
        window_->setHeight(viewRect.getHeight());
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
        uninitializeWindowSizeConnections();
        view_->removed();
        Musec::Controller::AudioEngineController::AppProject().removePluginWindowMapping(audioProcessor_);
        window_ = nullptr;
        view_->release();
        view_ = nullptr;
        return true;
    }
    return false;
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
        uninitializeWindowSizeConnections();
        auto width = window_->width();
        auto height = window_->height();
        // Set `left` and `top` to zero here, so that `right` and `bottom` is width and height
        // respectively.
        // I don't set `left` and `top` to the position of the window, because
        // some plugin vendors will treat `right` and `bottom` as width and height (e.g.
        // plugins made by DISTRHO Plugin Framework (https://github.com/DISTRHO/DPF)) and completely
        // ignores `left` and `right` in `IPlugView::checkSizeConstraint`.
        auto newRect = Steinberg::ViewRect(0, 0, width, height);
        auto oldRect = Steinberg::ViewRect();
        view_->getSize(&oldRect);
        assert(newRect.top <= newRect.bottom && newRect.left <= newRect.right);
        // QUES: If the plugin supports checking size constraint, and the function tweaked the rectangle
        // passed in, what should the function return? `Steinberg::kResultFalse`?
        auto checkSizeConstraintResult = view_->checkSizeConstraint(&newRect);
        // Some plugins can make this nonsense.
        if (newRect.top > newRect.bottom || newRect.left > newRect.right)
        {
            window_->setWidth(width);
            window_->setHeight(height);
            initializeWindowSizeConnections();
            return;
        }
        if(checkSizeConstraintResult != Steinberg::kNotImplemented)
        {
            window_->setWidth(newRect.getWidth());
            window_->setHeight(newRect.getHeight());
            auto onSizeResult = view_->onSize(&newRect);
            if(onSizeResult == Steinberg::kInvalidArgument)
            {
                window_->setWidth(width);
                window_->setHeight(height);
            }
            initializeWindowSizeConnections();
            return;
        }
        view_->getSize(&newRect);
        window_->setWidth(newRect.getWidth());
        window_->setHeight(newRect.getHeight());
        initializeWindowSizeConnections();
    }
}

bool VST3Plugin::getBypass() const
{
    return this->audioProcessorStatus_ < VST3AudioProcessorStatus::Processing;
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

int VST3Plugin::latency()
{
    return audioProcessor_? audioProcessor_->getLatencySamples(): 0;
}

bool VST3Plugin::processing()
{
    return audioProcessorStatus_ >= VST3AudioProcessorStatus::Processing;
}

const ISpeakerGroupCollection& VST3Plugin::audioInputSpeakerGroupCollection() const
{
    return inputSpeakerGroupCollection_;
}

const ISpeakerGroupCollection& VST3Plugin::audioOutputSpeakerGroupCollection() const
{
    return outputSpeakerGroupCollection_;
}

Steinberg::Vst::ProcessSetup VST3Plugin::processSetup()
{
    return processSetup_;
}
}
