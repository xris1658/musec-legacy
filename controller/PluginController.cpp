#include "PluginController.hpp"

#include "audio/plugin/VST3Plugin.hpp"
#include "audio/plugin/CLAPPlugin.hpp"
#include "controller/AudioEngineController.hpp"
#include "dao/PluginDAO.hpp"
#include "ui/PluginWindow.hpp"

namespace Musec::Controller::PluginController
{
std::vector<Base::PluginReadInfo> getAllPlugins()
{
    return Musec::DAO::selectAllPlugin();
}

std::shared_ptr<Musec::Audio::Plugin::IPlugin>
loadPlugin(const QString& path, int format, const char* id)
{
    try
    {
        if(format == Musec::Base::PluginFormat::FormatVST3)
        {
            return std::make_shared<Musec::Audio::Plugin::VST3Plugin>(
                path, reinterpret_cast<const Steinberg::TUID&>(*id));
        }
        else if(format == Musec::Base::PluginFormat::FormatCLAP)
        {
            return std::make_shared<Musec::Audio::Plugin::CLAPPlugin>(
                path, id);
        }
        return nullptr;
    }
    catch(...)
    {
        return nullptr;
    }
}

bool startPlugin(std::shared_ptr<Musec::Audio::Plugin::IPlugin> plugin)
{
    if(plugin)
    {
        if(!plugin->initialize(Musec::Controller::AudioEngineController::getCurrentSampleRate(),
            Musec::Controller::AudioEngineController::getMaxBlockSize()))
        {
            return false;
        }
        if(plugin->hasUI())
        {
            Musec::UI::createNewPluginWindow(plugin);
        }
        if(!plugin->activate())
        {
            return false;
        }
        if(!plugin->startProcessing())
        {
            return false;
        }
        return true;
    }
    return false;
}

Musec::Base::PluginReadInfo pluginFromId(int id)
{
    return Musec::DAO::selectPluginById(id);
}
}
