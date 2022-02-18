#ifndef MUSEC_CONTROLLER_PLUGINSETTINGSCONTROLLER
#define MUSEC_CONTROLLER_PLUGINSETTINGSCONTROLLER

#include <QString>

namespace Musec
{
namespace Controller
{
// 获取插件目录
QStringList getPluginDirectoryList();

// 添加插件目录
void addPluginDirectory(const QString&);

// 移除插件目录
void removePluginDirectory(const QString&);

// 扫描所有目录的插件
void scanPlugins();
}
}

#endif // MUSEC_CONTROLLER_PLUGINSETTINGSCONTROLLER
