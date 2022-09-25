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
void loadASIODriver();

QString getASIODriver();

void setASIODriver(const QString& clsid);

void openASIODriverControlPanel();

void unloadASIODriver();

void updateCurrentASIODriverInfo();

void allocateASIODriverBuffer();

void startASIODriver();
}
}
}

#endif //MUSEC_CONTROLLER_ASIODRIVERCONTROLLER
