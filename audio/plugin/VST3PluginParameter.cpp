#include "VST3PluginParameter.hpp"

#include <algorithm>

namespace Musec::Audio::Plugin
{
VST3PluginParameter::VST3PluginParameter(
    Steinberg::Vst::IEditController* editController,
    std::int32_t index):
    editController_(editController),
    index_(index)
{
    editController_->getParameterInfo(index, parameterInfo_);
    if(parameterInfo_.stepCount != 0)
    {
        IParameter::flags_ |= ParameterFlags::Discrete;
    }
    if(parameterInfo_.flags & Steinberg::Vst::ParameterInfo::ParameterFlags::kIsWrapAround)
    {
        IParameter::flags_ |= ParameterFlags::Periodic;
    }
    if(parameterInfo_.flags & Steinberg::Vst::ParameterInfo::ParameterFlags::kIsHidden)
    {
        IParameter::flags_ |= ParameterFlags::Hidden;
    }
    if(parameterInfo_.flags & Steinberg::Vst::ParameterInfo::ParameterFlags::kIsReadOnly)
    {
        IParameter::flags_ |= ParameterFlags::Readonly;
    }
    if(parameterInfo_.flags & Steinberg::Vst::ParameterInfo::ParameterFlags::kCanAutomate)
    {
        IParameter::flags_ |= ParameterFlags::Automatable;
    }
    IParameter::flags_ |= ParameterFlags::SupportMinMaxValue;
    IParameter::flags_ |= ParameterFlags::SupportDefaultValue;
}

QString VST3PluginParameter::name() const
{
    return QString::fromUtf16(parameterInfo_.title);
}

double VST3PluginParameter::minValue() const
{
    return 0.0;
}

double VST3PluginParameter::maxValue() const
{
    if(step() != 0)
    {
        return parameterInfo_.stepCount;
    }
    return 1.0;
}

double VST3PluginParameter::defaultValue() const
{
    auto defaultNormalizedValue = parameterInfo_.defaultNormalizedValue;
    if(auto stepCount = parameterInfo_.stepCount)
    {
        return defaultNormalizedValue * stepCount;
    }
    return defaultNormalizedValue;
}

double VST3PluginParameter::value() const
{
    auto normalizedValue = editController_->getParamNormalized(parameterInfo_.id);
    if(auto stepCount = parameterInfo_.stepCount)
    {
        return std::min<double>(static_cast<double>(stepCount), normalizedValue * (stepCount + 1));
    }
    return normalizedValue;
}

void VST3PluginParameter::setValue(double value)
{
    if(auto stepCount = parameterInfo_.stepCount)
    {
        editController_->setParamNormalized(parameterInfo_.id, value / static_cast<double>(stepCount));
    }
    editController_->setParamNormalized(parameterInfo_.id, value);
}

double VST3PluginParameter::step() const
{
    return parameterInfo_.stepCount;
}

const Steinberg::Vst::ParameterInfo& VST3PluginParameter::getParameterInfo() const
{
    return parameterInfo_;
}

void automationToParamList(const VST3PluginParameter& parameter,
    const Musec::Audio::Base::Automation& automation,
    Musec::Audio::Base::Automation::TimeType from, // inclusive [
    Musec::Audio::Base::Automation::TimeType to,   // exclusive )
    Musec::Audio::Base::Automation::TimeType precision,
    Steinberg::Vst::IParamValueQueue& paramList
)
{
    Steinberg::int32 index;
    for(auto i = from; i < to; i += precision)
    {
        paramList.addPoint(i, automation(i), index);
    }
}

void VST3PluginParameter::swap(VST3PluginParameter& rhs)
{
    std::swap(editController_, rhs.editController_);
    std::swap(parameterInfo_, rhs.parameterInfo_);
    std::swap(index_, rhs.index_);
}
}
