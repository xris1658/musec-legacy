#include "VST2Plugin.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "audio/engine/Project.hpp"
#include "controller/AudioEngineController.hpp"
#include "base/Constants.hpp"
#include "base/PluginBase.hpp"
#include "native/Native.hpp"

#include <windows.h>
#include <errhandlingapi.h>

#include <array>
#include <cstdlib>

// HostCanDos 命名空间只放到了源文件下，但是把用到这个命名空间内容的函数放到了别处
// 为了防止违反 ODR，将相关文件放到另一个命名空间下
namespace VST2AudioEffectX
{
#include <public.sdk/source/vst2.x/audioeffect.h>
#include <public.sdk/source/vst2.x/audioeffectx.h>
#include <public.sdk/source/vst2.x/audioeffect.cpp>
#include <public.sdk/source/vst2.x/audioeffectx.cpp>
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

VstIntPtr pluginVST2Callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
    VstIntPtr ret = 0;
    switch (opcode)
    {
        // AudioMasterOpcodes (aeffect.h)

    // 插件的参数通过 MIDI 和 GUI 发生了更改
    // index: 参数编号; opt: 参数值
    case audioMasterAutomate:
        break;
    // 宿主程序使用的 VST 版本
    case audioMasterVersion:
        ret = kVstVersion; // VST2.4
        break;
    // 加载 VST2 Shell 插件时，用于指定子插件
    case audioMasterCurrentId:
        ret = (ShellPluginId::instance().getId() == 0) && (!(ShellPluginId::instance().idShouldBeZero()))?
            effect->uniqueID:
            ShellPluginId::instance().getId();
        break;
    case audioMasterIdle:
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
    // 返回值: VstTimeInfo*
    case audioMasterGetTime:
        // TODO
        ret = NULL;
        break;
    // ptr: 指向 VstEvents 的指针
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
    // 返回值: 若支持则返回 1
    case audioMasterIOChanged:
        ret = 1;
        break;
#if kVstVersion < 2400
        case audioMasterNeedIdle:
            break;
#endif
    // index: 宽; value: 高; 返回值: 若支持则返回 1
    case audioMasterSizeWindow:
        Musec::Controller::AudioEngineController::AppProject().setPluginWindowSize(effect, index, value);
        ret = 1;
        break;
    // 获取当前采样率
    case audioMasterGetSampleRate:
        ret = Musec::Controller::AudioEngineController::getCurrentSampleRate();
        break;
    // 获取缓冲区大小
    case audioMasterGetBlockSize:
        ret = Musec::Controller::AudioEngineController::getCurrentBlockSize();
        break;
    // 获取输入延迟
    case audioMasterGetInputLatency:
        ret = Musec::Controller::AudioEngineController::getInputLatency();
        break;
    // 获取输出延迟
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
    // 返回值: VstProcessLevels
    case audioMasterGetCurrentProcessLevel:
        // TODO
        ret = VstProcessLevels::kVstProcessLevelUnknown;
        break;
    // 返回值: VstAutomationStates
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
    // ptr: 字符串缓冲区, 填入软件厂商的名称
    case audioMasterGetVendorString:
    {
        constexpr int vendorNameLength = sizeof(Musec::Base::CompanyName) + 1;
        std::strncpy(reinterpret_cast<char*>(ptr), Musec::Base::CompanyName, vendorNameLength);
        break;
    }
    // ptr: 字符串缓冲区, 填入软件产品的名称
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
    // 此处与 HostCanDos 中的字符串进行比较
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
    // 返回插件所在目录
    case audioMasterGetDirectory:
        break;
    case audioMasterUpdateDisplay:
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

template<typename SampleType>
VST2Plugin<SampleType>::VST2Plugin(const QString& path, bool scanPlugin, VstInt32 shellPluginId):
    VST2Plugin::Base(path)
{
    auto pluginEntryProc = Musec::Native::getExport<Musec::Base::VST2PluginEntryProc>(*this, "VSTPluginMain");
    if(!pluginEntryProc)
    {
        pluginEntryProc = Musec::Native::getExport<Musec::Base::VST2PluginEntryProc>(*this, "main");
        if(!pluginEntryProc)
        {
            // 抛出异常
            throw GetLastError();
        }
    }
    if(scanPlugin)
    {
        effect_ = ShellPluginId::instance().getShellPlugin(0, false, pluginEntryProc);
        effect_->dispatcher(effect_, effOpen, 0, 0, nullptr, 0);
        auto category = effect_->dispatcher(effect_, effGetPlugCategory, 0, 0, nullptr, 0);
        if(category == kPlugCategUnknown)
        {
            effect_->dispatcher(effect_, effClose, 0, 0, nullptr, 0);
            effect_ = ShellPluginId::instance().getShellPlugin(0, true, pluginEntryProc);
        }
    }
    else if(shellPluginId == 0)
    {
        effect_ = pluginEntryProc(pluginVST2Callback);
    }
    else
    {
        effect_ = ShellPluginId::instance().getShellPlugin(shellPluginId, true, pluginEntryProc);
    }
    if constexpr(std::is_same_v<SampleType, double>)
    {
        if(!(effect_->flags & VstAEffectFlags::effFlagsCanDoubleReplacing))
        {
            throw std::runtime_error("This plugin cannot process 64-bit float!");
        }
    }
    effect_->dispatcher(effect_, effOpen, 0, 0, nullptr, 0);
    inputsRaw_ = std::vector<SampleType*>(inputCount(), nullptr);
    outputsRaw_ = std::vector<SampleType*>(outputCount(), nullptr);
}

template<typename SampleType>
VST2Plugin<SampleType>::~VST2Plugin()
{
    if(effect_)
    {
        if(hasUI())
        {
            uninitializeEditor();
        }
        stopProcessing();
        deactivate();
        uninitialize();
        effect_->dispatcher(effect_, effClose, 0, 0, nullptr, 0);
    }
}

template<typename SampleType>
AEffect* VST2Plugin<SampleType>::effect() const
{
    return effect_;
}

template<typename SampleType>
std::uint8_t VST2Plugin<SampleType>::inputCount() const
{
    return static_cast<std::uint8_t>(effect_->numInputs);
}

template<typename SampleType>
std::uint8_t VST2Plugin<SampleType>::outputCount() const
{
    return static_cast<std::uint8_t>(effect_->numOutputs);
}

template<typename SampleType>
bool VST2Plugin<SampleType>::initialize(double sampleRate, std::int32_t maxSampleCount)
{
    effect_->dispatcher(effect_, AEffectOpcodes::effSetSampleRate, 0, 0, nullptr, sampleRate);
    effect_->dispatcher(effect_, AEffectOpcodes::effSetBlockSize, 0, maxSampleCount, nullptr, 0);
    initializeEditor();
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::uninitialize()
{
    uninitializeEditor();
    stopProcessing();
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::activate()
{
    effect_->dispatcher(effect_, AEffectOpcodes::effMainsChanged,  0, 1, nullptr, 0);
    activated_ = true;
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::deactivate()
{
    effect_->dispatcher(effect_, AEffectOpcodes::effMainsChanged, 0, 0, nullptr, 0);
    activated_ = false;
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::startProcessing()
{
    effect_->dispatcher(effect_, AEffectXOpcodes::effStartProcess, 0, 0, nullptr, 0);
    bypass_ = false;
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::stopProcessing()
{
    effect_->dispatcher(effect_, AEffectXOpcodes::effStopProcess, 0, 0, nullptr, 0);
    bypass_ = true;
    return true;
}

template<typename SampleType>
void VST2Plugin<SampleType>::process(const Musec::Audio::Base::AudioBufferViews<SampleType>& inputs,
    const Musec::Audio::Base::AudioBufferViews<SampleType>& outputs)
{
    for(int i = 0; i < inputs.size(); ++i)
    {
        inputsRaw_[i] = inputs[i].getSamples();
        if(inputsRaw_.size() > inputs.size())
        {
            inputsRaw_[i + inputs.size()] = inputs[i].getSamples();
        }
    }
    auto outputSize = outputs.size();
    for(int i = 0; i < outputSize; ++i)
    {
        outputsRaw_[i] = outputs[i].getSamples();
    }
    for(int i = outputSize; i < effect_->numOutputs; ++i)
    {
        outputsRaw_[i] = Musec::Controller::AudioEngineController::dummyBufferView<SampleType>().getSamples();
    }
    // 获取采样数的方式可能不对
    std::int32_t sampleCount = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    if constexpr(std::is_same_v<SampleType, float>)
    {
        effect_->processReplacing(effect_, inputsRaw_.data(), outputsRaw_.data(), sampleCount);
    }
    else if constexpr(std::is_same_v<SampleType, double>)
    {
        effect_->processDoubleReplacing(effect_, inputsRaw_.data(), outputsRaw_.data(), sampleCount);
    }
}

template<typename SampleType> bool VST2Plugin<SampleType>::initializeEditor()
{
    return effect_->flags & VstAEffectFlags::effFlagsHasEditor;
}

template<typename SampleType> bool VST2Plugin<SampleType>::uninitializeEditor()
{
    return detachWithWindow();
}

template<typename SampleType> bool VST2Plugin<SampleType>::getBypass() const
{
    return bypass_;
}

template<typename SampleType> QString VST2Plugin<SampleType>::getName() const
{
    if(!effect_)
    {
        return QString();
    }
    // Sylenth1 和 Ableton SAK 写入的字符个数为 kVstMaxEffectNameLen + 1
    // VST2 给的最大长度貌似不算空终止符，文档和代码内注释写得也并不清楚，只能靠试错得出结论
    std::array<char, kVstMaxEffectNameLen + 1> nameBuffer = {0};
#ifndef NDEBUG
    std::memset(nameBuffer.data(), 0x7F, nameBuffer.size());
#endif
    effect_->dispatcher(effect_, AEffectXOpcodes::effGetEffectName, 0, 0, nameBuffer.data(), 0);
    return QString(nameBuffer.data());
}

template<typename SampleType> bool VST2Plugin<SampleType>::attachToWindow(QWindow* window)
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
        window_ = window;
        return true;
    }
    else
    {
        return false;
    }
}

template<typename SampleType> bool VST2Plugin<SampleType>::detachWithWindow()
{
    if(!window_)
    {
        return true;
    }
    effect_->dispatcher(effect_, AEffectOpcodes::effEditClose, 0, 0, nullptr, 0);
    Musec::Controller::AudioEngineController::AppProject().removePluginWindowMapping(effect_);
    window_ = nullptr;
    return true;
}

template<typename SampleType>
QWindow* VST2Plugin<SampleType>::window()
{
    return window_;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::hasUI()
{
    return effect_ && (effect_->flags & VstAEffectFlags::effFlagsHasEditor);
}

template<typename SampleType>
Musec::Base::PluginFormat VST2Plugin<SampleType>::pluginFormat()
{
    return Musec::Base::PluginFormat::FormatVST2;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::activated()
{
    return activated_;
}

    template class VST2Plugin<float>;
template class VST2Plugin<double>;
// ------------------------------------------------------------------------------------------
}
