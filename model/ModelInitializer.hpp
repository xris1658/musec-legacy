#ifndef MUSEC_MODEL_MODELINITIALIZER
#define MUSEC_MODEL_MODELINITIALIZER

#include "ASIODriverListModel.hpp"
#include "AssetDirectoryListModel.hpp"
#include "PluginListModel.hpp"

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
