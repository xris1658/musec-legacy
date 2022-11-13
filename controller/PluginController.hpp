#ifndef MUSEC_CONTROLLER_PLUGINCONTROLLER
#define MUSEC_CONTROLLER_PLUGINCONTROLLER

#include "audio/plugin/IPlugin.hpp"
#include "base/PluginBase.hpp"
#include "dao/DatabaseDAO.hpp"
#include "dao/PluginDAO.hpp"

#include <QList>

#include <memory>

namespace Musec
{
namespace Controller
{
namespace PluginController
{
QList<Musec::Base::PluginReadInfo> getAllPlugins();

std::shared_ptr<Musec::Audio::Plugin::IPlugin>
loadPlugin(const QString& path, int format, const char* id);

bool startPlugin(std::shared_ptr<Musec::Audio::Plugin::IPlugin> plugin);

Musec::Base::PluginReadInfo pluginFromId(int id);
}
}
}

#endif // MUSEC_CONTROLLER_PLUGINCONTROLLER
