#ifndef MUSEC_AUDIO_PLUGIN_IPLUGIN
#define MUSEC_AUDIO_PLUGIN_IPLUGIN

#include "audio/device/IDevice.hpp"
#include "audio/plugin/IParameter.hpp"
#include "base/PluginBase.hpp"
#include "native/CompilerSpecific.hpp"

#include <QString>
#include <QWindow>

#include <array>
#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
MUSEC_INTERFACE IPlugin: public Musec::Audio::Device::IDevice
{
public:
    virtual bool initialize(double sampleRate, std::int32_t maxSampleCount) = 0;
    virtual bool uninitialize() = 0;
    virtual bool attachToWindow(QWindow* window) = 0;
    virtual bool detachWithWindow() = 0;
    virtual QWindow* window() = 0;
    virtual bool activate() = 0;
    virtual bool deactivate() = 0;
    virtual bool activated() = 0;
    virtual bool processing() = 0;
    virtual bool startProcessing() = 0;
    virtual bool stopProcessing() = 0;
    virtual bool hasUI() = 0;
    bool setProcessing(bool processing)
    {
        return processing? startProcessing(): stopProcessing();
    }
    bool setBypass(bool bypass)
    {
        return setProcessing(!bypass);
    }
    virtual bool getBypass() const = 0;
    virtual QString getName() const = 0;
    virtual Musec::Base::PluginFormat pluginFormat() = 0;
    virtual int parameterCount() = 0;
    virtual IParameter& parameter(int index) = 0;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_IPLUGIN
