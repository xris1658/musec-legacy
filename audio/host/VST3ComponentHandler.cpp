#include "VST3ComponentHandler.hpp"

namespace Musec::Audio::Host
{
Steinberg::tresult VST3ComponentHandler::queryInterface(const Steinberg::int8* _iid, void** obj)
{
    if(_iid == Steinberg::Vst::IComponentHandler_iid)
    {
        *obj = this;
        return Steinberg::kResultOk;
    }
    return Steinberg::kNoInterface;
}

Steinberg::uint32 VST3ComponentHandler::addRef()
{
    return 1;
}

Steinberg::uint32 VST3ComponentHandler::release()
{
    return 0;
}

Steinberg::tresult VST3ComponentHandler::beginEdit(Steinberg::Vst::ParamID id)
{
    return Steinberg::kResultOk;
}

Steinberg::tresult VST3ComponentHandler::performEdit(Steinberg::Vst::ParamID id,
    Steinberg::Vst::ParamValue valueNormalized)
{
    return Steinberg::kResultOk;
}

Steinberg::tresult VST3ComponentHandler::endEdit(Steinberg::Vst::ParamID id)
{
    return Steinberg::kResultOk;
}

Steinberg::tresult VST3ComponentHandler::restartComponent(Steinberg::int32 flags)
{
    return Steinberg::kResultOk;
}

VST3ComponentHandler& VST3ComponentHandler::instance()
{
    static VST3ComponentHandler ret;
    return ret;
}
}


