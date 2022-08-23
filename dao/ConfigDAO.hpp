#ifndef MUSEC_DAO_CONFIGDAO
#define MUSEC_DAO_CONFIGDAO

#include <QString>

#include <yaml-cpp/yaml.h>

namespace Musec
{
namespace DAO
{
constexpr auto configFileNameWithBackslash = "\\Musec.yml";
// <User folder>\AppData\Roaming\Musec\Musec.yml
const QString& ConfigFilePath();

void saveAppConfig(const YAML::Node& node, const QString& path);

YAML::Node loadAppConfig(const QString& path);
}
}

#endif // MUSEC_DAO_CONFIGDAO
