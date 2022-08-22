#ifndef MUSEC_AUDIO_PLUGIN_VST2PLUGINFORWARD
#define MUSEC_AUDIO_PLUGIN_VST2PLUGINFORWARD

#pragma warning(push)
#pragma warning(disable: 4996) // C 风格字符串操纵可能不安全
#include <pluginterfaces/vst2.x/aeffect.h>
#pragma warning(pop)

namespace Musec
{
namespace Audio
{
namespace Plugin
{
// VST2 插件的入口函数（VSTPluginMain 或 main）
using VST2PluginEntryProc = AEffect* (*)(audioMasterCallback);
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST2PLUGINFORWARD
