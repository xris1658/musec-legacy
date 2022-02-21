#include "MusecVST3Host.hpp"

#include <cwchar>

namespace Musec::Audio::Host
{
MusecVST3Host::MusecVST3Host() {}

MusecVST3Host::~MusecVST3Host() {}

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

tresult PLUGIN_API MusecVST3Host::queryInterface(const char* _iid, void** obj)
{
    QUERY_INTERFACE(_iid, obj, FUnknown::iid, IHostApplication)
    QUERY_INTERFACE(_iid, obj, IHostApplication::iid, IHostApplication)

    // 初始化属性表
    return kResultOk;
}

uint32 PLUGIN_API MusecVST3Host::addRef()
{
    return 1;
}

uint32 PLUGIN_API MusecVST3Host::release()
{
    return 1;
}

tresult PLUGIN_API MusecVST3Host::createInstance(TUID cid, TUID _iid, void** obj)
{
    if(cid == IMessage_iid)
    {
        // auto objAsMessage = reinterpret_cast<IMessage*>(&obj);
        // 初始化消息对象
        return kResultOk;
    }
    else if(cid == IAttributeList_iid && _iid == IAttributeList_iid)
    {
        // auto objAsAttributeList = reinterpret_cast<IAttributeList*>(&obj);
        // 初始化属性列表
        return kResultOk;
    }
    *obj = nullptr;
    return kResultFalse;
}
}
