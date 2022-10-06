#ifndef MUSEC_AUDIO_DRIVER_ASIODRIVERFORWARD
#define MUSEC_AUDIO_DRIVER_ASIODRIVERFORWARD

#pragma warning(push)
#pragma warning(disable: 4005) // `WINDOWS` macro is redefined

#include <combaseapi.h>

#include <common/iasiodrv.h>

#pragma warning(pop)

using ASIOBoolEnum = decltype(ASIOTrue);
using ASIOSampleTypeEnum = decltype(ASIOSTInt16MSB);
using ASIOErrorEnum = decltype(ASE_OK);
using ASIOMessageSelectorEnum = decltype(kAsioSelectorSupported);
using ASIOFutureSelectorEnum = decltype(kAsioEnableTimeCodeRead);
using ASIOTransportCommandEnum = decltype(kTransStart);

#endif // MUSEC_AUDIO_DRIVER_ASIODRIVERFORWARD
