#include "VST2Plugin.hpp"

#include "audio/plugin/VST2PluginShellPluginId.hpp"
#include "base/Constants.hpp"
#include "controller/AudioEngineController.hpp"

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
}