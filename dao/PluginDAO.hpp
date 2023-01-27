#ifndef MUSEC_DAO_PLUGINDAO
#define MUSEC_DAO_PLUGINDAO

#include "base/PluginBase.hpp"

#include <vector>

namespace Musec
{
namespace DAO
{
void createPluginTable();

void insertPlugin(const Musec::Base::PluginWriteInfo& pluginWriteInfo);

void removeAllPlugins();

std::vector<Musec::Base::PluginReadInfo> selectAllPlugin();

int getAllPluginCount();

Musec::Base::PluginReadInfo selectPluginById(int id);

std::vector<Musec::Base::PluginReadInfo> selectPluginByPath(const QString& path);

std::vector<QString> selectDistinctPluginPath();

void removePluginByPath(const QString& path);
}
}

#endif // MUSEC_DAO_PLUGINDAO
