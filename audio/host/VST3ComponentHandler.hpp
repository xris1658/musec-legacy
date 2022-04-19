#ifndef MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER
#define MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER

#include <pluginterfaces/vst/ivsteditcontroller.h>

namespace Musec
{
namespace Audio
{
namespace Host
{
class VST3ComponentHandler: public Steinberg::Vst::IComponentHandler
{
public:
    ~VST3ComponentHandler() = default;
public:
    static VST3ComponentHandler& instance();
public:
    Steinberg::tresult queryInterface(const Steinberg::int8* _iid, void** obj) override;
    Steinberg::uint32 addRef() override;
    Steinberg::uint32 release() override;
public:
    Steinberg::tresult beginEdit(Steinberg::Vst::ParamID id) override;
    Steinberg::tresult performEdit(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue valueNormalized) override;
    Steinberg::tresult endEdit(Steinberg::Vst::ParamID id) override;
    Steinberg::tresult restartComponent(Steinberg::int32 flags) override;
};
}
}
}

#endif //MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER
