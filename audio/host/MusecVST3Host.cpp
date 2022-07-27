#include "MusecVST3Host.hpp"

#include <public.sdk/source/vst/hosting/hostclasses.h>

#include <cwchar>

namespace Musec::Audio::Host
{
tresult PLUGIN_API MusecVST3Host::getName(String128 name)
{
    std::wcscpy(name, L"Musec");
    return kResultOk;
}

MusecVST3Host& MusecVST3Host::instance()
{
    static MusecVST3Host ret;
    return ret;
}

tresult PLUGIN_API MusecVST3Host::queryInterface(const char* iid_, void** obj)
{
    QUERY_INTERFACE(iid_, obj, FUnknown::iid, IHostApplication)
    QUERY_INTERFACE(iid_, obj, IHostApplication::iid, IHostApplication)

    // 初始化属性表
    return kNoInterface;
}

uint32 PLUGIN_API MusecVST3Host::addRef()
{
    return 1;
}

uint32 PLUGIN_API MusecVST3Host::release()
{
    return 1;
}

tresult PLUGIN_API MusecVST3Host::createInstance(TUID cid, TUID iid, void** obj)
{
    auto classId = FUID::fromTUID(cid);
    auto interfaceId = FUID::fromTUID(iid);
    if(classId == IMessage::iid && interfaceId == IMessage::iid)
    {
        // TODO: 换用从内存池分配
        *obj = new HostMessage;
        return kResultOk;
    }
    else if(classId == IAttributeList::iid && interfaceId == IAttributeList::iid)
    {
        // TODO: 换用从内存池分配
        *obj = new HostAttributeList;
        return kResultOk;
    }
    *obj = nullptr;
    return kNotImplemented;
}

ProcessContext& MusecVST3Host::AppProcessContext()
{
    static Steinberg::Vst::ProcessContext ret;
    return ret;
}
}
