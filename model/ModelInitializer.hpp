#ifndef MUSEC_MODEL_MODELINITIALIZER
#define MUSEC_MODEL_MODELINITIALIZER

#include "model/ASIODriverListModel.hpp"
#include "model/AssetDirectoryListModel.hpp"
#include "model/PluginListModel.hpp"

namespace Musec
{
namespace Model
{
class ModelInitializer
{
private:
    ModelInitializer();
public:
    static void initialize();
};
}
}

#endif // MUSEC_MODEL_MODELINITIALIZER
