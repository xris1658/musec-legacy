#include "Literals.hpp"

namespace Musec::Audio::Driver::Literals
{
const char* asioErrorMessage(ASIOError error)
{
    using ASIOErrorEnum = decltype(ASE_OK);
    switch(error)
    {
    case ASIOErrorEnum::ASE_OK:
        return asioErrorOk;
    case ASIOErrorEnum::ASE_SUCCESS:
        return asioErrorSuccess;
    case ASIOErrorEnum::ASE_NotPresent:
        return asioErrorNotPresent;
    case ASIOErrorEnum::ASE_HWMalfunction:
        return asioErrorHardwareMalfunction;
    case ASIOErrorEnum::ASE_InvalidParameter:
        return asioErrorInvalidParameter;
    case ASIOErrorEnum::ASE_InvalidMode:
        return asioErrorInvalidMode;
    case ASIOErrorEnum::ASE_SPNotAdvancing:
        return asioErrorSamplePositionIsNotAdvancing;
    case ASIOErrorEnum::ASE_NoClock:
        return asioErrorNoClock;
    case ASIOErrorEnum::ASE_NoMemory:
        return asioErrorNoMemory;
    default:
        break;
    }
    return asioErrorUnknown;
}
}