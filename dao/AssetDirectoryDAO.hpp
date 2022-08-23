#ifndef MUSEC_DAO_ASSETDIRECTORYDAO
#define MUSEC_DAO_ASSETDIRECTORYDAO

#include "base/AssetDirectoryBase.hpp"

#include <QString>

namespace Musec
{
namespace DAO
{
void createAssetDirectoryTable();

void addAssetDirectory(const QString& path, const QString& name);

int getAssetDirectoryLastId();

QList<Musec::Base::AssetDirectoryInfo> selectAllAssetDirectory(bool reserve = false);

int getAssetDirectoryCount();

Musec::Base::AssetDirectoryInfo selectAssetDirectoryById(int id);

void updateAssetDirectoryNameById(int id, const QString& name);

void removeAssetDirectoryById(int id);
}
}

#endif // MUSEC_DAO_ASSETDIRECTORYDAO
