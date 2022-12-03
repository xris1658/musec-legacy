#include "VST3Host.hpp"

#include "base/Base.hpp"
#include "base/Constants.hpp"

#include <public.sdk/source/vst/hosting/hostclasses.h>

#include <cwchar>

namespace Musec::Audio::Host
{
tresult PLUGIN_API VST3Host::getName(String128 name)
{
    for(int i = 0; i < Musec::Base::stackArraySize(Musec::Base::ProductName); ++i)
    {
        name[i] = Musec::Base::ProductName[i];
    }
    return kResultOk;
}

VST3Host& VST3Host::instance()
{
    static VST3Host ret;
    return ret;
}

tresult PLUGIN_API VST3Host::queryInterface(const char* iid_, void** obj)
{
    QUERY_INTERFACE(iid_, obj, FUnknown::iid, IHostApplication)
    QUERY_INTERFACE(iid_, obj, IHostApplication::iid, IHostApplication)
    *obj = nullptr;
    return kNoInterface;
}

uint32 PLUGIN_API VST3Host::addRef()
{
    return 1;
}

uint32 PLUGIN_API VST3Host::release()
{
    return 1;
}

tresult PLUGIN_API VST3Host::createInstance(TUID cid, TUID iid_, void** obj)
{
    auto classId = FUID::fromTUID(cid);
    auto interfaceId = FUID::fromTUID(iid_);
    if(classId == IMessage::iid && interfaceId == IMessage::iid)
    {
        // TODO: Allocate using memory pool
        *obj = new HostMessage;
        return kResultOk;
    }
    *obj = nullptr;
    return kNotImplemented;
}

ProcessContext& VST3Host::AppProcessContext()
{
    static Steinberg::Vst::ProcessContext ret;
    return ret;
}
}
