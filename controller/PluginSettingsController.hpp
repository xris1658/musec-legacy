#ifndef MUSEC_CONTROLLER_PLUGINSETTINGSCONTROLLER
#define MUSEC_CONTROLLER_PLUGINSETTINGSCONTROLLER

#include <QString>

namespace Musec
{
namespace Controller
{
namespace PluginSettingsController
{
QStringList getPluginDirectoryList();

void addPluginDirectory(const QString&);

void removePluginDirectory(const QString&);

void scanPlugins();

void setScanShortcut(bool value);
}
}
}

#endif // MUSEC_CONTROLLER_PLUGINSETTINGSCONTROLLER
