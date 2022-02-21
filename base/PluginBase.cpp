#include "PluginBase.hpp"

#include "base/Constants.hpp"
#include "native/WindowsLibraryRAII.hpp"
#include "audio/host/MusecVST3Host.hpp"

#include <pluginterfaces/vst/ivstaudioprocessor.h>

#include <QFileInfo>

#include <ShlObj_core.h>

#include <array>
#include <cstdint>
#include <tuple>
#include <cstring>

namespace Musec::Base
{
VstInt32 shellPluginId = 0;
bool shellPluginIdShouldBeZero = false;

PluginFormat pluginFormat(const QString& path)
{
    static QString vst2("dll");
    static QString vst3("vst3");
    QFileInfo fileInfo(path);
    auto suffix = fileInfo.suffix();
    if(suffix == vst2)
    {
        return PluginFormat::FormatVST2;
    }
    else if(suffix == vst3)
    {
        return PluginFormat::FormatVST3;
    }
    return PluginFormat::FormatNotAPlugin;
}

VstIntPtr pluginVST2Callback(AEffect* effect,
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
    case audioMasterAutomate:
        break;
    // 宿主程序使用的 VST 版本
    case audioMasterVersion:
        ret = 2400; // VST2.4
        break;
    // 加载 VST2 Shell 插件时，用于指定子插件
    case audioMasterCurrentId:
        ret =
            (shellPluginId == 0) && (!shellPluginIdShouldBeZero)?
                effect->uniqueID:
                shellPluginId;
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
        ret = 44100;
        break;
    case audioMasterGetBlockSize:
        // TODO
        ret = 512;
        break;
    case audioMasterGetInputLatency:
        // TODO
        ret = 0;
        break;
    case audioMasterGetOutputLatency:
        // TODO
        ret = 0;
        break;
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

QList<PluginBasicInfo> scanSingleLibraryFile(const QString& path)
{
    using namespace Musec::Base;
    using namespace Musec::Native;
    shellPluginId = 0;
    shellPluginIdShouldBeZero = false;
    QList<PluginBasicInfo> ret;
    auto format = pluginFormat(path);
    if(format == PluginFormat::FormatNotAPlugin)
    {
        return ret;
    }
    try
    {
        WindowsLibraryRAII library(path);
        if(format == PluginFormat::FormatVST2)
        {
            auto pluginEntryProc = library.getExport<VST2PluginEntryProc>("VSTPluginMain");
            if(!pluginEntryProc)
            {
                pluginEntryProc = library.getExport<VST2PluginEntryProc>("main");
            }
            if(!pluginEntryProc)
            {
                auto error = GetLastError();
                switch (error)
                {
                case ERROR_PROC_NOT_FOUND:
                    // 未找到入口函数，可能不是 VST 插件
                    break;
                default:
                    break;
                }
            }
            else
            {
                AEffect* effect = pluginEntryProc(pluginVST2Callback);
                if((*reinterpret_cast<std::uint32_t*>(effect)) == kEffectMagic)
                {
                    std::array<char, kVstMaxEffectNameLen> nameBuffer = {0};
                    // 加载 AEffect
                    effect->dispatcher(effect, effOpen, 0, 0, nullptr, 0);
                    auto category = effect->dispatcher(effect, effGetPlugCategory,
                        0, 0, nullptr, 0);
                    if(category == kPlugCategUnknown)
                    {
                        shellPluginIdShouldBeZero = true;
                        effect = pluginEntryProc(pluginVST2Callback);
                        category = effect->dispatcher(effect, effGetPlugCategory,
                            0, 0, nullptr, 0);
                    }
                    if(category == kPlugCategShell)
                    {
                        while (true)
                        {
                            shellPluginId = effect->dispatcher(
                                effect,
                                effShellGetNextPlugin,
                                0,
                                0,
                                nameBuffer.data(),
                                nameBuffer.size());
                            if(shellPluginId == 0 || nameBuffer[0] == 0)
                            {
                                break;
                            }
                            // 加载插件，获取插件的信息
                            AEffect* subPlugin = pluginEntryProc(pluginVST2Callback);
                            // 加载 AEffect
                            subPlugin->dispatcher(subPlugin, effOpen, 0, 0, nullptr, 0);
                            int pluginType = subPlugin->flags & effFlagsIsSynth?
                                PluginType::TypeInstrument:
                                PluginType::TypeAudioFX;
                            ret.append(std::make_tuple(
                                    shellPluginId,
                                    QString(nameBuffer.data()),
                                    PluginFormat::FormatVST2,
                                    pluginType
                                )
                            );
                            // 卸载 AEffect，缺少此步骤会导致内存泄漏
                            subPlugin->dispatcher(subPlugin, effClose, 0, 0, nullptr, 0);
                        }
                    }
                    // 只有插件是否为 Shell 需要用 effGetPlugCategory。
                    // 有些非 Shell 插件本身能够正常使用，但是 category
                    // 为 kPlugCategUnknown（e.g. Glitch2），因此
                    // 仍然需要通过 flags 确定类型。
                    else/* if(category != kPlugCategUnknown)*/
                    {
                        effect->dispatcher(
                            effect,
                            effGetEffectName,
                            0,
                            0,
                            nameBuffer.data(),
                            nameBuffer.size());
                        int pluginType = effect->flags & effFlagsIsSynth?
                            PluginType::TypeInstrument:
                            PluginType::TypeAudioFX;
                        ret.append(std::make_tuple(
                                effect->uniqueID,
                                QString(nameBuffer.data()),
                                PluginFormat::FormatVST2,
                                pluginType));
                    }
                    // 卸载 AEffect，缺少此步骤会导致内存泄漏
                    effect->dispatcher(effect, effClose, 0, 0, nullptr, 0);
                }
            }
        }
        else if(format == PluginFormat::FormatVST3)
        {
            auto pluginInitProc = library.getExport<VST3PluginInitProc>("InitDll");
            auto pluginFactoryProc = library.getExport<VST3PluginFactoryProc>("GetPluginFactory");
            auto pluginExitProc = library.getExport<VST3PluginExitProc>("ExitDll");
            if(!pluginFactoryProc)
            {
                auto error = GetLastError();
                switch (error)
                {
                case ERROR_PROC_NOT_FOUND:
                    // 未找到工厂函数，可能不是 VST 插件
                    break;
                default:
                    break;
                }
                return ret;
            }
            if(pluginInitProc)
            {
                bool initResult = pluginInitProc();
                if(!initResult)
                {
                    // 初始化出现问题
                }
            }
            auto factory = pluginFactoryProc();
            Steinberg::PFactoryInfo factoryInfo;
            factory->getFactoryInfo(&factoryInfo);
            auto classCount = factory->countClasses();
            for(decltype(classCount) i = 0; i < classCount; ++i)
            {
                Steinberg::PClassInfo classInfo;
                factory->getClassInfo(i, &classInfo);
                auto* category = classInfo.category;
                if(std::strcmp(category, kVstAudioEffectClass) == 0)
                {
                    using namespace Steinberg;
                    using namespace Steinberg::Vst;
                    using namespace Musec::Audio::Host;
                    auto pluginType = PluginType::TypeUnknown;
                    IAudioProcessor* audioProcessor = nullptr;
                    /*auto result = */factory->createInstance(classInfo.cid,
                        IAudioProcessor::iid,
                        reinterpret_cast<void**>(&audioProcessor));
                    IComponent* component = nullptr;
                    /*result = */audioProcessor->queryInterface(IComponent::iid,
                        reinterpret_cast<void**>(&component));
                    auto& host = MusecVST3Host::instance();
                    component->initialize(&host);
                    ProcessSetup setup;
                    setup.processMode = Vst::ProcessModes::kRealtime;
                    setup.sampleRate = 44100.0;
                    setup.maxSamplesPerBlock = 1024;
                    setup.symbolicSampleSize = Vst::SymbolicSampleSizes::kSample64;
                    audioProcessor->setupProcessing(setup);
                    constexpr Vst::MediaTypes mediaTypes[] =
                    {
                        Vst::MediaTypes::kAudio,
                        Vst::MediaTypes::kAudio,
                        Vst::MediaTypes::kEvent,
                        Vst::MediaTypes::kEvent
                    };
                    constexpr Vst::BusDirections busDirections[] =
                    {
                        Vst::BusDirections::kInput,
                        Vst::BusDirections::kOutput,
                        Vst::BusDirections::kInput,
                        Vst::BusDirections::kOutput
                    };
//                    constexpr wchar_t names[][5] = { L"音频输入", L"音频输出", L"事件输入", L"事件输出" };
                    int32 busCounts[] = { 0, 0, 0, 0 };
                    Vst::BusInfo busInfo;
                    for(int i = 0; i < 4; ++i)
                    {
                        busCounts[i] = component->getBusCount(mediaTypes[i], busDirections[i]);
                        auto busCount = busCounts[i];
                        for(decltype(busCount) j = 0; j < busCount; ++j)
                        {
                            component->getBusInfo(mediaTypes[i], busDirections[i], j, busInfo);
                            if(busInfo.busType == Vst::BusTypes::kMain)
                            {
                            }
                            else
                            {
                            }
                        }
                    }
                    // 没有考虑只有输出，只有输入的插件和只有音频输入和事件输出的插件。可能需要修改。
                    if(busCounts[0] && busCounts[1] && busCounts[2])
                    {
                        // 此类有事件输入，音频输入和音频输出。
                        // 可能也有事件输出。
                        // 程序认为这是带事件输入的音频效果器。
                        pluginType = PluginType::TypeAudioFX;
                    }
                    else if(busCounts[1] && busCounts[2])
                    {
                        // 此类有事件输入和音频输出，但没有音频输入。
                        // 可能也有事件输出。
                        // 程序认为这是乐器。
                        pluginType = PluginType::TypeInstrument;
                    }
                    else if(busCounts[0] && busCounts[1])
                    {
                        // 此类有音频输入和音频输出，但没有事件输入。
                        // 可能也有事件输出。
                        // 程序认为这是普通的音频效果器。
                        pluginType = PluginType::TypeAudioFX;
                    }
                    else if(busCounts[2] && busCounts[3])
                    {
                        // 此类有事件输入和事件输出，但没有音频输入和输出。
                        // 程序认为这是事件处理器，如 MIDI 效果器。
                        pluginType = PluginType::TypeMidiFX;
                    }
                    else if(busCounts[0] == 0 && busCounts[1] == 0
                         && busCounts[2] == 0 && busCounts[3] == 0)
                    {
                        // 此类没有输入和输出。
                        // 程序认为是加载插件的过程出现了问题。
                        pluginType = PluginType::TypeUnknown;
                    }
                    component->terminate();
                    audioProcessor->release();
                    ret.append(std::make_tuple(
                           i,
                           QString(classInfo.name),
                           PluginFormat::FormatVST3,
                           pluginType
                        )
                    );
                }
            }
            if(pluginExitProc)
            {
                pluginExitProc();
            }
        }
    }
    catch (WindowsLibraryRAII::ExceptionType error)
    {
        switch (error)
        {
        case ERROR_BAD_EXE_FORMAT:
            // 损坏 / 32 位
            break;
        case ERROR_DLL_INIT_FAILED:
            // 损坏
            break;
        default:
            break;
        }
        return ret;
    }
    return ret;
}

QStringList& defaultPluginDirectoryList()
{
    static QStringList ret;
    ret.reserve(3);
    static wchar_t path[MAX_PATH] = {0};
    auto getFolderResult = SHGetFolderPathW(
        nullptr,
        CSIDL_PROGRAM_FILES,
        NULL,
        SHGFP_TYPE_CURRENT,
        path);
    if(getFolderResult != S_OK)
    {
        // TODO: 异常处理
    }
    ret << QString::fromWCharArray(path).append("\\Steinberg\\VstPlugins")
        << QString::fromWCharArray(path).append("\\VstPlugins")
        << QString::fromWCharArray(path).append("\\Common Files\\VST3");
    return ret;
}
}
