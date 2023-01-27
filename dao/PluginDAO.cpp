#include "PluginDAO.hpp"

#include "base/Base.hpp"
#include "dao/DatabaseDAO.hpp"

#include <string>

namespace Musec::DAO
{
namespace Impl
{
const char16_t* createPluginTableCommand()
{
    static char16_t ret[] =
        u"CREATE TABLE IF NOT EXISTS plugin("
        "   `id`     INTEGER NOT NULL"
        "       CONSTRAINT plugin_pk"
        "           PRIMARY KEY AUTOINCREMENT,"
        "   `path`   TEXT NOT NULL,"
        "   `uid`    BLOB NOT NULL,"
        "   `name`   TEXT NOT NULL,"
        "   `format` INTEGER NOT NULL,"
        "   `type`   INTEGER NOT NULL"
        ");";
    return ret;
}
const char16_t* insertPluginCommand()
{
    static char16_t ret[] =
        u"INSERT INTO plugin (path, uid, name, format, type)"
        "VALUES (?, ?, ?, ?, ?)";
    return ret;
}
const char16_t* removeAllPluginsCommand()
{
    static char16_t ret[] =
        u"DELETE FROM plugin";
    return ret;
}
const char16_t* resetPluginIdFromSequenceCommand()
{
    static char16_t ret[] =
        u"DELETE FROM sqlite_sequence WHERE name = 'plugin'";
    return ret;
}
const char16_t* getAllPluginCountCommand()
{
    static char16_t ret[] =
        u"SELECT COUNT(*) FROM plugin";
    return ret;
}
const char16_t* selectAllPluginCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM plugin";
    return ret;
}
const char16_t* selectPluginByIdCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM plugin WHERE id = ?";
    return ret;
}
const char16_t* selectCountOfPluginByPathCommand()
{
    static char16_t ret[] =
        u"SELECT COUNT(*) FROM plugin WHERE path = ?";
    return ret;
}
const char16_t* selectPluginByPathCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM plugin WHERE path = ?";
    return ret;
}
const char16_t* selectCountOfDistinctPluginPathCommand()
{
    static char16_t ret[] =
        u"SELECT COUNT(DISTINCT path) FROM plugin";
    return ret;
}
const char16_t* selectDistinctPluginPathCommand()
{
    static char16_t ret[] =
        u"SELECT DISTINCT path FROM plugin";
    return ret;
}
const char16_t* removePluginByPathCommand()
{
    static char16_t ret[] =
        u"DELETE FROM plugin WHERE path = ?";
    return ret;
}
}

void createPluginTable()
{
    AppDatabase() << Impl::createPluginTableCommand();
}

void insertPlugin(const Musec::Base::PluginWriteInfo& pluginWriteInfo)
{
    using namespace Musec::Base;
    AppDatabase() << Impl::insertPluginCommand()
                  << std::get<WriteFieldPath>(pluginWriteInfo).toStdU16String()
                  << std::get<WriteFieldUid>(pluginWriteInfo)
                  << std::get<WriteFieldName>(pluginWriteInfo).toStdU16String()
                  << std::get<WriteFieldFormat>(pluginWriteInfo)
                  << std::get<WriteFieldType>(pluginWriteInfo);
}

void removeAllPlugins()
{
    AppDatabase() << Impl::removeAllPluginsCommand();
    AppDatabase() << Impl::resetPluginIdFromSequenceCommand();
}

std::vector<Musec::Base::PluginReadInfo> selectAllPlugin()
{
    using namespace Musec::Base;
    using Item = Musec::Base::PluginReadInfo;
    std::vector<Item> ret;
    ret.reserve(getAllPluginCount());
    AppDatabase() << Impl::selectAllPluginCommand()
                  >> [&ret](int id,
                            const std::u16string& path,
                            const std::vector<char>& uid,
                            const std::u16string& name,
                            int format,
                            int type)
    {
        ret.emplace_back(Item(id,
                        QString::fromStdU16String(path),
                        uid,
                        QString::fromStdU16String(name),
                        format,
                        type));
    };
    return ret;
}

int getAllPluginCount()
{
    int ret;
    AppDatabase() << Impl::getAllPluginCountCommand() >> ret;
    return ret;
}

Musec::Base::PluginReadInfo selectPluginById(int id)
{
    Musec::Base::PluginReadInfo ret;
    AppDatabase() << Impl::selectPluginByIdCommand() << id
                  >> [&ret](int id,
                            const std::u16string& path,
                            const std::vector<char>& uid,
                            const std::u16string& name,
                            int format,
                            int type)
    {
        ret = std::make_tuple(
            id,
            QString::fromStdU16String(path),
            uid,
            QString::fromStdU16String(name),
            format,
            type);
    };
    return ret;
}

std::vector<Musec::Base::PluginReadInfo> selectPluginByPath(const QString& path)
{
    std::vector<Musec::Base::PluginReadInfo> ret;
    int count = 0; AppDatabase() << Impl::selectCountOfPluginByPathCommand() >> count;
    ret.reserve(count);
    AppDatabase() << Impl::selectPluginByPathCommand()
                  >> [&ret](int id,
                      const std::u16string& path,
                      const std::vector<char>& uid,
                      const std::u16string& name,
                      int format,
                      int type)
    {
        ret.emplace_back(Musec::Base::PluginReadInfo(id,
            QString::fromStdU16String(path),
            uid,
            QString::fromStdU16String(name),
            format,
            type));
    };
    return ret;
}

std::vector<QString> selectDistinctPluginPath()
{
    std::vector<QString> ret;
    int count = 0; AppDatabase() << Impl::selectCountOfDistinctPluginPathCommand() >> count;
    ret.reserve(count);
    AppDatabase() << Impl::selectDistinctPluginPathCommand()
                  >> [&ret](const std::u16string& path)
    {
        ret.emplace_back(QString::fromStdU16String(path));
    };
    return ret;
}

void removePluginByPath(const QString& path)
{
    AppDatabase() << Impl::removePluginByPathCommand() << path.toStdU16String();
}
}
