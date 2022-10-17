#ifndef MUSEC_CONTROLLER_ASIODRIVERCONTROLLER
#define MUSEC_CONTROLLER_ASIODRIVERCONTROLLER

// QUES: Moving the following line to ASIODrvierController.cpp generates C2664,
// indicating that ASIOSampleRate and double are not same
#include "audio/driver/ASIOCallback.hpp"

#include "model/ASIOChannelInfoListModel.hpp"

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

Musec::Model::ASIOChannelInfoListModel& inputChannelInfoList();

Musec::Model::ASIOChannelInfoListModel& outputChannelInfoList();

void resetASIODriver();
}
}
}

#endif //MUSEC_CONTROLLER_ASIODRIVERCONTROLLER
