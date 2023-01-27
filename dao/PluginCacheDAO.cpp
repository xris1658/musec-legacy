#include "PluginCacheDAO.hpp"

#include "dao/DatabaseDAO.hpp"

namespace Musec
{
namespace DAO
{
namespace Impl
{
const char16_t* createPluginCacheTableCommand()
{
    static char16_t ret[] =
        u"CREATE TABLE IF NOT EXISTS plugin_cache("
        "   `id`          INTEGER NOT NULL"
        "      CONSTRAINT plugin_cache_pk"
        "          PRIMARY KEY AUTOINCREMENT,"
        "   `fingerprint` BLOB NOT NULL,"
        "   `property`    INTEGER NOT NULL"
        ");";
    return ret;
}

const char16_t* addPluginCacheCommand()
{
    static char16_t ret[] =
        u"INSERT INTO plugin_cache(fingerprint, property)"
        "VALUES(?, ?)";
    return ret;
}

const char16_t* getPluginCacheLastRowIdCommand()
{
    static char16_t ret[] =
        u"SELECT seq FROM sqlite_sequence WHERE name = `plugin_cache`";
    return ret;
}

const char16_t* removePluginCacheCommand()
{
    static char16_t ret[] =
        u"DELETE FROM plugin_cache WHERE id = ?";
    return ret;
}

const char16_t* clearPluginCacheCommand()
{
    static char16_t ret[] =
        u"DELETE FROM plugin";
    return ret;
}

const char16_t* resetPluginCacheIdFromSequenceCommand()
{
    static char16_t ret[] =
        u"DELETE FROM sqlite_sequence WHERE name = 'plugin_cache'";
    return ret;
}

const char16_t* getAllPluginCacheCountCommand()
{
    static char16_t ret[] =
        u"SELECT COUNT(*) FROM plugin_cache";
    return ret;
}

const char16_t* selectAllPluginCacheCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM plugin_cache";
    return ret;
}

const char16_t* selectPluginCacheByFingerprintCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM plugin_cache WHERE fingerprint = ?";
    return ret;
}

const char16_t* updatePluginCacheByIdCommand()
{
    static char16_t ret[] =
        u"UPDATE plugin_cache SET fingerprint = ?, property = ? WHERE id = ?";
    return ret;
}
}

void createPluginCacheTable()
{
    AppDatabase() << Impl::createPluginCacheTableCommand();
}

int addPluginCache(const Base::PluginCache& pluginCache)
{
    using namespace Musec::Base;
    AppDatabase() << Impl::addPluginCacheCommand()
                  << std::get<PluginCacheField::Fingerprint>(pluginCache)
                  << std::get<PluginCacheField::Property>(pluginCache);
    return AppDatabase().last_insert_rowid();
}

void removePluginCache(int id)
{
    using namespace Musec::Base;
    AppDatabase() << Impl::removePluginCacheCommand()
                  << id;
}

void clearPluginCache()
{
    AppDatabase() << Impl::clearPluginCacheCommand();
    AppDatabase() << Impl::resetPluginCacheIdFromSequenceCommand();
}

std::vector<Musec::Base::PluginCacheInDatabase> selectAllPluginCache()
{
    std::vector<Musec::Base::PluginCacheInDatabase> ret;
    int size = 0; AppDatabase() << Impl::getAllPluginCacheCountCommand() >> size;
    ret.reserve(size);
    AppDatabase() << Impl::selectAllPluginCacheCommand()
                  >> [&ret](int id,
                            const std::vector<char>& fingerprint,
                            int property)
    {
        ret.emplace_back(std::make_tuple(id, fingerprint, property));
    };
    return ret;
}

Musec::Base::PluginCacheInDatabase selectPluginCacheByFingerprint(const std::vector<char>& fingerprint)
{
    Musec::Base::PluginCacheInDatabase ret;
    AppDatabase() << Impl::selectPluginCacheByFingerprintCommand() << fingerprint
                  >> [&ret](int id,
                            const std::vector<char>& fingerprint,
                            int property)
    {
        ret = std::make_tuple(id, fingerprint, property);
    };
    return ret;
}

void updatePluginCacheById(int id, const std::vector<char>& fingerprint, int properties)
{
    AppDatabase() << Impl::updatePluginCacheByIdCommand() << fingerprint << properties << id;
}

}
}