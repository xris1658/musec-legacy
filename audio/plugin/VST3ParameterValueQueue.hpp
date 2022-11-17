#ifndef MUSEC_AUDIO_PLUGIN_VST3PARAMETERVALUEQUEUE
#define MUSEC_AUDIO_PLUGIN_VST3PARAMETERVALUEQUEUE

#include "base/CircularDeque.hpp"

#include <pluginterfaces/vst/ivstparameterchanges.h>

#include <utility>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using namespace Steinberg;

// TODO: Becoming thread-safe
class VST3ParameterValueQueue: public Steinberg::Vst::IParamValueQueue
{
    struct Point
    {
        Point(int32 sampleOffset, Vst::ParamValue value):
            sampleOffset_(sampleOffset), value_(value)
        {}
        int32 sampleOffset_;
        Vst::ParamValue value_;
    };
public: // ctor, dtor, assignment, etc.
    VST3ParameterValueQueue(Vst::ParamID paramId);
    VST3ParameterValueQueue(const VST3ParameterValueQueue&) = delete;
    VST3ParameterValueQueue(VST3ParameterValueQueue&&) noexcept = default;
    ~VST3ParameterValueQueue() noexcept = default;
public: // FUnknown interfaces
    tresult queryInterface(const TUID _iid, void** obj) override;
    uint32 addRef() override;
    uint32 release() override;
public: // IParamValueQueue interfaces
    Vst::ParamID getParameterId() override;
    int32 getPointCount() override;
    tresult getPoint(int32 index, int32& sampleOffset, Vst::ParamValue& value) override;
    tresult addPoint(int32 sampleOffset, Vst::ParamValue value, int32& index) override;
public: // New functions
    void clear();
private:
    Steinberg::Vst::ParamID paramId_;
    Musec::Base::CircularDeque<Point, 2048> queue_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3PARAMETERVALUEQUEUE
