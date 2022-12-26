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
    if(parameterInfo_.flags & Steinberg::Vst::ParameterInfo::ParameterFlags::kIsList)
    {
        IParameter::flags_ |= ParameterFlags::ShowAsList;
    }
    IParameter::flags_ |= ParameterFlags::SupportMinMaxValue;
    IParameter::flags_ |= ParameterFlags::SupportDefaultValue;
}

std::uint32_t VST3PluginParameter::id() const
{
    return parameterInfo_.id;
}

QString VST3PluginParameter::name() const
{
    return QString::fromUtf16(parameterInfo_.title);
}

double VST3PluginParameter::minValue() const
{
    return editController_->normalizedParamToPlain(parameterInfo_.id, 0.0);
}

double VST3PluginParameter::maxValue() const
{
    return editController_->normalizedParamToPlain(parameterInfo_.id, 1.0);
}

double VST3PluginParameter::defaultValue() const
{
    return editController_->normalizedParamToPlain(parameterInfo_.id, parameterInfo_.defaultNormalizedValue);
}

double VST3PluginParameter::value() const
{
    auto normalizedValue = editController_->getParamNormalized(parameterInfo_.id);
    return parameterInfo_.stepCount?
        editController_->normalizedParamToPlain(parameterInfo_.id, normalizedValue):
        normalizedValue;
}

void VST3PluginParameter::setValue(double value)
{
    // FIXME
    if(auto stepCount = parameterInfo_.stepCount)
    {
        editController_->setParamNormalized(
            parameterInfo_.id,
            editController_->plainParamToNormalized(parameterInfo_.id, value)
        );
    }
    editController_->setParamNormalized(parameterInfo_.id, value);
}

double VST3PluginParameter::stepSize() const
{
    return parameterInfo_.stepCount == 0?
        parameterInfo_.stepCount:
        (maxValue() - minValue()) / static_cast<double>(parameterInfo_.stepCount);
}

const Steinberg::Vst::ParameterInfo& VST3PluginParameter::getParameterInfo() const
{
    return parameterInfo_;
}

QString VST3PluginParameter::valueToString(double value) const
{
    auto normalizedValue = editController_->plainParamToNormalized(parameterInfo_.id, value);
    String128 string;
    if(editController_->getParamStringByValue(parameterInfo_.id, normalizedValue, string) == Steinberg::kResultOk
        && string[0] != 0)
    {
        return QString::fromUtf16(string);
    }
    return QString::number(value);
}

double VST3PluginParameter::stringToValue(const QString& string) const
{
    ParamValue ret = 0;
    auto getValueResult = editController_->getParamValueByString(parameterInfo_.id, const_cast<Steinberg::Vst::TChar*>(reinterpret_cast<const Steinberg::Vst::TChar*>(string.data())), ret);
    return getValueResult == Steinberg::kResultOk? ret: -1;
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
}
