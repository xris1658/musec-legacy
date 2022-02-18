#ifndef MUSEC_DAO_PLUGINDAO
#define MUSEC_DAO_PLUGINDAO

#include "base/PluginBase.hpp"

#include <QList>

namespace Musec
{
namespace DAO
{
void createPluginTable();

void insertPlugin(const Musec::Base::PluginWriteInfo& pluginWriteInfo);

void removeAllPlugins();

QList<Musec::Base::PluginReadInfo> selectAllPlugin(bool reserve);

int getAllPluginCount();
}
}

#endif // MUSEC_DAO_PLUGINDAO
