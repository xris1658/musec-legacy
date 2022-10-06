#include "ASIOSampleTypeName.hpp"

#include <QCoreApplication>

namespace Musec::Audio::Driver
{
QString getASIOSampleTypeName(ASIOSampleType type)
{
    switch(type)
    {
    case ASIOSampleTypeEnum::ASIOSTInt16MSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "16-Bit Integer MSB");
    case ASIOSampleTypeEnum::ASIOSTInt24MSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "24-Bit Integer MSB");
    case ASIOSampleTypeEnum::ASIOSTInt32MSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "32-Bit Integer MSB");
    case ASIOSampleTypeEnum::ASIOSTFloat32MSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "IEEE-754 32-Bit Float MSB");
    case ASIOSampleTypeEnum::ASIOSTFloat64MSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "IEEE-754 64-Bit Float MSB");
    case ASIOSampleTypeEnum::ASIOSTInt32MSB16:
        return QCoreApplication::translate("ASIOSampleTypeName", "16-Bit Integer MSB inside 32-Bit");
    case ASIOSampleTypeEnum::ASIOSTInt32MSB18:
        return QCoreApplication::translate("ASIOSampleTypeName", "18-Bit Integer MSB inside 32-Bit");
    case ASIOSampleTypeEnum::ASIOSTInt32MSB20:
        return QCoreApplication::translate("ASIOSampleTypeName", "20-Bit Integer MSB inside 32-Bit");
    case ASIOSampleTypeEnum::ASIOSTInt32MSB24:
        return QCoreApplication::translate("ASIOSampleTypeName", "24-Bit Integer MSB inside 32-Bit");
    case ASIOSampleTypeEnum::ASIOSTInt16LSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "16-Bit Integer LSB");
    case ASIOSampleTypeEnum::ASIOSTInt24LSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "24-Bit Integer LSB");
    case ASIOSampleTypeEnum::ASIOSTInt32LSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "32-Bit Integer LSB");
    case ASIOSampleTypeEnum::ASIOSTFloat32LSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "IEEE-754 32-Bit Float LSB");
    case ASIOSampleTypeEnum::ASIOSTFloat64LSB:
        return QCoreApplication::translate("ASIOSampleTypeName", "IEEE-754 64-Bit Float LSB");
    case ASIOSampleTypeEnum::ASIOSTInt32LSB16:
        return QCoreApplication::translate("ASIOSampleTypeName", "16-Bit Integer LSB inside 32-Bit");
    case ASIOSampleTypeEnum::ASIOSTInt32LSB18:
        return QCoreApplication::translate("ASIOSampleTypeName", "18-Bit Integer LSB inside 32-Bit");
    case ASIOSampleTypeEnum::ASIOSTInt32LSB20:
        return QCoreApplication::translate("ASIOSampleTypeName", "20-Bit Integer LSB inside 32-Bit");
    case ASIOSampleTypeEnum::ASIOSTInt32LSB24:
        return QCoreApplication::translate("ASIOSampleTypeName", "24-Bit Integer LSB inside 32-Bit");
    case ASIOSampleTypeEnum::ASIOSTDSDInt8LSB1:
        return QCoreApplication::translate("ASIOSampleTypeName", "DSD 1-Bit (8 samples per byte, first sample in LSB)");
    case ASIOSampleTypeEnum::ASIOSTDSDInt8MSB1:
        return QCoreApplication::translate("ASIOSampleTypeName", "DSD 1-Bit (8 samples per byte, first sample in MSB)");
    case ASIOSampleTypeEnum::ASIOSTDSDInt8NER8:
        return QCoreApplication::translate("ASIOSampleTypeName", "DSD 8-Bit (1 sample per type, no endianness required)");
    default:
        return QCoreApplication::translate("ASIOSampleTypeName", "Unknown sample type");
    }
}
}