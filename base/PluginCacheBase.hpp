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

using PluginCache = std::tuple<QString, std::vector<char>, int>;

using PluginCacheInDatabase = std::tuple<int, QString, std::vector<char>, int>;

enum PluginCacheField
{
    Path = 0,
    Fingerprint,
    Property
};

enum PluginCacheInDatabaseField
{
    IdInDB = 0,
    PathInDB,
    FingerprintInDB,
    PropertyInDB
};

PluginCache scanLibraryCacheProperties(const QString& path);
}
}

#endif //MUSEC_BASE_PLUGINCACHEBASE
