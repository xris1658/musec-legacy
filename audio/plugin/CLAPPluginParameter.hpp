#ifndef MUSEC_AUDIO_PLUGIN_CLAPPLUGINPARAMETER
#define MUSEC_AUDIO_PLUGIN_CLAPPLUGINPARAMETER

#include "audio/base/Automation.hpp"
#include "audio/plugin/IParameter.hpp"

#include <clap/plugin.h>
#include <clap/ext/params.h>
#include <clap/helpers/param-queue.hh>

#include <utility>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using CLAPParamQueue = clap::helpers::ParamQueue<std::pair<Musec::Audio::Base::Automation::TimeType, double>, 4096>;

class CLAPPluginParameter: public IParameter
{
public:
    CLAPPluginParameter(const clap_plugin* plugin, const clap_plugin_params* params, uint32_t index);
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
private:
    const clap_plugin* plugin_;
    const clap_plugin_params* params_;
    uint32_t index_;
    clap_param_info paramInfo_;
};

void automationToParamList(const CLAPPluginParameter& parameter,
    const Musec::Audio::Base::Automation& automation,
    Musec::Audio::Base::Automation::TimeType from,
    Musec::Audio::Base::Automation::TimeType to,
    Musec::Audio::Base::Automation::TimeType precision,
    CLAPParamQueue& paramList
);
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_CLAPPLUGINPARAMETER
