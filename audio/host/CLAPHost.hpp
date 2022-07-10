#ifndef MUSEC_AUDIO_HOST_CLAPHOST
#define MUSEC_AUDIO_HOST_CLAPHOST

#include <clap/host.h>
#include <clap/ext/gui.h>
#include <clap/ext/params.h>

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Host
{
const clap_host& AppCLAPHost();

clap_host_gui& AppCLAPHostGUI();

clap_host_params& AppCLAPHostParams();
}
}
}

#endif //MUSEC_AUDIO_HOST_CLAPHOST
