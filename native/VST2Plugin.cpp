#include "VST2Plugin.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "base/Constants.hpp"
#include "base/PluginBase.hpp"

#include <windows.h>
#include <errhandlingapi.h>

namespace Musec::Native
{
VstIntPtr pluginVST2Callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
    VstIntPtr ret = 0;
    switch (opcode)
    {
        // AudioMasterOpcodes (aeffect.h)

        // 插件的参数通过 MIDI 和 GUI 发生了更改
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
        case audioMasterGetTime:
            // TODO
            ret = NULL;
            break;
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
        case audioMasterIOChanged:
            ret = 1;
            break;
#if kVstVersion < 2400
            case audioMasterNeedIdle:
        break;
#endif
        case audioMasterSizeWindow:
            ret = 1;
            break;
        case audioMasterGetSampleRate:
            // TODO
        {
            auto& driver = Musec::Audio::Driver::AppASIODriver();
            if (driver)
            {
                ret = Musec::Audio::Driver::getASIODriverStreamInfo(driver).sampleRate;
            }
            break;
        }
        case audioMasterGetBlockSize:
        {
            auto& driver = Musec::Audio::Driver::AppASIODriver();
            if (driver)
            {
                ret = Musec::Audio::Driver::getASIODriverStreamInfo(driver).preferredBufferSize;
            }
            break;
        }
        case audioMasterGetInputLatency:
        {
            auto& driver = Musec::Audio::Driver::AppASIODriver();
            if (driver)
            {
                ret = Musec::Audio::Driver::getASIODriverStreamInfo(driver).inputLatencyInSamples;
            }
            break;
        }
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
        case audioMasterGetCurrentProcessLevel:
            // TODO
            ret = VstProcessLevels::kVstProcessLevelUnknown;
            break;
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
        case audioMasterGetVendorString:
        {
            constexpr int vendorNameLength = sizeof(Musec::Base::CompanyName) + 1;
            std::strncpy(reinterpret_cast<char*>(ptr), Musec::Base::CompanyName, vendorNameLength);
            break;
        }
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
        case audioMasterCanDo:
            // TODO
            ret = 1;
            break;
        case audioMasterGetLanguage:
            ret = VstHostLanguage::kVstLangEnglish;
            break;
#if kVstVersion < 2400
            case audioMasterOpenWindow:
        break;
    case audioMasterCloseWindow:
        break;
#endif
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

// ShellPluginId
// ------------------------------------------------------------------------------------------

ShellPluginId::ShellPluginId()
{
    //
}

ShellPluginId::~ShellPluginId() noexcept
{
    //
}

ShellPluginId& ShellPluginId::instance()
{
    static ShellPluginId ret;
    return ret;
}

VstInt32 ShellPluginId::getId() const
{
    return id_;
}

bool ShellPluginId::idShouldBeZero() const
{
    return idShouldBeZero_;
}

AEffect* ShellPluginId::getShellPlugin(VstInt32 id, bool idShouldBeZero,
                                       Musec::Base::VST2PluginEntryProc pluginEntryProc)
{
    Steinberg::Base::Thread::FGuard lg(mutex_);
    id_ = id;
    idShouldBeZero_ = idShouldBeZero;
    return pluginEntryProc(pluginVST2Callback);
}
// ------------------------------------------------------------------------------------------

// VST2Plugin
// ------------------------------------------------------------------------------------------

VST2Plugin::VST2Plugin(): VST2Plugin::Base()
{
    //
}

VST2Plugin::VST2Plugin(const QString& path, bool scanPlugin, VstInt32 shellPluginId):
    VST2Plugin::Base(path)
{
    auto pluginEntryProc = getExport<Musec::Base::VST2PluginEntryProc>(*this, "VSTPluginMain");
    if(!pluginEntryProc)
    {
        pluginEntryProc = getExport<Musec::Base::VST2PluginEntryProc>(*this, "main");
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
}

VST2Plugin::~VST2Plugin() noexcept
{
    if(effect_)
    {
        effect_->dispatcher(effect_, effClose, 0, 0, nullptr, 0);
    }
}

AEffect* VST2Plugin::effect() const
{
    return effect_;
}
// ------------------------------------------------------------------------------------------
}
