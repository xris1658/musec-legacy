#ifndef MUSEC_DAO_PLUGINDIRECTORYDAO
#define MUSEC_DAO_PLUGINDIRECTORYDAO

#include <QString>
#include <QList>

namespace Musec
{
namespace DAO
{
void createPluginDirectoryTable();

void addPluginDirectory(const QString& directory);

void removePluginDirectory(const QString& directory);

QList<QString> selectPluginDirectory(bool reserve = false);
}
}

#endif // MUSEC_DAO_PLUGINDIRECTORYDAO
