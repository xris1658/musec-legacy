#ifndef MUSEC_UI_PLUGINWINDOW
#define MUSEC_UI_PLUGINWINDOW

#include "audio/plugin/IPlugin.hpp"

#include <memory>

namespace Musec
{
namespace UI
{
void createNewPluginWindow(std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>> pluginNeedsWindow);
void createNewPluginWindowComplete(QWindow* window);
}
}

#endif //MUSEC_UI_PLUGINWINDOW
