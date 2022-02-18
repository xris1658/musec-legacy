#ifndef MUSEC_BASE_ASSETDIRECTORYBASE
#define MUSEC_BASE_ASSETDIRECTORYBASE

#include <QList>
#include <QString>

namespace Musec
{
namespace Base
{
using AssetDirectoryInfo = std::tuple<int, QString, QString>;

enum AssetDirectoryInfoField
{
    FieldId,
    FieldDirectory,
    FieldName
};
}
}

#endif // MUSEC_BASE_ASSETDIRECTORYBASE
