#ifndef MUSEC_DAO_PLUGINCACHEDAO
#define MUSEC_DAO_PLUGINCACHEDAO

#include "base/PluginCacheBase.hpp"

namespace Musec
{
namespace DAO
{
void createPluginCacheTable();

int addPluginCache(const Musec::Base::PluginCache& pluginCache);

void removePluginCache(int id);

void clearPluginCache();

std::vector<Musec::Base::PluginCacheInDatabase> selectAllPluginCache();

Musec::Base::PluginCacheInDatabase selectPluginCacheByPath(const QString& path);

std::vector<Musec::Base::PluginCacheInDatabase> selectPluginCacheByFingerprint(const std::vector<char>& fingerprint);
}
}

#endif //MUSEC_DAO_PLUGINCACHEDAO
