#ifndef MUSEC_AUDIO_PLUGIN_VST2PLUGINFORWARD
#define MUSEC_AUDIO_PLUGIN_VST2PLUGINFORWARD

#pragma warning(push)
#pragma warning(disable: 4996) // C-style string manipulation might be unsafe
#include <pluginterfaces/vst2.x/aeffect.h>
#pragma warning(pop)

namespace Musec
{
namespace Audio
{
namespace Plugin
{
// Entry function type of VST2 plugins (might be VSTPluginMain or main)
using VST2PluginEntryProc = AEffect* (*)(audioMasterCallback);
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST2PLUGINFORWARD
