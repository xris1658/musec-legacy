#ifndef MUSEC_AUDIO_PLUGIN_CLAPPLUGIN
#define MUSEC_AUDIO_PLUGIN_CLAPPLUGIN

#include "audio/plugin/IPlugin.hpp"
#include "native/WindowsLibraryRAII.hpp"

#include <clap/ext/audio-ports.h>
#include <clap/ext/gui.h>
#include <clap/ext/params.h>
#include <clap/entry.h>
#include <clap/plugin.h>
#include <clap/plugin-factory.h>
#include <clap/process.h>

#include <QString>

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

enum class CLAPPluginEditorStatus: std::int8_t
{
    NoEditor =0x00,
    Created = 0x01,
    Initialized = 0x02
};

template<typename SampleType>
class CLAPPlugin:
    public Musec::Native::WindowsLibraryRAII,
    public Musec::Audio::Plugin::IPlugin<SampleType>
{
public:
    CLAPPlugin();
    CLAPPlugin(const QString& path);
    CLAPPlugin(CLAPPlugin&&) = default;
    bool createPlugin(int index);
    static CLAPPlugin<SampleType> fromPathAndIndex(const QString& path, int index);
    ~CLAPPlugin() override;
public:
    std::uint8_t inputCount() const override;
    std::uint8_t outputCount() const override;
    void process(const Musec::Audio::Base::AudioBufferViews<SampleType>& inputs,
        const Musec::Audio::Base::AudioBufferViews<SampleType>& outputs) override;
public:
    const clap_plugin* plugin() const;
    const clap_plugin_factory* factory() const;
public:
    bool initialize(double sampleRate, std::int32_t maxSampleCount) override;
    bool uninitialize() override;
    bool initializeEditor();
    bool uninitializeEditor();
    bool attachToWindow(QWindow* window) override;
    bool detachWithWindow() override;
    QWindow* window() override;
    bool activate() override;
    bool deactivate() override;
    bool activated() override;
    bool startProcessing() override;
    bool stopProcessing() override;
    bool hasUI() override;
    bool getBypass() const override;
    QString getName() const override;
    Musec::Base::PluginFormat pluginFormat() override;
private:
    const clap_plugin* plugin_ = nullptr;
    const clap_plugin_audio_ports* audioPorts_ = nullptr;
    const clap_plugin_entry* entry_ = nullptr;
    const clap_plugin_factory* factory_ = nullptr;
    const clap_plugin_descriptor* desc_ = nullptr;
    double sampleRate_;
    std::uint32_t minBlockSize_;
    std::uint32_t maxBlockSize_;
    clap_process processData_;
    std::vector<clap_audio_buffer> bufferForProcessData_;
    const clap_plugin_gui* gui_ = nullptr;
    QWindow* window_ = nullptr;
    const clap_plugin_params* params_ = nullptr;
    clap_window clapWindow_;
    CLAPPluginStatus pluginStatus_ = CLAPPluginStatus::NoPlugin;
    CLAPPluginEditorStatus pluginEditorStatus_ = CLAPPluginEditorStatus::NoEditor;
};

extern template class CLAPPlugin<float>;
extern template class CLAPPlugin<double>;
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_CLAPPLUGIN
