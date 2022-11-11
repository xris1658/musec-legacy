#include "VST2Plugin.hpp"

#include "audio/plugin/VST2PluginParameter.hpp"
#include "audio/plugin/VST2PluginShellPluginId.hpp"
#include "audio/plugin/VST2SpeakerGroupCollection.hpp"
#include "base/PluginBase.hpp"
#include "controller/AudioEngineController.hpp"
#include "native/Native.hpp"

#include <errhandlingapi.h>

#include <array>
#include <cstdlib>

namespace Musec::Audio::Plugin
{
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
