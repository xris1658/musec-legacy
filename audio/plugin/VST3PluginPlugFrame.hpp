#ifndef MUSEC_AUDIO_PLUGIN_VST3PLUGINPLUGFRAME
#define MUSEC_AUDIO_PLUGIN_VST3PLUGINPLUGFRAME

#include <pluginterfaces/gui/iplugview.h>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
class VST3Plugin;

class VST3PluginPlugFrame: public Steinberg::IPlugFrame
{
public:
    VST3PluginPlugFrame(Musec::Audio::Plugin::VST3Plugin* plugin = nullptr);
    VST3PluginPlugFrame(const VST3PluginPlugFrame&) = delete;
    VST3PluginPlugFrame(VST3PluginPlugFrame&&) = default;
    ~VST3PluginPlugFrame() {}
public: // FUnknown interfaces
    Steinberg::tresult queryInterface(const Steinberg::TUID iid, void** obj) override;
    Steinberg::uint32 addRef() override;
    Steinberg::uint32 release() override;
public:
    Steinberg::tresult resizeView(Steinberg::IPlugView* view,
        Steinberg::ViewRect* newSize) override;
private:
    Musec::Audio::Plugin::VST3Plugin* plugin_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3PLUGINPLUGFRAME
