#include "VST3ComponentHandler.hpp"

namespace Musec::Audio::Host
{
tresult PLUGIN_API VST3ComponentHandler::queryInterface(const int8* iid, void** obj)
{
    QUERY_INTERFACE(iid, obj, FUnknown::iid, VST3ComponentHandler)
    QUERY_INTERFACE(iid, obj, IComponentHandler::iid, VST3ComponentHandler)
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
    return kResultOk;
}

VST3ComponentHandler& VST3ComponentHandler::instance()
{
    static VST3ComponentHandler ret;
    return ret;
}
}


