#ifndef MUSEC_AUDIO_PLUGIN_VST2PLUGINPARAMETER
#define MUSEC_AUDIO_PLUGIN_VST2PLUGINPARAMETER

#include "audio/plugin/IParameter.hpp"
#include "audio/plugin/VST2Plugin.hpp"

#include <pluginterfaces/vst2.x/aeffectx.h>

#include <memory>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
class VST2PluginParameter: public IParameter
{
public:
    VST2PluginParameter(Musec::Audio::Plugin::VST2Plugin& plugin, VstInt32 index);
    VST2PluginParameter(VST2PluginParameter&&) = default;
    VST2PluginParameter& operator=(VST2PluginParameter&&) = default;
public:
    QString name() const override;
    double minValue() const override;
    double maxValue() const override;
    double defaultValue() const override;
    double value() const override;
    void setValue(double value) override;
    double step() const override;
private:
    VstInt32 index_;
    AEffect* effect_;
    VstParameterProperties parameterProperties_;
    bool parameterPropertiesValid_ = true;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST2PLUGINPARAMETER
