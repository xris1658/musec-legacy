#include "Literals.hpp"

#include <ui/UI.hpp>

namespace Musec::Audio::Driver::Literals
{
QString asioErrorMessage(ASIOError error)
{
    using namespace Musec::UI;
    using ASIOErrorEnum = decltype(ASE_OK);
    switch(error)
    {
    case ASIOErrorEnum::ASE_OK:
        return strings->property(asioErrorOk).toString();
    case ASIOErrorEnum::ASE_SUCCESS:
        return strings->property(asioErrorSuccess).toString();
    case ASIOErrorEnum::ASE_NotPresent:
        return strings->property(asioErrorNotPresent).toString();
    case ASIOErrorEnum::ASE_HWMalfunction:
        return strings->property(asioErrorHardwareMalfunction).toString();
    case ASIOErrorEnum::ASE_InvalidParameter:
        return strings->property(asioErrorInvalidParameter).toString();
    case ASIOErrorEnum::ASE_InvalidMode:
        return strings->property(asioErrorInvalidMode).toString();
    case ASIOErrorEnum::ASE_SPNotAdvancing:
        return strings->property(asioErrorSamplePositionIsNotAdvancing).toString();
    case ASIOErrorEnum::ASE_NoClock:
        return strings->property(asioErrorNoClock).toString();
    case ASIOErrorEnum::ASE_NoMemory:
        return strings->property(asioErrorNoMemory).toString();
    default:
        break;
    }
    return strings->property(asioErrorUnknown).toString();
}
}