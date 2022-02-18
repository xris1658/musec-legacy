#include "PluginController.hpp"

namespace Musec
{
namespace Controller
{
QList<Musec::Base::PluginReadInfo> getAllPlugins()
{
    QList<Musec::Base::PluginReadInfo> ret = Musec::DAO::selectAllPlugin(true);
    return ret;
}
}
}
