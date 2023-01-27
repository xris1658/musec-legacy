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

Musec::Base::PluginCacheInDatabase selectPluginCacheByFingerprint(const std::vector<char>& fingerprint);

void updatePluginCacheById(int id, const std::vector<char>& fingerprint, int properties);
}
}

#endif //MUSEC_DAO_PLUGINCACHEDAO
