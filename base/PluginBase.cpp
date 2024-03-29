#include "PluginBase.hpp"

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
const char* vst3 = "vst3";
const char* clap = "clap";

PluginFormat pluginFormat(const QString& path)
{
    QFileInfo fileInfo(path);
    auto suffix = fileInfo.suffix();
    if(suffix == vst3)
    {
        return PluginFormat::FormatVST3;
    }
    else if(suffix == clap)
    {
        return PluginFormat::FormatCLAP;
    }
    return PluginFormat::FormatNotAPlugin;
}

std::vector<PluginBasicInfo> scanSingleLibraryFile(const QString& path)
{
    using namespace Musec::Base;
    using namespace Musec::Native;
    std::vector<PluginBasicInfo> ret;
    auto format = pluginFormat(path);
    // Some plugins might leak
    // TODO: Scan plugins with a separate program to prevent memory leak in the main application
    if(format == PluginFormat::FormatVST3)
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
                ret.reserve(classCount);
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
                                ret.emplace_back(
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
                                ret.emplace_back(
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
                            plugin.createPlugin(classInfo2.cid);
                            plugin.initialize(44100, 1024);
                            auto* component = plugin.component();
                            struct BusCount { int ai; int ao; int ei; int eo; };
                            auto [audioInput, audioOutput, eventInput, eventOutput] = BusCount {};
                            audioInput = component->getBusCount(Steinberg::Vst::MediaTypes::kAudio,
                                Steinberg::Vst::BusDirections::kInput);
                            audioOutput = component->getBusCount(Steinberg::Vst::MediaTypes::kAudio,
                                Steinberg::Vst::BusDirections::kOutput);
                            eventInput = component->getBusCount(Steinberg::Vst::MediaTypes::kEvent,
                                Steinberg::Vst::BusDirections::kInput);
                            eventOutput = component->getBusCount(Steinberg::Vst::MediaTypes::kEvent,
                                Steinberg::Vst::BusDirections::kOutput);
                            if(eventInput && audioOutput)
                            {
                                if(audioInput)
                                {
                                    // audio effect that has event inputs (e.g. FabFilter Pro-Q 3) or
                                    // instrument that has audio inputs (e.g. Surge XT)
                                    // TODO: Clarify plugin type by finding out which bus is the main input
                                    Steinberg::Vst::BusInfo busInfo;
                                    bool hasMainAudioInput = false;
                                    bool hasMainEventInput = false;
                                    for(int i = 0; i < audioInput; ++i)
                                    {
                                        component->getBusInfo(Steinberg::Vst::MediaTypes::kAudio,
                                            Steinberg::Vst::BusDirections::kInput, i, busInfo);
                                        if(busInfo.busType == Steinberg::Vst::BusTypes::kMain)
                                        {
                                            hasMainAudioInput = true;
                                        }
                                    }
                                    for(int i = 0; i < eventInput; ++i)
                                    {
                                        component->getBusInfo(Steinberg::Vst::MediaTypes::kEvent,
                                            Steinberg::Vst::BusDirections::kInput, i, busInfo);
                                        if(busInfo.busType == Steinberg::Vst::BusTypes::kMain)
                                        {
                                            hasMainEventInput = true;
                                        }
                                    }
                                    if(hasMainAudioInput && hasMainEventInput)
                                    {
                                        // TODO: Let user decide which type the plugin is
                                    }
                                    if(hasMainAudioInput && (!hasMainEventInput))
                                    {
                                        pluginType = PluginType::TypeInstrument;
                                    }
                                    if(hasMainEventInput && (!hasMainAudioInput))
                                    {
                                        pluginType = PluginType::TypeAudioFX;
                                    }
                                }
                                else
                                {
                                    pluginType = PluginType::TypeInstrument;
                                }
                            }
                            if(audioInput && audioOutput)
                            {
                                pluginType = PluginType::TypeAudioFX;
                            }
                            if(eventInput && eventOutput)
                            {
                                pluginType = PluginType::TypeMidiFX;
                            }
                            ret.emplace_back(
                                std::make_tuple(
                                    uid,
                                    QString(classInfo2.name),
                                    PluginFormat::FormatVST3,
                                    pluginType
                                )
                            );
                            plugin.uninitialize();
                        }
                    }
                }
                factory2->release();
            }
            // I've never actually met any plugin that doesn't implement `IPluginFactory2`
            else
            {
                auto classCount = factory->countClasses();
                ret.reserve(classCount);
                Steinberg::PClassInfo classInfo;
                for(decltype(classCount) i = 0; i < classCount; ++i)
                {
                    factory->getClassInfo(i, &classInfo);
                    if(std::strcmp(classInfo.category, kVstAudioEffectClass) == 0)
                    {
                        std::memcpy(uid.data(), classInfo.cid, 16);
                        ret.emplace_back(
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
        catch (...) {}
    }
    else if(format == PluginFormat::FormatCLAP)
    {
        try
        {
            Musec::Audio::Plugin::CLAPPlugin plugin(path);
            const auto factory = plugin.factory();
            const auto invalidationFactory = plugin.invalidationFactory();
            if(invalidationFactory)
            {
                auto invalidationCount = invalidationFactory->count(invalidationFactory);
                for(decltype(invalidationCount) i = 0; i < invalidationCount; ++i)
                {
                    auto invalidationSource = invalidationFactory->get(invalidationFactory, i);
                    // TODO: Do something with invalidationSource
                }
            }
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
                        ret.emplace_back(
                            std::make_tuple(
                                uid,
                                QString(name),
                                PluginFormat::FormatCLAP,
                                PluginType::TypeInstrument
                            )
                        );
                    }
                    if(std::strcmp(feature, CLAP_PLUGIN_FEATURE_AUDIO_EFFECT) == 0
                    || std::strcmp(feature, CLAP_PLUGIN_FEATURE_ANALYZER) == 0)
                    {
                        ret.emplace_back(
                            std::make_tuple(
                                uid,
                                QString(name),
                                PluginFormat::FormatCLAP,
                                PluginType::TypeAudioFX
                            )
                        );
                    }
                    if(std::strcmp(feature, CLAP_PLUGIN_FEATURE_NOTE_EFFECT) == 0)
                    {
                        ret.emplace_back(
                            std::make_tuple(
                                uid,
                                QString(name),
                                PluginFormat::FormatCLAP,
                                PluginType::TypeMidiFX
                            )
                        );
                    }
                }
            }
        }
        catch (...) {}
    }
    return ret;
}
}
