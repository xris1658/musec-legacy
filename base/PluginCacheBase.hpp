#ifndef MUSEC_BASE_PLUGINCACHEBASE
#define MUSEC_BASE_PLUGINCACHEBASE

#include <QString>

#include <tuple>
#include <vector>

namespace Musec
{
namespace Base
{
enum LibraryProperties
{
    IsPlugin = 1 << 0,
    Discardable = 1 << 1
};

using PluginCache = std::tuple<std::vector<char>, int>;

using PluginCacheInDatabase = std::tuple<int, std::vector<char>, int>;

enum PluginCacheField
{
    Fingerprint = 0,
    Property
};

enum PluginCacheInDatabaseField
{
    IdInDB = 0,
    FingerprintInDB,
    PropertyInDB
};

PluginCache scanLibraryCacheProperties(const QString& path);
}
}

#endif //MUSEC_BASE_PLUGINCACHEBASE
