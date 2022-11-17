#include "VST3ParameterValueQueue.hpp"

namespace Musec::Audio::Plugin
{
using namespace Steinberg;

VST3ParameterValueQueue::VST3ParameterValueQueue(Vst::ParamID paramId):
    paramId_(paramId)
{

}

tresult VST3ParameterValueQueue::queryInterface(const int8* _iid, void** obj)
{
    QUERY_INTERFACE(_iid, obj, FUnknown::iid, Vst::IParamValueQueue)
    QUERY_INTERFACE(_iid, obj, Vst::IParamValueQueue::iid, Vst::IParamValueQueue)
    *obj = nullptr;
    return kNoInterface;
}

uint32 VST3ParameterValueQueue::addRef()
{
    return 1;
}

uint32 VST3ParameterValueQueue::release()
{
    return 1;
}

Vst::ParamID VST3ParameterValueQueue::getParameterId()
{
    return paramId_;
}

int32 VST3ParameterValueQueue::getPointCount()
{
    return queue_.size();
}

tresult VST3ParameterValueQueue::getPoint(int32 index, int32& sampleOffset,
    Vst::ParamValue& value)
{
    if(index >= 0 && index < getPointCount())
    {
        const auto& point = queue_[index];
        sampleOffset = point.sampleOffset_;
        value = point.value_;
        return kResultOk;
    }
    return kInvalidArgument;
}

tresult VST3ParameterValueQueue::addPoint(int32 sampleOffset, Vst::ParamValue value,
    int32& index)
{
    if(!queue_.full())
    {
        queue_.emplaceBack(sampleOffset, value);
        index = queue_.size() - 1;
        return kResultOk;
    }
    return kResultFalse;
}

void VST3ParameterValueQueue::clear()
{
    queue_.clear();
}
}