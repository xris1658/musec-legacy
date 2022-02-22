#include "PluginController.hpp"

namespace Musec::Controller::PluginController
{
QList<Base::PluginReadInfo> getAllPlugins()
{
    QList<Base::PluginReadInfo> ret = DAO::selectAllPlugin(true);
    return ret;
}
}
