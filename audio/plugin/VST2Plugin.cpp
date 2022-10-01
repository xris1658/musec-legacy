#include "VST2Plugin.hpp"

#include "audio/plugin/VST2PluginParameter.hpp"
#include "audio/plugin/VST2PluginShellPluginId.hpp"
#include "audio/plugin/VST2SpeakerGroupCollection.hpp"
#include "controller/AudioEngineController.hpp"
#include "base/Constants.hpp"
#include "base/PluginBase.hpp"
#include "native/Native.hpp"

#include <errhandlingapi.h>

#include <array>
#include <cstdlib>

// namespace `HostCanDos` is present only in the source file,
// but the function using contents in this namespace is at elsewhere.
// The following namespace is used to deal with ODR.
namespace VST2AudioEffectX
{
#pragma warning(push)
#pragma warning(disable: 4458) // Shadows the local declaration
#pragma warning(disable: 4100) // Unreferenced parameter
#include <public.sdk/source/vst2.x/audioeffect.h>
#include <public.sdk/source/vst2.x/audioeffectx.h>
#include <public.sdk/source/vst2.x/audioeffect.cpp>
#include <public.sdk/source/vst2.x/audioeffectx.cpp>
#pragma warning(pop)
}

namespace Musec::Audio::Plugin
{
namespace Impl
{
bool stringEqual(const char* lhs, const char* rhs)
{
    return std::strcmp(lhs, rhs) == 0;
}
}

VstIntPtr VSTCALLBACK pluginVST2Callback(
    AEffect* effect,
    VstInt32 opcode,
    [[maybe_unused]] VstInt32 index,
    [[maybe_unused]] VstIntPtr value,
    [[maybe_unused]] void* ptr,
    [[maybe_unused]] float opt)
{
    VstIntPtr ret = 0;
    switch (opcode)
    {
    // AudioMasterOpcodes (aeffect.h)

    // A parameter of the plugin is adjusted via MIDI or GUI
    // index: parameter index;
    // opt: value of the parameter
    case audioMasterAutomate:
        break;
    // VST version used by the host application
    case audioMasterVersion:
        ret = kVstVersion; // VST2.4
        break;
    // Returns the sub plugin ID while a VST2 Shell plugin is being loaded
    case audioMasterCurrentId:
        ret = (VST2PluginShellPluginId::instance().getId() == 0) && (!(VST2PluginShellPluginId::instance().idShouldBeZero()))?
              effect->uniqueID:
              VST2PluginShellPluginId::instance().getId();
        break;
    case audioMasterIdle:
        effect->dispatcher(effect, AEffectOpcodes::effEditIdle, 0, 0, nullptr, 0.0L);
        break;
#if kVstVersion < 2400
        case audioMasterPinConnected:
            break;
#endif
        // AudioMasterOpcodesX
#if kVstVersion < 2400
        case audioMasterWantMidi:
            break;
#endif
    // Returns VstTimeInfo*
    case audioMasterGetTime:
        // TODO
        ret = NULL;
        break;
    // ptr: pointer to VstEvents
    case audioMasterProcessEvents:
    {
        // auto events = reinterpret_cast<VstEvents*>(ptr);
        // TODO
        break;
    }
#if kVstVersion < 2400
        case audioMasterSetTime:
            break;
        case audioMasterTempoAt:
            break;
        case audioMasterGetNumAutomatableParameters:
            break;
        case audioMasterGetParameterQuantization:
            break;
#endif
    // Returns 1 if supported
    case audioMasterIOChanged:
        ret = 1;
        break;
#if kVstVersion < 2400
        case audioMasterNeedIdle:
            break;
#endif
    // index: width; value: height; returns 1 if supported
    case audioMasterSizeWindow:
        Musec::Controller::AudioEngineController::AppProject().setPluginWindowSize(effect, index, value);
        ret = 1;
        break;
    case audioMasterGetSampleRate:
        ret = Musec::Controller::AudioEngineController::getCurrentSampleRate();
        break;
    case audioMasterGetBlockSize:
        ret = Musec::Controller::AudioEngineController::getCurrentBlockSize();
        break;
    case audioMasterGetInputLatency:
        ret = Musec::Controller::AudioEngineController::getInputLatency();
        break;
    case audioMasterGetOutputLatency:
        ret = Musec::Controller::AudioEngineController::getOutputLatency();
        break;
#if kVstVersion < 2400
        case audioMasterGetPreviousPlug:
            break;
        case audioMasterGetNextPlug:
            break;
        case audioMasterWillReplaceOrAccumulate:
            break;
#endif
    // Returns VstProcessLevels
    case audioMasterGetCurrentProcessLevel:
        ret = *reinterpret_cast<VstProcessLevels*>(effect + 1);
        break;
    // Returns VstAutomationStates
    case audioMasterGetAutomationState:
        // TODO
        ret = VstAutomationStates::kVstAutomationUnsupported;
        break;
    case audioMasterOfflineStart:
        break;
    case audioMasterOfflineRead:
        break;
    case audioMasterOfflineWrite:
        break;
    case audioMasterOfflineGetCurrentPass:
        break;
    case audioMasterOfflineGetCurrentMetaPass:
        break;
#if kVstVersion < 2400
        case audioMasterSetOutputSampleRate:
            break;
        case audioMasterGetOutputSpeakerArrangement:
            break;
#endif
    // ptr: Character buffer
    case audioMasterGetVendorString:
    {
        constexpr int vendorNameLength = sizeof(Musec::Base::CompanyName) + 1;
        std::strncpy(reinterpret_cast<char*>(ptr), Musec::Base::CompanyName, vendorNameLength);
        break;
    }
    // ditto
    case audioMasterGetProductString:
    {
        constexpr int productNameLength = sizeof(Musec::Base::ProductName) + 1;
        std::strncpy(reinterpret_cast<char*>(ptr), Musec::Base::ProductName, productNameLength);
        break;
    }
    case audioMasterGetVendorVersion:
        ret = 1;
        break;
    case audioMasterVendorSpecific:
        break;
#if kVstVersion < 2400
        case audioMasterSetIcon:
            break;
#endif
    // Compare with strings in HostCanDos
    case audioMasterCanDo:
    {
        using namespace VST2AudioEffectX::HostCanDos;
        using namespace Impl;
        auto canDo = reinterpret_cast<const char*>(ptr);
        if(stringEqual(canDo, canDoSendVstEvents))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoSendVstMidiEvent))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoSendVstTimeInfo))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoReceiveVstEvents))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoReceiveVstMidiEvent))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoReportConnectionChanges))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoAcceptIOChanges))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoSizeWindow))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoOffline))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoOpenFileSelector))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoCloseFileSelector))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoStartStopProcess))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoShellCategory))
        {
            ret = 1;
        }
        else if(stringEqual(canDo, canDoSendVstMidiEventFlagIsRealtime))
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
        break;
    }
    case audioMasterGetLanguage:
        ret = VstHostLanguage::kVstLangEnglish;
        break;
#if kVstVersion < 2400
        case audioMasterOpenWindow:
        break;
    case audioMasterCloseWindow:
        break;
#endif
    // Get the directory of the plugin
    case audioMasterGetDirectory:
        break;
    case audioMasterUpdateDisplay:
        effect->dispatcher(effect, AEffectOpcodes::effEditIdle, 0, 0, nullptr, 0.0);
        break;
    case audioMasterBeginEdit:
        break;
    case audioMasterEndEdit:
        break;
    case audioMasterOpenFileSelector:
        break;
    case audioMasterCloseFileSelector:
        break;
#if kVstVersion < 2400
        case audioMasterEditFile:
            break;
        case audioMasterGetChunkFile:
            break;
        case audioMasterGetInputSpeakArrangement:
            break;
#endif
    default:
        break;
    }
    return ret;
}

VST2Plugin::VST2Plugin(const QString& path, bool scanPlugin, VstInt32 shellPluginId):
    VST2Plugin::Base(path)
{
    auto pluginEntryProc = Musec::Native::getExport<VST2PluginEntryProc>(*this, "VSTPluginMain");
    if(!pluginEntryProc)
    {
        pluginEntryProc = Musec::Native::getExport<VST2PluginEntryProc>(*this, "main");
        if(!pluginEntryProc)
        {
            throw GetLastError();
        }
    }
    if(scanPlugin)
    {
        effect_ = VST2PluginShellPluginId::instance().getShellPlugin(0, false, pluginEntryProc);
        effect_->dispatcher(effect_, effOpen, 0, 0, nullptr, 0);
        auto category = effect_->dispatcher(effect_, effGetPlugCategory, 0, 0, nullptr, 0);
        if(category == kPlugCategUnknown)
        {
            effect_->dispatcher(effect_, effClose, 0, 0, nullptr, 0);
            effect_ = VST2PluginShellPluginId::instance().getShellPlugin(0, true, pluginEntryProc);
        }
    }
    else if(shellPluginId == 0)
    {
        effect_ = pluginEntryProc(pluginVST2Callback);
    }
    else
    {
        effect_ = VST2PluginShellPluginId::instance().getShellPlugin(shellPluginId, true, pluginEntryProc);
    }
}

VST2Plugin::~VST2Plugin()
{
    if(effect_)
    {
        if(VST2Plugin::hasUI())
        {
            uninitializeEditor();
        }
        VST2Plugin::stopProcessing();
        VST2Plugin::deactivate();
        VST2Plugin::uninitialize();
    }
}

AEffect* VST2Plugin::effect() const
{
    return effect_;
}

std::uint8_t VST2Plugin::audioInputCount() const
{
    return static_cast<std::uint8_t>(effect_->numInputs);
}

std::uint8_t VST2Plugin::audioOutputCount() const
{
    return static_cast<std::uint8_t>(effect_->numOutputs);
}

bool VST2Plugin::initialize(double sampleRate, std::int32_t maxSampleCount)
{
    if(auto version = effect_->dispatcher(effect_, AEffectXOpcodes::effGetVstVersion, 0, 0, nullptr, 0.0f);
        version >= 2300)
    {
        VstSpeakerArrangement* inputSpeakerArrangement = nullptr;
        VstSpeakerArrangement* outputSpeakerArrangement = nullptr;
        if(auto getSpeakerArrangementResult = effect_->dispatcher(
            effect_, AEffectXOpcodes::effGetSpeakerArrangement, 0,
            ToVstPtr(&inputSpeakerArrangement), &outputSpeakerArrangement, 0.0f);
            getSpeakerArrangementResult)
        {
            if(outputSpeakerArrangement
            && outputSpeakerArrangement->type != VstSpeakerArrangementType::kSpeakerArrStereo)
            {
                outputSpeakerArrangement->type = VstSpeakerArrangementType::kSpeakerArrStereo;
                outputSpeakerArrangement->numChannels = 2;
                outputSpeakerArrangement->speakers[0].type = VstSpeakerType::kSpeakerL;
                outputSpeakerArrangement->speakers[1].type = VstSpeakerType::kSpeakerR;
                if(!(effect_->flags & VstAEffectFlags::effFlagsIsSynth))
                {
                    if(inputSpeakerArrangement
                    && inputSpeakerArrangement->type != VstSpeakerArrangementType::kSpeakerArrStereo)
                    {
                        inputSpeakerArrangement->type = VstSpeakerArrangementType::kSpeakerArrStereo;
                        inputSpeakerArrangement->numChannels = 2;
                        inputSpeakerArrangement->speakers[0].type = VstSpeakerType::kSpeakerL;
                        inputSpeakerArrangement->speakers[1].type = VstSpeakerType::kSpeakerR;
                    }
                }
                auto setSpeakerArrangementResult = effect_->dispatcher(
                    effect_, AEffectXOpcodes::effSetSpeakerArrangement, 0, ToVstPtr(&inputSpeakerArrangement),
                    &outputSpeakerArrangement, 0.0f);
                if(!setSpeakerArrangementResult)
                {
                    return false;
                }
            }
            // effGetSpeakerArrangement can return multiple speaker arrangements.
            // TODO: How to get the number of speaker arrangements?
            inputSpeakerGroupCollection_ = {inputSpeakerArrangement, 1};
            outputSpeakerGroupCollection_ = {outputSpeakerArrangement, 1};
        }
    }
    effect_->dispatcher(effect_, effOpen, 0, 0, nullptr, 0);
    inputsRaw_ = std::vector<SampleType*>(VST2Plugin::audioInputCount(), nullptr);
    outputsRaw_ = std::vector<SampleType*>(VST2Plugin::audioOutputCount(), nullptr);
    effect_->dispatcher(effect_, AEffectOpcodes::effSetSampleRate, 0, 0, nullptr, sampleRate);
    effect_->dispatcher(effect_, AEffectOpcodes::effSetBlockSize, 0, maxSampleCount, nullptr, 0);
    initializeEditor();
    auto paramCount = parameterCount();
    paramBlock_ = decltype(paramBlock_)(sizeof(VST2PluginParameter) * paramCount);
    auto paramBlockAsArray = reinterpret_cast<VST2PluginParameter*>(paramBlock_.data());
    for(decltype(paramCount) i = 0; i < paramCount; ++i)
    {
        auto& param = paramBlockAsArray[i];
        param = VST2PluginParameter(*this, i);
    }
    return true;
}

bool VST2Plugin::uninitialize()
{
    inputSpeakerGroupCollection_ = {};
    outputSpeakerGroupCollection_ = {};
    paramBlock_ = Musec::Base::FixedSizeMemoryBlock();
    uninitializeEditor();
    stopProcessing();
    deactivate();
    effect_->dispatcher(effect_, effClose, 0, 0, nullptr, 0);
    return true;
}

bool VST2Plugin::activate()
{
    effect_->dispatcher(effect_, AEffectOpcodes::effMainsChanged, 0, 1, nullptr, 0);
    activated_ = true;
    return true;
}

bool VST2Plugin::deactivate()
{
    effect_->dispatcher(effect_, AEffectOpcodes::effMainsChanged, 0, 0, nullptr, 0);
    activated_ = false;
    return true;
}

bool VST2Plugin::startProcessing()
{
    effect_->dispatcher(effect_, AEffectXOpcodes::effStartProcess, 0, 0, nullptr, 0);
    bypass_ = false;
    return true;
}

bool VST2Plugin::stopProcessing()
{
    effect_->dispatcher(effect_, AEffectXOpcodes::effStopProcess, 0, 0, nullptr, 0);
    bypass_ = true;
    return true;
}

void VST2Plugin::process(Musec::Audio::Base::AudioBufferView<SampleType>* inputs, int inputCount,
    Musec::Audio::Base::AudioBufferView<SampleType>* outputs, int outputCount)
{
    processLevel_ = VstProcessLevels::kVstProcessLevelRealtime;
    for(int i = 0; i < inputCount; ++i)
    {
        inputsRaw_[i] = inputs[i].getSamples();
        if(inputsRaw_.size() > inputCount)
        {
            inputsRaw_[i + inputCount] = inputs[i].getSamples();
        }
    }
    for(int i = inputCount; i < this->audioInputCount(); ++i)
    {
        inputsRaw_[i] = Musec::Controller::AudioEngineController::dummyBufferView<SampleType>().getSamples();
    }
    for(int i = 0; i < outputCount && i < this->audioOutputCount(); ++i)
    {
        outputsRaw_[i] = outputs[i].getSamples();
    }
    for(int i = outputCount; i < effect_->numOutputs; ++i)
    {
        outputsRaw_[i] = Musec::Controller::AudioEngineController::dummyBufferView<SampleType>().getSamples();
    }
    std::int32_t sampleCount = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    effect_->processReplacing(effect_, inputsRaw_.data(), outputsRaw_.data(), sampleCount);
    processLevel_ = VstProcessLevels::kVstProcessLevelUser;
}

bool VST2Plugin::initializeEditor()
{
    return effect_->flags & VstAEffectFlags::effFlagsHasEditor;
}

bool VST2Plugin::uninitializeEditor()
{
    return detachWithWindow();
}

bool VST2Plugin::getBypass() const
{
    return bypass_;
}

QString VST2Plugin::getName() const
{
    if(!effect_)
    {
        return QString();
    }
    // Sylenth1 and Ableton SAK write `kVstMaxEffectNameLen + 1` characters, null terminator included
    // The maximum lengths in VST2 excludes the null terminator.
    std::array<char, kVstMaxEffectNameLen + 1> nameBuffer = {0};
#ifndef NDEBUG
    std::memset(nameBuffer.data(), 0x7F, nameBuffer.size());
#endif
    effect_->dispatcher(effect_, AEffectXOpcodes::effGetEffectName, 0, 0, nameBuffer.data(), 0);
    return QString(nameBuffer.data());
}

bool VST2Plugin::attachToWindow(QWindow* window)
{
    window->setTitle(getName());
    ERect* rect = nullptr;
    effect_->dispatcher(effect_, AEffectOpcodes::effEditOpen, 0, 0, reinterpret_cast<Musec::Native::WindowType>(window->winId()), 0);
    effect_->dispatcher(effect_, AEffectOpcodes::effEditGetRect, 0, 0, &rect, 0);
    if(rect)
    {
        auto width = rect->right - rect->left;
        auto height = rect->bottom - rect->top;
        window->setWidth(width);
        window->setHeight(height);
        // window->setX(rect->left);
        // window->setY(rect->top);
        Musec::Controller::AudioEngineController::AppProject().addPluginWindowMapping(effect_, window);
        Musec::Controller::AudioEngineController::AppProject().addVST2Plugin(effect_);
        window_ = window;
        return true;
    }
    else
    {
        return false;
    }
}

bool VST2Plugin::detachWithWindow()
{
    if(!window_)
    {
        return true;
    }
    Musec::Controller::AudioEngineController::AppProject().removeVST2Plugin(effect_);
    effect_->dispatcher(effect_, AEffectOpcodes::effEditClose, 0, 0, nullptr, 0);
    Musec::Controller::AudioEngineController::AppProject().removePluginWindowMapping(effect_);
    window_ = nullptr;
    return true;
}

QWindow* VST2Plugin::window()
{
    return window_;
}

bool VST2Plugin::hasUI()
{
    return effect_ && (effect_->flags & VstAEffectFlags::effFlagsHasEditor);
}

Musec::Base::PluginFormat VST2Plugin::pluginFormat()
{
    return Musec::Base::PluginFormat::FormatVST2;
}

bool VST2Plugin::activated()
{
    return activated_;
}

int VST2Plugin::parameterCount()
{
    return effect_->numParams;
}

IParameter& VST2Plugin::parameter(int index)
{
    return (reinterpret_cast<VST2PluginParameter*>(paramBlock_.data()))[index];
}

bool VST2Plugin::processing()
{
    return !bypass_;
}

// Exceeds lifetime?
const Musec::Audio::Device::ISpeakerGroupCollection& VST2Plugin::audioInputSpeakerGroupCollection() const
{
    return inputSpeakerGroupCollection_;
}

// Exceeds lifetime?
const Musec::Audio::Device::ISpeakerGroupCollection& VST2Plugin::audioOutputSpeakerGroupCollection() const
{
    return outputSpeakerGroupCollection_;
}
// ------------------------------------------------------------------------------------------
}
