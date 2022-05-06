#include "PluginWindow.hpp"

#include "event/EventBase.hpp"

namespace Musec::UI
{
std::weak_ptr<Musec::Audio::Plugin::IPlugin<float>> pluginNeedsWindow;
void createNewPluginWindow(std::shared_ptr<Musec::Audio::Plugin::IPlugin<float>> pluginNeedsWindow)
{
    Musec::UI::pluginNeedsWindow = std::weak_ptr<Musec::Audio::Plugin::IPlugin<float>>(pluginNeedsWindow);
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
        window->close();
    }
}
}
