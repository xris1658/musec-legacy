#ifndef MUSEC_AUDIO_PLUGIN_VST3PARAMETERCHANGES
#define MUSEC_AUDIO_PLUGIN_VST3PARAMETERCHANGES

#include "audio/plugin/VST3ParameterValueQueue.hpp"

#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using namespace Steinberg;

// TODO: Becoming thread-safe
class VST3ParameterChanges: public Steinberg::Vst::IParameterChanges
{
public:
    VST3ParameterChanges(int maxParameterCount);
    VST3ParameterChanges(const VST3ParameterChanges&) = delete;
    VST3ParameterChanges(VST3ParameterChanges&&) noexcept = default;
    ~VST3ParameterChanges() noexcept = default;
public: // FUnknown interfaces
    tresult queryInterface(const int8* _iid, void** obj) override;
    uint32 addRef() override;
    uint32 release() override;
public: // IParameterChanges interfaces
    int32 getParameterCount() override;
    Vst::IParamValueQueue* getParameterData(int32 index) override;
    Vst::IParamValueQueue* addParameterData(const Vst::ParamID& id, int32& index) override;
public: // New functions
private:
    std::vector<Musec::Audio::Plugin::VST3ParameterValueQueue> container_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3PARAMETERCHANGES
