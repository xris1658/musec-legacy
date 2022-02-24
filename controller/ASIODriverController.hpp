#ifndef MUSEC_CONTROLLER_ASIODRIVERCONTROLLER
#define MUSEC_CONTROLLER_ASIODRIVERCONTROLLER

#include "controller/AppController.hpp"
#include "controller/ConfigController.hpp"
#include "audio/driver/ASIOCallback.hpp"
#include "dao/DatabaseDAO.hpp"
#include "event/EventBase.hpp"
#include "ui/UI.hpp"

#include <QString>

namespace Musec
{
namespace Controller
{
namespace ASIODriverController
{
// 加载 ASIO 驱动
void loadASIODriver();

// 获取当前 ASIO 驱动的 CLSID
QString getASIODriver();

// 切换 ASIO 驱动
void setASIODriver(const QString& clsid);

// 打开 ASIO 驱动的控制面板
void openASIODriverControlPanel();

// 卸载 ASIO 驱动
void unloadASIODriver();

// 更新 ASIO 驱动的信息
void updateCurrentASIODriverInfo();
}
}
}

#endif //MUSEC_CONTROLLER_ASIODRIVERCONTROLLER
