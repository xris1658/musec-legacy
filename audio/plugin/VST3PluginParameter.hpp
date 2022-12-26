#ifndef MUSEC_AUDIO_PLUGIN_VST3PLUGINPARAMETER
#define MUSEC_AUDIO_PLUGIN_VST3PLUGINPARAMETER

#include "audio/base/Automation.hpp"
#include "audio/plugin/IParameter.hpp"
#include "audio/plugin/VST3Plugin.hpp"

#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
class VST3PluginParameter: public Musec::Audio::Plugin::IParameter
{
public:
    VST3PluginParameter(Steinberg::Vst::IEditController* editController, std::int32_t index);
public:
    std::uint32_t id() const override;
    QString name() const override;
    double minValue() const override;
    double maxValue() const override;
    double defaultValue() const override;
    double value() const override;
    void setValue(double value) override;
    double stepSize() const override;
public:
    QString valueToString(double value) const override;
    double stringToValue(const QString& string) const override;
public:
    const Steinberg::Vst::ParameterInfo& getParameterInfo() const;
private:
    Steinberg::Vst::IEditController* editController_ = nullptr;
    Steinberg::Vst::ParameterInfo parameterInfo_ = {};
    std::int32_t index_ = -1;
};

void automationToParamList(const VST3PluginParameter& parameter,
    const Musec::Audio::Base::Automation& automation,
    Musec::Audio::Base::Automation::TimeType from, // inclusive [
    Musec::Audio::Base::Automation::TimeType to,   // exclusive )
    Musec::Audio::Base::Automation::TimeType precision,
    Steinberg::Vst::IParamValueQueue& paramList
);
}
}
}


#endif //MUSEC_AUDIO_PLUGIN_VST3PLUGINPARAMETER
