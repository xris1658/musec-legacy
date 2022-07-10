#include "PluginBase.hpp"

#include "audio/host/MusecVST3Host.hpp"
#include "audio/plugin/VST2Plugin.hpp"
#include "audio/plugin/VST3Plugin.hpp"
#include "audio/plugin/CLAPPlugin.hpp"
#include "controller/ASIODriverController.hpp"
#include "native/Native.hpp"

#include <clap/plugin-features.h>

#include <pluginterfaces/vst/ivstaudioprocessor.h>

#include <QFileInfo>

#include <ShlObj_core.h>

#include <array>
#include <cstdint>
#include <tuple>
#include <cstring>

namespace Musec::Base
{
PluginFormat pluginFormat(const QString& path)
{
    static QString vst2("dll");
    static QString vst3("vst3");
    static QString clap("clap");
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
    else if(suffix == clap)
    {
        return PluginFormat::FormatCLAP;
    }
    return PluginFormat::FormatNotAPlugin;
}

QList<PluginBasicInfo> scanSingleLibraryFile(const QString& path)
{
    using namespace Musec::Base;
    using namespace Musec::Native;
    QList<PluginBasicInfo> ret;
    auto format = pluginFormat(path);
    if(format == PluginFormat::FormatVST2)
    {
        try
        {
            Musec::Audio::Plugin::VST2Plugin<float> plugin(path, true);
            auto effect = plugin.effect();
            std::array<char, kVstMaxProductStrLen> nameBuffer = {0};
            auto category = effect->dispatcher(effect, effGetPlugCategory, 0, 0, nullptr, 0);
            if(category == kPlugCategShell)
            {
                while (true)
                {
                    auto shellPluginId = effect->dispatcher(
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
    //                AEffect* subPlugin = pluginEntryProc(pluginVST2Callback);
                    auto plugin = Musec::Audio::Plugin::VST2Plugin<float>(path, false, shellPluginId);
                    auto subPlugin = plugin.effect();
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
                }
            }
            // 只有插件是否为 Shell 有必要用 effGetPlugCategory。
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
                ret.append(
                    std::make_tuple(
                        effect->uniqueID,
                        QString(nameBuffer.data()),
                        PluginFormat::FormatVST2,
                        pluginType
                    )
                );
            }
        }
        catch (WindowsLibraryRAII::ExceptionType)
        {
            //
        }
    }
    else if(format == PluginFormat::FormatVST3)
    {
        try
        {
            WindowsLibraryRAII library(path);
            auto pluginInitProc = getExport<VST3PluginInitProc>(library, "InitDll");
            auto pluginFactoryProc = getExport<VST3PluginFactoryProc>(library, "GetPluginFactory");
            auto pluginExitProc = getExport<VST3PluginExitProc>(library, "ExitDll");
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
            Steinberg::IPluginFactory2* factory2 = nullptr;
            auto factory2Result = factory->queryInterface(Steinberg::IPluginFactory2::iid, reinterpret_cast<void**>(&factory2));
            if(factory2Result == Steinberg::kResultOk)
            {
                auto classCount = factory2->countClasses();
                for(decltype(classCount) i = 0; i < classCount; ++i)
                {
                    Steinberg::PClassInfo2 classInfo2;
                    factory2->getClassInfo2(i, &classInfo2);
                    auto* category = classInfo2.category;
                    if(std::strcmp(category, kVstAudioEffectClass) == 0)
                    {
                        std::array<char, Steinberg::PClassInfo2::kSubCategoriesSize> subCateg = {0};
                        std::strncpy(subCateg.data(), classInfo2.subCategories, subCateg.size());
                        std::array<char*, Steinberg::PClassInfo2::kSubCategoriesSize> strings = {nullptr};
                        strings[0] = subCateg.data();
                        int wordCount = 0;
                        for(auto j = 0; j < subCateg.size(); ++j)
                        {
                            if(subCateg[j] == '|')
                            {
                                subCateg[j] = 0;
                                strings[++wordCount] = subCateg.data() + j + 1;
                            }
                            else if(subCateg[j] == 0)
                            {
                                break;
                            }
                        }
                        auto pluginType = PluginType::TypeUnknown;
                        for(auto j = 0; j <= wordCount; ++j)
                        {
                            if(std::strcmp(strings[j], Steinberg::Vst::PlugType::kFx) == 0)
                            {
                                pluginType = PluginType::TypeAudioFX;
                                ret.append(
                                    std::make_tuple(
                                        i,
                                        QString(classInfo2.name),
                                        PluginFormat::FormatVST3,
                                        pluginType
                                    )
                                );
                            }
                            else if(std::strcmp(strings[j], Steinberg::Vst::PlugType::kInstrument) == 0)
                            {
                                pluginType = PluginType::TypeInstrument;
                                ret.append(
                                    std::make_tuple(
                                        i,
                                        QString(classInfo2.name),
                                        PluginFormat::FormatVST3,
                                        pluginType
                                    )
                                );
                            }
                        }
                        if(pluginType == PluginType::TypeUnknown)
                        {
                            ret.append(std::make_tuple(
                                    i,
                                    QString(classInfo2.name),
                                    PluginFormat::FormatVST3,
                                    pluginType
                            ));
                        }
                    }
                }
                factory2->release();
            }
            else
            {
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
                        int32 busCounts[] = { 0, 0, 0, 0 };
                        for(int i = 0; i < 4; ++i)
                        {
                            busCounts[i] = component->getBusCount(mediaTypes[i], busDirections[i]);
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
                        component->release();
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
            }
            factory->release();
            if(pluginExitProc)
            {
                auto result = pluginExitProc();
                if (!result)
                {
                    //
                }
            }
        }
        catch (WindowsLibraryRAII::ExceptionType)
        {
//
        }
    }
    else if(format == PluginFormat::FormatCLAP)
    {
        try
        {
            Musec::Audio::Plugin::CLAPPlugin<float> plugin(path);
            const auto factory = plugin.factory();
            auto count = factory->get_plugin_count(factory);
            for(decltype(count) i = 0; i < count; ++i)
            {
                auto desc = factory->get_plugin_descriptor(factory, i);
                auto name = desc->name;
                auto features = desc->features;
                for(int j = 0; features[j]; ++j)
                {
                    auto feature = features[j];
                    if(std::strcmp(feature, CLAP_PLUGIN_FEATURE_INSTRUMENT) == 0)
                    {
                        ret.append(
                            std::make_tuple(
                                i,
                                QString(desc->name),
                                PluginFormat::FormatCLAP,
                                PluginType::TypeInstrument
                            )
                        );
                    }
                    else if(std::strcmp(feature, CLAP_PLUGIN_FEATURE_AUDIO_EFFECT) == 0
                         || std::strcmp(feature, CLAP_PLUGIN_FEATURE_NOTE_EFFECT) == 0)
                    {
                        ret.append(
                            std::make_tuple(
                                i,
                                QString(desc->name),
                                PluginFormat::FormatCLAP,
                                PluginType::TypeAudioFX
                            )
                        );
                    }
                }
            }
        }
        catch(WindowsLibraryRAII::ExceptionType) {}
    }
    return ret;
}

QStringList& defaultPluginDirectoryList()
{
    static QStringList ret;
    ret.reserve(5);
    auto programFilesPath = Musec::Native::programFilesFolder();
    if(!programFilesPath.isEmpty())
    {
        // VST2
        ret << programFilesPath.append("\\Steinberg\\VstPlugins")
            << programFilesPath.append("\\VstPlugins")
        // VST3
            << programFilesPath.append("\\Common Files\\VST3");
    }
    auto localAppDataPath = Musec::Native::localAppDataFolder();
    if(!localAppDataPath.isEmpty())
    {
        // CLAP
        ret << programFilesPath.append("\\Common Files\\CLAP")
            << localAppDataPath.append("\\Programs\\Common\\CLAP");
    }
    return ret;
}
}
