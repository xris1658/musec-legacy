#include "VST2Plugin.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "base/Constants.hpp"
#include "base/PluginBase.hpp"

#include <windows.h>
#include <errhandlingapi.h>

#include <cstring>

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
        ret = 1;
        break;
        // 获取当前采样率
    case audioMasterGetSampleRate:
    {
        auto& driver = Musec::Audio::Driver::AppASIODriver();
        if (driver)
        {
            ret = Musec::Audio::Driver::getASIODriverStreamInfo(driver).sampleRate;
        }
        break;
    }
        // 获取缓冲区大小
    case audioMasterGetBlockSize:
    {
        auto& driver = Musec::Audio::Driver::AppASIODriver();
        if (driver)
        {
            ret = Musec::Audio::Driver::getASIODriverStreamInfo(driver).preferredBufferSize;
        }
        break;
    }
        // 获取输入延迟
    case audioMasterGetInputLatency:
    {
        auto& driver = Musec::Audio::Driver::AppASIODriver();
        if (driver)
        {
            ret = Musec::Audio::Driver::getASIODriverStreamInfo(driver).inputLatencyInSamples;
        }
        break;
    }
        // 获取输出延迟
    case audioMasterGetOutputLatency:
    {
        auto& driver = Musec::Audio::Driver::AppASIODriver();
        if (driver)
        {
            ret = Musec::Audio::Driver::getASIODriverStreamInfo(driver).outputLatencyInSamples;
        }
        break;
    }
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
        // ptr: 字符串缓冲区, 填入软件厂商的名字
    case audioMasterGetVendorString:
    {
        constexpr int vendorNameLength = sizeof(Musec::Base::CompanyName) + 1;
        std::strncpy(reinterpret_cast<char*>(ptr), Musec::Base::CompanyName, vendorNameLength);
        break;
    }
        // ptr: 字符串缓冲区, 填入软件产品的名字
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
    effect_->dispatcher(effect_, effOpen, 0, 0, nullptr, 0);
    inputsRaw_ = std::vector<SampleType*>(inputCount(), nullptr);
    outputsRaw_ = std::vector<SampleType*>(outputCount(), nullptr);
}

template<typename SampleType>
VST2Plugin<SampleType>::~VST2Plugin()
{
    if(effect_)
    {
        stopProcessing();
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
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::uninitialize()
{
    stopProcessing();
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::activate()
{
    effect_->dispatcher(effect_, AEffectOpcodes::effMainsChanged,  0, 1, nullptr, 0);
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::deactivate()
{
    effect_->dispatcher(effect_, AEffectOpcodes::effMainsChanged, 0, 0, nullptr, 0);
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::startProcessing()
{
    effect_->dispatcher(effect_, AEffectXOpcodes::effStartProcess, 0, 0, nullptr, 0);
    return true;
}

template<typename SampleType>
bool VST2Plugin<SampleType>::stopProcessing()
{
    effect_->dispatcher(effect_, AEffectXOpcodes::effStopProcess, 0, 0, nullptr, 0);
    return true;
}

template<typename SampleType>
void VST2Plugin<SampleType>::process(const Musec::Audio::Base::AudioBufferViews<SampleType>& inputs,
    const Musec::Audio::Base::AudioBufferViews<SampleType>& outputs)
{
    for(int i = 0; i < inputs.size(); ++i)
    {
        inputsRaw_[i] = inputs[i].getSamples();
    }
    for(int i = 0; i < outputs.size(); ++i)
    {
        outputsRaw_[i] = outputs[i].getSamples();
    }
    // 获取采样数的方式可能不对
    std::int32_t sampleCount = outputs[0].size();
    if constexpr(std::is_same_v<SampleType, float>)
    {
        effect_->processReplacing(effect_, inputsRaw_.data(), outputsRaw_.data(), sampleCount);
    }
    else if constexpr(std::is_same_v<SampleType, double>)
    {
        effect_->processDoubleReplacing(effect_, inputsRaw_.data(), outputsRaw_.data(), sampleCount);
    }
}

template class VST2Plugin<float>;
template class VST2Plugin<double>;
// ------------------------------------------------------------------------------------------
}
