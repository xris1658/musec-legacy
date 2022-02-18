#ifndef MUSEC_DAO_CONFIGDAO
#define MUSEC_DAO_CONFIGDAO

#include <QString>

#include <yaml-cpp/yaml.h>

namespace Musec
{
namespace DAO
{
constexpr auto configFileNameWithBackslash = "\\Musec.yml";
// 设置文件路径（用户文件夹\AppData\Roaming\Musec\Musec.yml）
const QString& ConfigFilePath();

// 写入配置文件
void saveAppConfig(const YAML::Node& node, const QString& path);

// 加载配置文件
YAML::Node loadAppConfig(const QString& path);
}
}

#endif // MUSEC_DAO_CONFIGDAO
