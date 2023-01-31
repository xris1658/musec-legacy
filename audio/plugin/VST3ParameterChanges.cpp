#include "VST3ParameterChanges.hpp"

namespace Musec::Audio::Plugin
{
using namespace Steinberg;

VST3ParameterChanges::VST3ParameterChanges()
{
}

tresult VST3ParameterChanges::queryInterface(const int8* _iid, void** obj)
{
    QUERY_INTERFACE(_iid, obj, FUnknown::iid, Vst::IParameterChanges)
    QUERY_INTERFACE(_iid, obj, Vst::IParameterChanges::iid, Vst::IParameterChanges)
    *obj = nullptr;
    return kNoInterface;
}

uint32 VST3ParameterChanges::addRef()
{
    return 1;
}

uint32 VST3ParameterChanges::release()
{
    return 1;
}

int32 VST3ParameterChanges::getParameterCount()
{
    return container_.size();
}

Vst::IParamValueQueue* VST3ParameterChanges::getParameterData(int32 index)
{
    if(index >= 0 && index < getParameterCount())
    {
        return static_cast<Vst::IParamValueQueue*>(&container_[index]);
    }
    return nullptr;
}

Vst::IParamValueQueue* VST3ParameterChanges::addParameterData(const Vst::ParamID& id, int32& index)
{
    auto& reference = container_.emplace_back(id);
    index = container_.size() - 1;
    return static_cast<Vst::IParamValueQueue*>(&reference);
}
}