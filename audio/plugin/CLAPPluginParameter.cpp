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
        flags_ |= ParameterFlags::Discrete;
    }
    if(flags & CLAP_PARAM_IS_PERIODIC)
    {
        flags_ |= ParameterFlags::Periodic;
    }
    if(flags & CLAP_PARAM_IS_HIDDEN)
    {
        flags_ |= ParameterFlags::Hidden;
    }
    if(flags & CLAP_PARAM_IS_READONLY)
    {
        flags_ |= ParameterFlags::Readonly;
    }
    if(flags & CLAP_PARAM_IS_AUTOMATABLE)
    {
        flags_ |= ParameterFlags::Automatable;
    }
    flags |= ParameterFlags::SupportMinMaxValue;
    flags |= ParameterFlags::SupportDefaultValue;
}

std::uint32_t CLAPPluginParameter::id() const
{
    return paramInfo_.id;
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
    // TODO
}

double CLAPPluginParameter::stepSize() const
{
    return flags_ & ParameterFlags::Discrete? 1: 0;
}

QString CLAPPluginParameter::valueToString(double value) const
{
    constexpr std::uint32_t bufferSize = 128;
    char buffer[bufferSize];
    if(params_->value_to_text(plugin_, paramInfo_.id, value, buffer, bufferSize)
    && buffer[0] != 0)
    {
        return QString::fromUtf8(buffer);
    }
    return QString::number(value);
}

double CLAPPluginParameter::stringToValue(const QString& string) const
{
    auto utf8 = string.toUtf8();
    double ret;
    return params_->text_to_value(plugin_, paramInfo_.id, utf8.data(), &ret)? ret: -1;
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