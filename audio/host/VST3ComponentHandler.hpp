#ifndef MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER
#define MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER

#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstunits.h>

namespace Musec
{
namespace Audio
{
namespace Host
{
using namespace Steinberg;
using namespace Steinberg::Vst;

class VST3ComponentHandler:
    public IComponentHandler,
    public IComponentHandler2,
    public IUnitHandler,
    public IUnitHandler2
{
private:
    VST3ComponentHandler() = default;
public:
    ~VST3ComponentHandler() = default;
public:
    DECLARE_FUNKNOWN_METHODS
public: // IComponentHandler 接口
    tresult PLUGIN_API beginEdit(ParamID id) override;
    tresult PLUGIN_API performEdit(ParamID id, ParamValue valueNormalized) override;
    tresult PLUGIN_API endEdit(ParamID id) override;
    tresult PLUGIN_API restartComponent(int32 flags) override;
public: // IComponentHandler2 接口
    tresult PLUGIN_API setDirty(TBool state) override;
    tresult PLUGIN_API requestOpenEditor(FIDString name) override;
    tresult PLUGIN_API startGroupEdit() override;
    tresult PLUGIN_API finishGroupEdit() override;
public: // IUnitHandler 接口
    tresult PLUGIN_API notifyUnitSelection(UnitID unitId) override;
    tresult PLUGIN_API notifyProgramListChange(ProgramListID listId, int32 programIndex) override;
public: // IUnitHandler2 接口
    tresult PLUGIN_API notifyUnitByBusChange() override;
public:
    static VST3ComponentHandler& instance();
};
}
}
}

#endif //MUSEC_AUDIO_HOST_VST3COMPONENTHANDLER
