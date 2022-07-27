#ifndef MUSEC_AUDIO_PLUGIN_IPARAMETER
#define MUSEC_AUDIO_PLUGIN_IPARAMETER

#include <QString>

#include <cstdint>
#include <type_traits>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
enum ParameterSupportFlags: std::int32_t
{
    SupportDiscrete     = 1 << 0,
    SupportPeriodic     = 1 << 1,
    SupportHidden       = 1 << 2,
    SupportReadonly     = 1 << 3,
    SupportAutomatable  = 1 << 4,
    SupportMinMaxValue  = 1 << 5,
    SupportDefaultValue = 1 << 6
};

class IParameter
{
public:
    virtual QString name() const = 0;
    virtual double minValue() const = 0;
    virtual double maxValue() const = 0;
    virtual double defaultValue() const = 0;
    virtual double value() const = 0;
    virtual void setValue(double value) = 0;
    virtual double step() const = 0;
public:
    std::int32_t flags()
    {
        return flags_;
    }
protected:
    std::int32_t flags_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_IPARAMETER
