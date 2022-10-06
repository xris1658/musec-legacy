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
const char* vst2 = "dll";
const char* vst3 = "vst3";
const char* clap = "clap";

PluginFormat pluginFormat(const QString& path)
{
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
            Musec::Audio::Plugin::VST2Plugin plugin(path, true);
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
                    // AEffect* subPlugin = pluginEntryProc(pluginVST2Callback);
                    auto plugin = Musec::Audio::Plugin::VST2Plugin(path, false, shellPluginId);
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
            // `effGetPlugCategory` is needed only if we check if the plugin is a shell plugin.
            // Some non-shell plugin just leave this at kPlugCategUnknown.
            // To get the type, simply check `flags` using `effFlagsIsSynth`.
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
        // TODO: Use VST3Plugin here
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
                }
            }
            auto factory = pluginFactoryProc();
            Steinberg::IPluginFactory2* factory2 = nullptr;
            // Musec try using IPluginFactory2 to check the type first.
            // If failed, Musec has to guess the type according to the I/O type and count.
            auto factory2Result = factory->queryInterface(Steinberg::IPluginFactory2_iid, reinterpret_cast<void**>(&factory2));
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
                                                                  IAudioProcessor_iid,
                                                                  reinterpret_cast<void**>(&audioProcessor));
                        IComponent* component = nullptr;
                        /*result = */audioProcessor->queryInterface(IComponent_iid,
                                                                    reinterpret_cast<void**>(&component));
                        auto& host = MusecVST3Host::instance();
                        component->initialize(&host);
                        ProcessSetup setup{};
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
                        // Plugins that have only outputs, or only inputs, or only audio inputs and
                        // event outputs are not taken into account.
                        if(busCounts[0] && busCounts[1] && busCounts[2])
                        {
                            // This class has event I, audio I and audio O, maybe even event O.
                            // Musec guesses that the plugin is an audio effect that has
                            // event input (e.g. FabFilter Pro-Q 2).
                            // Note that the plugin might be an instrument that has audio I as
                            // sidechain (e.g. Surge XT).
                            pluginType = PluginType::TypeAudioFX;
                        }
                        else if(busCounts[1] && busCounts[2])
                        {
                            // This class has event I and audio O, but no audio I.
                            // Musec guesses that the plugin is an instrument.
                            pluginType = PluginType::TypeInstrument;
                        }
                        else if(busCounts[0] && busCounts[1])
                        {
                            // This class has audio I/O, or maybe even event O, but no event I.
                            // Musec guesses that the plugin is an audio effect.
                            pluginType = PluginType::TypeAudioFX;
                        }
                        else if(busCounts[2] && busCounts[3])
                        {
                            // This class has only event I/O.
                            // Musec guesses that the plugin is an event processor like MIDI effect.
                            pluginType = PluginType::TypeMidiFX;
                        }
                        else if(busCounts[0] == 0 && busCounts[1] == 0
                                && busCounts[2] == 0 && busCounts[3] == 0)
                        {
                            // This class has no I/O, i.e. isolated.
                            // Musec cannot figure it out.
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
            Musec::Audio::Plugin::CLAPPlugin plugin(path);
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
        ret << QString(programFilesPath).append("\\Steinberg\\VstPlugins")
            << QString(programFilesPath).append("\\VstPlugins")
        // VST3
            << QString(programFilesPath).append("\\Common Files\\VST3")
        // CLAP
            << QString(programFilesPath).append("\\Common Files\\CLAP");
    }
    auto localAppDataPath = Musec::Native::localAppDataFolder();
    if(!localAppDataPath.isEmpty())
    {
        // CLAP
        ret << QString(localAppDataPath).append("\\Programs\\Common\\CLAP");
    }
    return ret;
}
}
