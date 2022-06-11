#ifndef MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER
#define MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER

#include <pluginterfaces/vst/ivsteditcontroller.h>

namespace Musec
{
namespace Audio
{
namespace Host
{
using namespace Steinberg;
using namespace Steinberg::Vst;

class VST3ComponentHandler: public IComponentHandler
{
private:
    VST3ComponentHandler() = default;
public:
    ~VST3ComponentHandler() = default;
public:
    DECLARE_FUNKNOWN_METHODS
public:
    tresult PLUGIN_API beginEdit(ParamID id) override;
    tresult PLUGIN_API performEdit(ParamID id, ParamValue valueNormalized) override;
    tresult PLUGIN_API endEdit(ParamID id) override;
    tresult PLUGIN_API restartComponent(int32 flags) override;
public:
    static VST3ComponentHandler& instance();
};
}
}
}

#endif //MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER
