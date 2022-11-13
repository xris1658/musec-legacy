#include "PluginBase.hpp"

#include "audio/plugin/VST2Plugin.hpp"
#include "audio/plugin/VST3Plugin.hpp"
#include "audio/plugin/CLAPPlugin.hpp"
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
            std::vector<char> shellPluginIdRaw(sizeof(VstInt32) + 1, 0);
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
                    auto shellPluginIdAsInt32 = static_cast<VstInt32>(shellPluginId);
                    std::memcpy(shellPluginIdRaw.data(), &shellPluginIdAsInt32, sizeof(VstInt32));
                    ret.append(std::make_tuple(
                            shellPluginIdRaw,
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
                std::memcpy(shellPluginIdRaw.data(), &(effect->uniqueID), sizeof(VstInt32));
                ret.append(
                    std::make_tuple(
                        shellPluginIdRaw,
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
            std::vector<char> uid(16, 0);
            Musec::Audio::Plugin::VST3Plugin plugin(path);
            auto factory = plugin.factory();
            Steinberg::IPluginFactory2* factory2 = nullptr;
            if(
                factory->queryInterface(
                    Steinberg::IPluginFactory2::iid, reinterpret_cast<void**>(&factory2)
                ) == Steinberg::kResultOk
            )
            {
                auto classCount = factory2->countClasses();
                Steinberg::PClassInfo2 classInfo2;
                for(decltype(classCount) i = 0; i < classCount; ++i)
                {
                    factory2->getClassInfo2(i, &classInfo2);
                    auto* category = classInfo2.category;
                    if(std::strcmp(category, kVstAudioEffectClass) == 0)
                    {
                        std::memcpy(uid.data(), classInfo2.cid, 16);
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
                                        uid,
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
                                        uid,
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
                                    uid,
                                    QString(classInfo2.name),
                                    PluginFormat::FormatVST3,
                                    pluginType
                            ));
                        }
                    }
                }
                factory2->release();
            }
            // I've never actually met any plugin that doesn't implement `IPluginFactory2`
            else
            {
                auto classCount = factory->countClasses();
                Steinberg::PClassInfo classInfo;
                for(decltype(classCount) i = 0; i < classCount; ++i)
                {
                    factory->getClassInfo(i, &classInfo);
                    if(std::strcmp(classInfo.category, kVstAudioEffectClass) == 0)
                    {
                        std::memcpy(uid.data(), classInfo.cid, 16);
                        ret.append(
                            std::make_tuple(
                                uid,
                                QString(classInfo.name),
                                PluginFormat::FormatVST3,
                                PluginType::TypeUnknown
                            )
                        );
                    }
                }
            }
        }
        catch(WindowsLibraryRAII::ExceptionType)
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
                auto length = std::strlen(desc->id);
                std::vector<char> uid(length + 1, 0);
                std::memcpy(uid.data(), desc->id, length);
                for(int j = 0; features[j]; ++j)
                {
                    auto feature = features[j];
                    if(std::strcmp(feature, CLAP_PLUGIN_FEATURE_INSTRUMENT) == 0)
                    {
                        ret.append(
                            std::make_tuple(
                                uid,
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
                                uid,
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
