#include "VST2Plugin.hpp"

#include "audio/engine/Project.hpp"
#include "audio/plugin/VST2PluginParameter.hpp"
#include "audio/plugin/VST2PluginShellPluginId.hpp"
#include "controller/AudioEngineController.hpp"
#include "base/Constants.hpp"
#include "base/PluginBase.hpp"
#include "native/Native.hpp"

#include <errhandlingapi.h>

#include <array>
#include <cstdlib>

// HostCanDos 命名空间只放到了源文件下，但是把用到这个命名空间内容的函数放到了别处
// 为了防止违反 ODR，将相关文件放到另一个命名空间下
namespace VST2AudioEffectX
{
#pragma warning(push)
#pragma warning(disable: 4458) // 局部声明覆盖
#pragma warning(disable: 4100) // 未引用的形参
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
        ret = *reinterpret_cast<VstProcessLevels*>(effect + 1);
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
            // 抛出异常
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
    effect_->dispatcher(effect_, effOpen, 0, 0, nullptr, 0);
    inputsRaw_ = std::vector<SampleType*>(VST2Plugin::inputCount(), nullptr);
    outputsRaw_ = std::vector<SampleType*>(VST2Plugin::outputCount(), nullptr);
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
        effect_->dispatcher(effect_, effClose, 0, 0, nullptr, 0);
    }
}

AEffect* VST2Plugin::effect() const
{
    return effect_;
}

std::uint8_t VST2Plugin::inputCount() const
{
    return static_cast<std::uint8_t>(effect_->numInputs);
}

std::uint8_t VST2Plugin::outputCount() const
{
    return static_cast<std::uint8_t>(effect_->numOutputs);
}

bool VST2Plugin::initialize(double sampleRate, std::int32_t maxSampleCount)
{
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
    paramBlock_ = Musec::Base::FixedSizeMemoryBlock();
    uninitializeEditor();
    stopProcessing();
    deactivate();
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
    for(int i = 0; i < outputCount; ++i)
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
    // Sylenth1 和 Ableton SAK 写入的字符个数为 kVstMaxEffectNameLen + 1
    // VST2 给的最大长度貌似不算空终止符，文档和代码内注释写得也并不清楚，只能靠试错得出结论
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
// ------------------------------------------------------------------------------------------
}
