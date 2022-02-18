#ifndef MUSEC_CONTROLLER_ASSETDIRECTORYCONTROLLER
#define MUSEC_CONTROLLER_ASSETDIRECTORYCONTROLLER

#include "base/AssetDirectoryBase.hpp"
#include "model/AssetDirectoryListModel.hpp"
#include "dao/AssetDirectoryDAO.hpp"

#include <QString>

namespace Musec
{
namespace Controller
{
QList<Musec::Base::AssetDirectoryInfo> getAssetDirectory();

void addAssetDirectory(const QString& directory);

void renameAssetDirectory(int id, const QString& name);

void removeAssetDirectory(int id);
}
}

#endif // MUSEC_CONTROLLER_ASSETDIRECTORYCONTROLLER
