#include "ASIOErrorText.hpp"

#include <QCoreApplication>

namespace Musec::Audio::Driver
{
QString getASIOErrorText(ASIOError error)
{
    switch(error)
    {
    case ASIOErrorEnum::ASE_OK:
        return QCoreApplication::translate("ASIOErrorText", "The operation is done successfully.");
    case ASIOErrorEnum::ASE_SUCCESS:
        return QCoreApplication::translate("ASIOErrorText", "The operation is done successfully.");
    case ASIOErrorEnum::ASE_NotPresent:
        return QCoreApplication::translate("ASIOErrorText", "There is no hardware input or output devices, or the devices are not available.");
    case ASIOErrorEnum::ASE_HWMalfunction:
        return QCoreApplication::translate("ASIOErrorText", "The current hardware is malfunctioning.");
    case ASIOErrorEnum::ASE_InvalidParameter:
        return QCoreApplication::translate("ASIOErrorText", "The current parameter is invalid. You might need to check settings of the audio driver.");
    case ASIOErrorEnum::ASE_InvalidMode:
        return QCoreApplication::translate("ASIOErrorText", "The mode of the hardware is invalid.");
    case ASIOErrorEnum::ASE_SPNotAdvancing:
        return QCoreApplication::translate("ASIOErrorText", "The hardware can't return the sample position.");
    case ASIOErrorEnum::ASE_NoClock:
        return QCoreApplication::translate("ASIOErrorText", "The sample rate selected is not supported by the hardware.");
    case ASIOErrorEnum::ASE_NoMemory:
        return QCoreApplication::translate("ASIOErrorText", "The available memory space is not enough to complete current operation.");
    default:
        return QCoreApplication::translate("ASIOErrorText", "An error occurred. Unfortunately, that's all we know.");
    }
}
}