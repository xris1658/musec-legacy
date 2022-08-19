#ifndef MUSEC_AUDIO_PLUGIN_CLAPPLUGIN
#define MUSEC_AUDIO_PLUGIN_CLAPPLUGIN

#include "audio/base/Automation.hpp"
#include "audio/host/CLAPHost.hpp"
#include "audio/plugin/IPlugin.hpp"
#include "base/FixedSizeMemoryBlock.hpp"
#include "native/WindowsLibraryRAII.hpp"

#include <clap/ext/audio-ports.h>
#include <clap/ext/gui.h>
#include <clap/ext/params.h>
#include <clap/entry.h>
#include <clap/plugin.h>
#include <clap/plugin-factory.h>
#include <clap/process.h>

#include <clap/helpers/event-list.hh>

#include <QString>

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
enum class CLAPPluginStatus: std::int8_t
{
    NoPlugin = 0x00,
    Factory = 0x01,
    Created = 0x02,
    Initialized = 0x04,
    Activated = 0x08,
    Processing = 0x10
};

class CLAPPlugin:
    public Musec::Native::WindowsLibraryRAII,
    public Musec::Audio::Plugin::IPlugin
{
    using SampleType = float;
public:
    CLAPPlugin();
    CLAPPlugin(const QString& path);
    CLAPPlugin(CLAPPlugin&& rhs) = delete;
    CLAPPlugin& operator=(CLAPPlugin&& rhs) = delete;
    bool createPlugin(int index);
    CLAPPlugin(const QString& path, int index);
    ~CLAPPlugin() override;
public:
    std::uint8_t inputCount() const override;
    std::uint8_t outputCount() const override;
    void process(Musec::Audio::Base::AudioBufferView<SampleType>* inputs, int inputCount,
        Musec::Audio::Base::AudioBufferView<SampleType>* outputs, int outputCount) override;
public:
    const clap_plugin* plugin() const;
    const clap_plugin_factory* factory() const;
    const clap_plugin_gui* pluginGUI() const;
    const clap_plugin_params* pluginParams() const;
public:
    bool initialize(double sampleRate, std::int32_t maxSampleCount) override;
    bool uninitialize() override;
    bool attachToWindow(QWindow* window) override;
    bool detachWithWindow() override;
    QWindow* window() override;
    bool activate() override;
    bool deactivate() override;
    bool activated() override;
    bool processing() override;
    bool startProcessing() override;
    bool stopProcessing() override;
    bool hasUI() override;
    bool getBypass() const override;
    QString getName() const override;
    Musec::Base::PluginFormat pluginFormat() override;
    int parameterCount() override;
    IParameter& parameter(int index) override;
private:
    bool initializeEditor();
    bool uninitializeEditor();
    void onWindowSizeChanged();
    void initHost();
private:
    char hostArea[sizeof(Musec::Audio::Host::CLAPHost)];
    const clap_plugin* plugin_ = nullptr;
    const clap_plugin_audio_ports* audioPorts_ = nullptr;
    const clap_plugin_entry* entry_ = nullptr;
    const clap_plugin_factory* factory_ = nullptr;
    const clap_plugin_descriptor* desc_ = nullptr;
    const clap_plugin_gui* gui_ = nullptr;
    const clap_plugin_params* params_ = nullptr;
    double sampleRate_;
    std::uint32_t minBlockSize_;
    std::uint32_t maxBlockSize_;
    clap_process processData_;
    QWindow* window_ = nullptr;
    clap_window clapWindow_;
    CLAPPluginStatus pluginStatus_ = CLAPPluginStatus::NoPlugin;
    clap_audio_buffer processDataInput_;
    clap_audio_buffer processDataOutput_;
    clap::helpers::EventList eventInputList_;
    clap::helpers::EventList eventOutputList_;
    std::vector<SampleType*> rawInputs_;
    std::vector<SampleType*> rawOutputs_;
    Musec::Base::FixedSizeMemoryBlock paramBlock_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_CLAPPLUGIN
