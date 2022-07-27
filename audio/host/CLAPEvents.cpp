#include "CLAPEvents.hpp"

namespace Musec::Audio::Host
{
clap_event_transport& AppCLAPEventTransport()
{
    static clap_event_transport ret;
    return ret;
}
}