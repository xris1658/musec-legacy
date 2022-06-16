#include "VST3ComponentHandler.hpp"

namespace Musec::Audio::Host
{
tresult PLUGIN_API VST3ComponentHandler::queryInterface(const int8* iid, void** obj)
{
    QUERY_INTERFACE(iid, obj, FUnknown::iid, VST3ComponentHandler)
    QUERY_INTERFACE(iid, obj, IComponentHandler::iid, VST3ComponentHandler)
    QUERY_INTERFACE(iid, obj, IComponentHandler2::iid, VST3ComponentHandler)
    QUERY_INTERFACE(iid, obj, IUnitHandler::iid, VST3ComponentHandler)
    QUERY_INTERFACE(iid, obj, IUnitHandler2::iid, VST3ComponentHandler)
    return kNoInterface;
}

// 使用 magic static，且使用此类的插件均在此类析构之前销毁。因此直接返回 1 即可。
uint32 PLUGIN_API VST3ComponentHandler::addRef()
{
    return 1U;
}

// 同上
uint32 PLUGIN_API VST3ComponentHandler::release()
{
    return 1U;
}

tresult PLUGIN_API VST3ComponentHandler::beginEdit(ParamID id)
{
    return kResultOk;
}

tresult PLUGIN_API VST3ComponentHandler::performEdit(ParamID id, ParamValue valueNormalized)
{
    return kResultOk;
}

tresult PLUGIN_API VST3ComponentHandler::endEdit(ParamID id)
{
    return kResultOk;
}

tresult PLUGIN_API VST3ComponentHandler::restartComponent(int32 flags)
{
    if(flags == 0)
    {
        return kResultOk;
    }
    if(flags & RestartFlags::kReloadComponent)
    {
        flags -= RestartFlags::kReloadComponent;
    }
    if(flags & RestartFlags::kIoChanged)
    {
        flags -= RestartFlags::kIoChanged;
    }
    if(flags & RestartFlags::kParamValuesChanged)
    {
        flags -= RestartFlags::kParamValuesChanged;
    }
    if(flags & RestartFlags::kLatencyChanged)
    {
        flags -= RestartFlags::kLatencyChanged;
    }
    if(flags & RestartFlags::kParamTitlesChanged)
    {
        flags -= RestartFlags::kParamTitlesChanged;
    }
    if(flags & RestartFlags::kMidiCCAssignmentChanged)
    {
        flags -= RestartFlags::kMidiCCAssignmentChanged;
    }
    if(flags & RestartFlags::kNoteExpressionChanged)
    {
        flags -= RestartFlags::kNoteExpressionChanged;
    }
    if(flags & RestartFlags::kIoTitlesChanged)
    {
        flags -= RestartFlags::kIoTitlesChanged;
    }
    if(flags & RestartFlags::kPrefetchableSupportChanged)
    {
        flags -= RestartFlags::kPrefetchableSupportChanged;
    }
    if(flags & RestartFlags::kRoutingInfoChanged)
    {
        flags -= RestartFlags::kRoutingInfoChanged;
    }
    if(flags & kKeyswitchChanged)
    {
        flags -= RestartFlags::kKeyswitchChanged;
    }
    return kResultOk;
}

tresult VST3ComponentHandler::setDirty(TBool state)
{
    return kResultOk;
}

tresult VST3ComponentHandler::requestOpenEditor(FIDString name)
{
    return kNotImplemented;
}

tresult VST3ComponentHandler::startGroupEdit()
{
    return kNotImplemented;
}

tresult VST3ComponentHandler::finishGroupEdit()
{
    return kNotImplemented;
}

tresult VST3ComponentHandler::notifyUnitSelection(UnitID unitId)
{
    return kNotImplemented;
}

tresult VST3ComponentHandler::notifyProgramListChange(ProgramListID listId, int32 programIndex)
{
    return kNotImplemented;
}

tresult VST3ComponentHandler::notifyUnitByBusChange()
{
    return kNotImplemented;
}

VST3ComponentHandler& VST3ComponentHandler::instance()
{
    static VST3ComponentHandler ret;
    return ret;
}
}


