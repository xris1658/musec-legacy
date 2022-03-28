#include "ModelInitializer.hpp"

#include "model/ASIODriverListModel.hpp"
#include "model/AssetDirectoryListModel.hpp"
#include "model/FileListModel.hpp"
#include "model/FolderListModel.hpp"
#include "model/PluginListModel.hpp"

#include <qqml.h>

namespace Musec::Model
{
ModelInitializer::ModelInitializer()
{
    QML_REGISTER_TYPE(AssetDirectoryListModel, Musec.Models, 1, 0);
    QML_REGISTER_TYPE(PluginListModel, Musec.Models, 1, 0);
    QML_REGISTER_TYPE(ASIODriverListModel, Musec.Models, 1, 0);
    QML_REGISTER_TYPE(FileListModel, Musec.Models, 1, 0);
    QML_REGISTER_TYPE(FolderListModel, Musec.Models, 1, 0);
}

void ModelInitializer::initialize()
{
    static ModelInitializer ret;
}
}
