#include "ModelInitializer.hpp"

#include <qqml.h>

namespace Musec::Model
{
ModelInitializer::ModelInitializer()
{
    QML_REGISTER_TYPE(AssetDirectoryListModel, Musec.Models, 1, 0);
    QML_REGISTER_TYPE(PluginListModel, Musec.Models, 1, 0);
    QML_REGISTER_TYPE(ASIODriverListModel, Musec.Models, 1, 0);
}

void ModelInitializer::initialize()
{
    static ModelInitializer ret;
}
}
