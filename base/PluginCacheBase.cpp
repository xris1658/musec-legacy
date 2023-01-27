#include "PluginCacheBase.hpp"

#include "audio/plugin/CLAPPlugin.hpp"
#include "audio/plugin/VST3Plugin.hpp"
#include "base/PluginBase.hpp"
#include "util/FileHash.hpp"

namespace Musec::Base
{
PluginCache scanLibraryCacheProperties(const QString& path)
{
    auto fingerprint = Musec::Util::fileHash(path);
    auto format = Musec::Base::pluginFormat(path);
    if(format == PluginFormat::FormatVST3)
    {
        try
        {
            Musec::Audio::Plugin::VST3Plugin plugin(path);
            auto factory = plugin.factory();
            Steinberg::PFactoryInfo factoryInfo {};
            factory->getFactoryInfo(&factoryInfo);
            if(factoryInfo.flags & Steinberg::PFactoryInfo::FactoryFlags::kClassesDiscardable)
            {
                return std::make_tuple(
                    fingerprint,
                    Musec::Base::LibraryProperties::IsPlugin
                    | Musec::Base::LibraryProperties::Discardable);
            }
            else
            {
                return std::make_tuple(
                    fingerprint,
                    Musec::Base::LibraryProperties::IsPlugin);
            }
        }
        catch(...)
        {
            return std::make_tuple(fingerprint, 0);
        }
    }
    else if(format == PluginFormat::FormatCLAP)
    {
        try
        {
            Musec::Audio::Plugin::CLAPPlugin plugin(path);
            // auto invalidationFactory = plugin.invalidationFactory();
            // FIXME: I don't quite know how to use invalidationFactory,
            //  so I have to rescan the plugin every time.
            return std::make_tuple(fingerprint,
                Musec::Base::LibraryProperties::IsPlugin
                | Musec::Base::LibraryProperties::Discardable);
        }
        catch(...)
        {
            return std::make_tuple(fingerprint, 0);
        }
    }
}
}