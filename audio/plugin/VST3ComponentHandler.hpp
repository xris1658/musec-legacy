#ifndef MUSEC_AUDIO_PLUGIN_VST3COMPONENTHANDLER
#define MUSEC_AUDIO_PLUGIN_VST3COMPONENTHANDLER

#include <pluginterfaces/vst/ivsteditcontroller.h>

#include <map>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
class VST3Plugin;

class VST3ComponentHandler:
    public Steinberg::Vst::IComponentHandler,
    public Steinberg::Vst::IComponentHandler2
{
public:
    VST3ComponentHandler(Musec::Audio::Plugin::VST3Plugin* plugin = nullptr);
    VST3ComponentHandler(const VST3ComponentHandler&) = delete;
    VST3ComponentHandler(VST3ComponentHandler&&) = default;
    ~VST3ComponentHandler() {}
public: // FUnknown interfaces
    Steinberg::tresult queryInterface(const Steinberg::TUID iid, void** obj) override;
    Steinberg::uint32 addRef() override;
    Steinberg::uint32 release() override;
public: // IComponentHandler interfaces
    Steinberg::tresult PLUGIN_API beginEdit(Steinberg::Vst::ParamID id) override;
    Steinberg::tresult PLUGIN_API performEdit(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue valueNormalized) override;
    Steinberg::tresult PLUGIN_API endEdit(Steinberg::Vst::ParamID id) override;
    Steinberg::tresult PLUGIN_API restartComponent(Steinberg::int32 flags) override;
public: // IComponentHandler2 interfaces
    Steinberg::tresult PLUGIN_API setDirty(Steinberg::TBool state) override;
    Steinberg::tresult PLUGIN_API requestOpenEditor(Steinberg::FIDString name = Steinberg::Vst::ViewType::kEditor) override;
    Steinberg::tresult PLUGIN_API startGroupEdit() override;
    Steinberg::tresult PLUGIN_API finishGroupEdit() override;
private:
    Musec::Audio::Plugin::VST3Plugin* plugin_;
    std::map<Steinberg::Vst::ParamID, int> paramIdAndIndex_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3COMPONENTHANDLER
