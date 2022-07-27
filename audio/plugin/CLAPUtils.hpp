#ifndef MUSEC_AUDIO_PLUGIN_CLAPUTILS
#define MUSEC_AUDIO_PLUGIN_CLAPUTILS

#include <clap/events.h>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using CLAPEventType = decltype(CLAP_EVENT_NOTE_ON);

template<typename CLAPEventStruct>
void fillEventHeader(CLAPEventStruct& event, CLAPEventType eventType)
{
    clap_event_header& header = event.header;
    header.size = sizeof(CLAPEventStruct);
    header.type = eventType;
    header.space_id = CLAP_CORE_EVENT_SPACE_ID;
}
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_CLAPUTILS
