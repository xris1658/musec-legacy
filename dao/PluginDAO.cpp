#include "PluginDAO.hpp"
#include "dao/DatabaseDAO.hpp"
#include "base/Base.hpp"

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
        "   `uid`    INTEGER NOT NULL,"
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
const char16_t* selectPluginByNameCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM plugin WHERE name = ?";
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
}

QList<Musec::Base::PluginReadInfo> selectAllPlugin(bool reserve)
{
    using namespace Musec::Base;
    using Item = Musec::Base::PluginReadInfo;
    QList<Item> ret;
    if(reserve)
    {
        ret.reserve(getAllPluginCount());
    }
    AppDatabase() << Impl::selectAllPluginCommand()
                  >> [&ret](int id,
                            const std::u16string& path,
                            int uid,
                            const std::u16string& name,
                            int format,
                            int type)
    {
        ret.append(Item(id,
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
}
