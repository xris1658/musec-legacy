#include "VST2PluginParameter.hpp"

#include <utility>

namespace Musec::Audio::Plugin
{
VST2PluginParameter::VST2PluginParameter(
    Musec::Audio::Plugin::VST2Plugin& plugin,
    VstInt32 index):
    IParameter(),
    effect_(plugin.effect()),
    index_(index)
{
    if(index_ < 0 || effect_->numParams < index_)
    {
        throw std::out_of_range("Error: parameter index out of range.");
    }
    flags_ &= (ParameterSupportFlags::SupportAutomatable | ParameterSupportFlags::SupportMinMaxValue);
    if(effect_->dispatcher(effect_, AEffectXOpcodes::effGetParameterProperties, index_, 0, &parameterProperties_, 0.0) == 1)
    {
        if(parameterProperties_.flags &
            (VstParameterFlags::kVstParameterIsSwitch
            | VstParameterFlags::kVstParameterUsesIntStep
            | VstParameterFlags::kVstParameterUsesFloatStep)
        )
        {
            flags_ &= ParameterSupportFlags::SupportDiscrete;
        }
        else
        {
            parameterPropertiesValid_ = false;
        }
    }
}

QString VST2PluginParameter::name() const
{
    //
    char buffer[64];
    effect_->dispatcher(effect_, AEffectOpcodes::effGetParamName, index_, 0, &buffer, 0);
    return QString(buffer);
}

double VST2PluginParameter::minValue() const
{
    if(parameterPropertiesValid_ && (parameterProperties_.flags & VstParameterFlags::kVstParameterUsesIntStep))
    {
        return parameterProperties_.minInteger;
    }
    return 0.0;
}

double VST2PluginParameter::maxValue() const
{
    if(parameterPropertiesValid_ && (parameterProperties_.flags & VstParameterFlags::kVstParameterUsesIntStep))
    {
        return parameterProperties_.maxInteger;
    }
    return 1.0;
}

double VST2PluginParameter::defaultValue() const
{
    return 0;
}

double VST2PluginParameter::value() const
{
    return effect_->getParameter(effect_, index_);
}

void VST2PluginParameter::setValue(double value)
{
    return effect_->setParameter(effect_, index_, value);
}

double VST2PluginParameter::step() const
{
    if(parameterPropertiesValid_)
    {
        if(parameterProperties_.flags & VstParameterFlags::kVstParameterUsesFloatStep)
        {
            return parameterProperties_.stepFloat;
        }
        if(parameterProperties_.flags & VstParameterFlags::kVstParameterUsesIntStep)
        {
            return parameterProperties_.stepInteger;
        }
    }
    return 0.0;
}
}