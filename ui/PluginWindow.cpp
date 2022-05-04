#include "PluginWindow.hpp"

#include "event/EventBase.hpp"

namespace Musec::UI
{
std::weak_ptr<Musec::Audio::Plugin::IPlugin<double>> pluginNeedsWindow;
void createNewPluginWindow(std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>> pluginNeedsWindow)
{
    Musec::UI::pluginNeedsWindow = std::weak_ptr<Musec::Audio::Plugin::IPlugin<double>>(pluginNeedsWindow);
    Musec::Event::eventHandler->newPluginWindow();
}

void createNewPluginWindowComplete(QWindow* window)
{
    if(!pluginNeedsWindow.expired())
    {
        auto plugin = pluginNeedsWindow.lock();
        plugin->initializeEditor();
        plugin->attachToWindow(window);
        window->showNormal();
    }
}
}
