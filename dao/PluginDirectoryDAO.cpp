#include "PluginDirectoryDAO.hpp"
#include "dao/DatabaseDAO.hpp"

namespace Musec::DAO
{
namespace Impl
{
const char16_t* createPluginDirectoryTableCommand()
{
    static char16_t ret[] =
        u"CREATE TABLE IF NOT EXISTS plugin_directory("
        "   `path` TEXT NOT NULL,"
        "   PRIMARY KEY (`path`)"
        ");";
    return ret;
}
const char16_t* addPluginDirectoryCommand()
{
    static char16_t ret[] =
        u"INSERT INTO plugin_directory (path)"
        "VALUES(?)";
    return ret;
}
const char16_t* removePluginDirectoryCommand()
{
    static char16_t ret[] =
        u"DELETE FROM plugin_directory WHERE path = ?";
    return ret;
}
const char16_t* selectPluginDirectoryCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM plugin_directory";
    return ret;
}

const char16_t* selectPluginDirectoryCountCommand()
{
    static char16_t ret[] =
        u"SELECT COUNT(*) FROM plugin_directory";
    return ret;
}
}

void createPluginDirectoryTable()
{
    AppDatabase() << Impl::createPluginDirectoryTableCommand();
}

void addPluginDirectory(const QString& directory)
{
    AppDatabase() << Impl::addPluginDirectoryCommand()
                  << directory.toStdU16String();
}

void removePluginDirectory(const QString& directory)
{
    AppDatabase() << Impl::removePluginDirectoryCommand()
                  << directory.toStdU16String();
}

QList<QString> selectPluginDirectory(bool reserve)
{
    QList<QString> ret;
    if(reserve)
    {
        int size;
        AppDatabase() << Impl::selectPluginDirectoryCountCommand() >> size;
        ret.reserve(size);
    }
    AppDatabase() << Impl::selectPluginDirectoryCommand()
                  >> [&ret](const std::u16string& item)
    {
        ret.append(QString::fromStdU16String(item));
    };
    return ret;
}
}
