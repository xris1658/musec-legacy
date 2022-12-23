#include "PluginWindow.hpp"

#include "event/EventBase.hpp"
#include "model/PluginParameterListModel.hpp"

namespace Musec::UI
{
std::weak_ptr<Musec::Audio::Plugin::IPlugin> pluginNeedsWindow;

Musec::Entities::Plugin* pluginNeedsBasicPluginEditor;

void createNewPluginWindow(std::shared_ptr<Musec::Audio::Plugin::IPlugin> pluginNeedsWindow)
{
    Musec::UI::pluginNeedsWindow = std::weak_ptr<Musec::Audio::Plugin::IPlugin>(pluginNeedsWindow);
    Musec::Event::eventHandler->newPluginWindow();
}

void createNewPluginWindowComplete(QWindow* window)
{
    if(!pluginNeedsWindow.expired())
    {
        auto plugin = pluginNeedsWindow.lock();
        plugin->attachToWindow(window);
        window->showNormal();
    }
}

void destroyPluginWindow(QWindow* window)
{
    if(window)
    {
        window->setProperty("destroyingPlugin", QVariant::fromValue(true));
        delete window;
    }
}

void UI::createBasicPluginEditor(Entities::Plugin* plugin)
{
    pluginNeedsBasicPluginEditor = plugin;
    Musec::Event::eventHandler->newBasicPluginEditor();
}

void createBasicPluginEditorComplete(QWindow* window)
{
    if(auto plugin = pluginNeedsBasicPluginEditor)
    {
        window->setProperty("pluginName", plugin->property("name"));
        window->setTitle(plugin->getName());
        window->setProperty("parameterListModel", QVariant::fromValue(static_cast<QObject*>(new Musec::Model::PluginParameterListModel(plugin->plugin().get(), window))));
        plugin->setBasicPluginEditor(window);
        window->showNormal();
    }
}

void destroyBasicPluginEditor(QWindow* window)
{
    if(window)
    {
        delete window;
    }
}
}
