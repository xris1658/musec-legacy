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

Musec::Base::PluginReadInfo selectPluginFromId(int id);
}
}

#endif // MUSEC_DAO_PLUGINDAO
