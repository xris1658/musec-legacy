#ifndef MUSEC_CONTROLLER_ASSETCONTROLLER
#define MUSEC_CONTROLLER_ASSETCONTROLLER

#include "model/FileListModel.hpp"
#include "model/FolderListModel.hpp"

namespace Musec
{
namespace Controller
{
namespace AssetController
{
Musec::Model::FileListModel::List getFolderInDirectory(const QString& directory);

Musec::Model::FolderListModel::List getFileInDirectory(const QString& directory);
}
}
}

#endif //MUSEC_CONTROLLER_ASSETCONTROLLER
