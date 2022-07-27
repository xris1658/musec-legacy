#include "CLAPPluginParameter.hpp"

namespace Musec::Audio::Plugin
{

CLAPPluginParameter::CLAPPluginParameter(const clap_plugin* plugin, const clap_plugin_params* params, uint32_t index):
    plugin_(plugin), params_(params), index_(index)
{
    params_->get_info(plugin_, index_, &paramInfo_);
    auto flags = paramInfo_.flags;
    if(flags & CLAP_PARAM_IS_STEPPED)
    {
        flags_ |= ParameterSupportFlags::SupportDiscrete;
    }
    if(flags & CLAP_PARAM_IS_PERIODIC)
    {
        flags_ |= ParameterSupportFlags::SupportPeriodic;
    }
    if(flags & CLAP_PARAM_IS_HIDDEN)
    {
        flags_ |= ParameterSupportFlags::SupportHidden;
    }
    if(flags & CLAP_PARAM_IS_READONLY)
    {
        flags_ |= ParameterSupportFlags::SupportReadonly;
    }
    if(flags & CLAP_PARAM_IS_AUTOMATABLE)
    {
        flags_ |= ParameterSupportFlags::SupportAutomatable;
    }
    flags |= ParameterSupportFlags::SupportMinMaxValue;
    flags |= ParameterSupportFlags::SupportDefaultValue;
}

QString CLAPPluginParameter::name() const
{
    // CLAP: All strings are valid UTF-8
    return QString::fromUtf8(paramInfo_.name);
}

double CLAPPluginParameter::minValue() const
{
    return paramInfo_.min_value;
}

double CLAPPluginParameter::maxValue() const
{
    return paramInfo_.max_value;
}

double CLAPPluginParameter::defaultValue() const
{
    return paramInfo_.default_value;
}

double CLAPPluginParameter::value() const
{
    double ret;
    params_->get_value(plugin_, paramInfo_.id, &ret);
    return ret;
}

void CLAPPluginParameter::setValue(double value)
{
    //
}

double CLAPPluginParameter::step() const
{
    return flags_ & ParameterSupportFlags::SupportDiscrete? 1: 0;
}

void automationToParamList(const CLAPPluginParameter& parameter,
    const Musec::Audio::Base::Automation& automation,
    Musec::Audio::Base::Automation::TimeType from,
    Musec::Audio::Base::Automation::TimeType to,
    Musec::Audio::Base::Automation::TimeType precision,
    CLAPParamQueue& paramList)
{
    for(auto i = from; i < to; i += precision)
    {
        paramList.push({i, automation(i)});
    }
}
}