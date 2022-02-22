#ifndef MUSEC_CONTROLLER_PLUGINCONTROLLER
#define MUSEC_CONTROLLER_PLUGINCONTROLLER

#include "base/PluginBase.hpp"
#include "dao/DatabaseDAO.hpp"
#include "dao/PluginDAO.hpp"

#include <QList>

namespace Musec
{
namespace Controller
{
namespace PluginController
{
QList<Musec::Base::PluginReadInfo> getAllPlugins();
}
}
}

#endif // MUSEC_CONTROLLER_PLUGINCONTROLLER
