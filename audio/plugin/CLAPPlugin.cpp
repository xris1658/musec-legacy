#include "CLAPPlugin.hpp"

#include "audio/host/CLAPEvents.hpp"
#include "audio/host/CLAPHost.hpp"
#include "audio/plugin/CLAPPluginParameter.hpp"
#include "controller/AudioEngineController.hpp"
#include "native/Native.hpp"

namespace Musec::Audio::Plugin
{
namespace Impl
{
template<typename T>
const T* getExtension(const clap_plugin* plugin, const char* id)
{
    return reinterpret_cast<const T*>(plugin->get_extension(plugin, id));
}
}

CLAPPlugin::CLAPPlugin(const QString& path):
    Musec::Native::Library(path)
{
    entry_ = Musec::Native::Library::getExport<const clap_plugin_entry*>("clap_entry");
    if(!entry_)
    {
        throw std::runtime_error("Error: CLAP plugin entry not found! This might be not a CLAP plugin.");
    }
    auto pathAsUtf8 = path.toUtf8();
    auto initResult = entry_->init(pathAsUtf8.data());
    if(!initResult)
    {
        entry_ = nullptr;
        throw std::runtime_error("Error: CLAP plugin entry initialization failed!");
    }
    factory_ = reinterpret_cast<const clap_plugin_factory*>(entry_->get_factory(CLAP_PLUGIN_FACTORY_ID));
    if(factory_)
    {
        pluginStatus_ = CLAPPluginStatus::Factory;
    }
    invalidationFactory_ = reinterpret_cast<const clap_plugin_invalidation_factory*>(
        entry_->get_factory(CLAP_PLUGIN_INVALIDATION_FACTORY_ID));
}

bool CLAPPlugin::createPlugin(const char* id)
{
    if(factory_)
    {
        auto host = new(hostArea) Musec::Audio::Host::CLAPHost(*this);
        plugin_ = factory_->create_plugin(factory_, &(host->host()), id);
        pluginStatus_ = CLAPPluginStatus::Created;
        return plugin_;
    }
    return false;
}

CLAPPlugin::CLAPPlugin(const QString& path, const char* id):
    CLAPPlugin(path)
{
    createPlugin(id);
}

CLAPPlugin::~CLAPPlugin()
{
    if(pluginStatus_ == CLAPPluginStatus::Processing)
    {
        CLAPPlugin::stopProcessing();
    }
    if(pluginStatus_ == CLAPPluginStatus::Activated)
    {
        CLAPPlugin::deactivate();
    }
    if(pluginStatus_ == CLAPPluginStatus::Initialized)
    {
        CLAPPlugin::uninitialize();
    }
    pluginStatus_ = CLAPPluginStatus::NoPlugin;
    if(entry_)
    {
        entry_->deinit();
        entry_ = nullptr;
    }
}

std::uint8_t CLAPPlugin::audioInputCount() const
{
    auto count = audioPorts_->count(plugin_, true);
    clap_audio_port_info audioPortInfo;
    std::uint32_t ret = 0;
    for(decltype(count) i = 0; i < count; ++i)
    {
        audioPorts_->get(plugin_, i, true, &audioPortInfo);
        ret += audioPortInfo.channel_count;
    }
    return ret;
}

std::uint8_t CLAPPlugin::audioOutputCount() const
{
    auto count = audioPorts_->count(plugin_, false);
    clap_audio_port_info audioPortInfo;
    std::uint32_t ret = 0;
    for(decltype(count) i = 0; i < count; ++i)
    {
        audioPorts_->get(plugin_, i, false, &audioPortInfo);
        ret += audioPortInfo.channel_count;
    }
    return ret;
}

void CLAPPlugin::process(Musec::Audio::Base::AudioBufferView<SampleType>* inputs, int inputBufferCount,
    Musec::Audio::Base::AudioBufferView<SampleType>* outputs, int outputBufferCount)
{
    // const auto& eventTransport = Musec::Audio::Host::AppCLAPEventTransport();
    // eventInputList_.push(&(eventTransport.header));
    processData_.steady_time = Musec::Native::currentTimeValueInNanosecond();
    processData_.frames_count = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    rawInputs_.resize(this->audioInputCount());
    rawOutputs_.resize(this->audioOutputCount());
    processData_.audio_inputs_count = this->audioInputCount()? 1: 0;
    processData_.audio_outputs_count = 1;
    for(int i = 0; i < inputBufferCount; ++i)
    {
        rawInputs_[i] = inputs[i].getSamples();
    }
    for(int i = 0; i < outputBufferCount; ++i)
    {
        rawOutputs_[i] = outputs[i].getSamples();
    }
    processDataInput_.channel_count = audioInputCount();
    processDataOutput_.channel_count = audioOutputCount();
    processDataInput_.data32 = rawInputs_.data();
    processDataOutput_.data32 = rawOutputs_.data();
    processData_.audio_inputs = &processDataInput_;
    processData_.audio_outputs = &processDataOutput_;
    plugin_->process(plugin_, &processData_);
}

void CLAPPlugin::process(const Musec::Audio::Device::AudioProcessData<SampleType>& audioProcessData)
{
    processData_.frames_count = audioProcessData.singleBufferSize;
    for(int i = 0; i < processData_.audio_inputs_count; ++i)
    {
        audioInputBuffers_[i].data32 = audioProcessData.inputs[i];
    }
    for(int i = 0; i < processData_.audio_outputs_count; ++i)
    {
        audioOutputBuffers_[i].data32 = audioProcessData.outputs[i];
    }
}

const clap_plugin* CLAPPlugin::plugin() const
{
    return plugin_;
}

const clap_plugin_factory* CLAPPlugin::factory() const
{
    return factory_;
}

const clap_plugin_invalidation_factory* CLAPPlugin::invalidationFactory() const
{
    return invalidationFactory_;
}

const clap_plugin_gui* CLAPPlugin::pluginGUI() const
{
    return gui_;
}

const clap_plugin_params* CLAPPlugin::pluginParams() const
{
    return params_;
}

bool CLAPPlugin::initialize(double sampleRate, std::int32_t maxSampleCount)
{
    sampleRate_ = sampleRate;
    // Why is the maximum buffer size is needed while initializing the CLAP plugin?
    minBlockSize_ = 1;
    maxBlockSize_ = maxSampleCount;
    if(plugin_)
    {
        if(plugin_->init(plugin_))
        {
            pluginStatus_ = CLAPPluginStatus::Initialized;
            audioPorts_ = Impl::getExtension<clap_plugin_audio_ports>(plugin_, CLAP_EXT_AUDIO_PORTS);
            if(!audioPorts_)
            {
                return false;
            }
            notePorts_ = Impl::getExtension<clap_plugin_note_ports>(plugin_, CLAP_EXT_NOTE_PORTS);
            latency_ = Impl::getExtension<clap_plugin_latency>(plugin_, CLAP_EXT_LATENCY);
            inputSpeakerGroupCollection_ = {plugin_, audioPorts_, true};
            outputSpeakerGroupCollection_ = {plugin_, audioPorts_, false};
            if(initializeParameters())
            {
                initializeUI();
                return true;
            }
        }
    }
    return false;
}

bool CLAPPlugin::uninitialize()
{
    if(gui_)
    {
        uninitializeUI();
    }
    inputSpeakerGroupCollection_ = {};
    outputSpeakerGroupCollection_ = {};
    uninitializeParameters();
    if(plugin_)
    {
        plugin_->destroy(plugin_);
        pluginStatus_ = CLAPPluginStatus::Factory;
    }
    using Musec::Audio::Host::CLAPHost;
    auto ptr = reinterpret_cast<CLAPHost*>(hostArea);
    ptr->~CLAPHost();
    return true;
}

bool CLAPPlugin::initializeParameters()
{
    params_ = Impl::getExtension<clap_plugin_params>(plugin_, CLAP_EXT_PARAMS);
    if(params_)
    {
        auto paramCount = parameterCount();
        paramBlock_ = Musec::Base::FixedSizeMemoryBlock(sizeof(CLAPPluginParameter) * paramCount);
        auto paramBlockAsArray = reinterpret_cast<CLAPPluginParameter*>(paramBlock_.data());
        for(decltype(paramCount) i = 0; i < paramCount; ++i)
        {
            new(paramBlockAsArray + i) CLAPPluginParameter(plugin_, params_, i);
        }
    }
    return params_;
}

bool CLAPPlugin::uninitializeParameters()
{
    detachWithWindow();
    paramBlock_ = {0};
    return true;
}

bool CLAPPlugin::initializeUI()
{
    gui_ = Impl::getExtension<clap_plugin_gui>(plugin_, CLAP_EXT_GUI);
    return gui_;
}

bool CLAPPlugin::uninitializeUI()
{
    detachWithWindow();
    return true;
}

bool CLAPPlugin::activate()
{
    if(plugin_)
    {
        processDataInput_.latency = 0;
        processDataOutput_.latency = 0;
        if(plugin_->activate(plugin_, sampleRate_, minBlockSize_, maxBlockSize_))
        {
            pluginStatus_ = CLAPPluginStatus::Activated;
            processData_.transport = &Musec::Audio::Host::AppCLAPEventTransport();
            processData_.in_events = eventInputList_.clapInputEvents();
            processData_.out_events = eventOutputList_.clapOutputEvents();
            audioInputBuffers_.resize(audioInputSpeakerGroupCollection().channelGroupCount());
            audioOutputBuffers_.resize(audioOutputSpeakerGroupCollection().channelGroupCount());
            return true;
        }
    }
    return false;
}

bool CLAPPlugin::deactivate()
{
    if(plugin_)
    {
        plugin_->deactivate(plugin_);
        pluginStatus_ = CLAPPluginStatus::Initialized;
    }
    return true;
}

bool CLAPPlugin::startProcessing()
{
    if(plugin_ && plugin_->start_processing(plugin_))
    {
        pluginStatus_ = CLAPPluginStatus::Processing;
        for(int i = 0; i < audioInputBuffers_.size(); ++i)
        {
            audioInputBuffers_[i].channel_count = audioOutputSpeakerGroupCollection().channelGroupAt(i).channelCount();
            audioInputBuffers_[i].latency = 0;
        }
        for(int i = 0; i < audioOutputBuffers_.size(); ++i)
        {
            audioOutputBuffers_[i].channel_count = audioOutputSpeakerGroupCollection().channelGroupAt(i).channelCount();
            audioOutputBuffers_[i].latency = 0; // FIXME
        }
        processData_.audio_inputs_count = audioInputSpeakerGroupCollection().channelGroupCount();
        processData_.audio_outputs_count = audioOutputSpeakerGroupCollection().channelGroupCount();
        processData_.audio_inputs = audioInputBuffers_.data();
        processData_.audio_outputs = audioOutputBuffers_.data();
    }
    return pluginStatus_ == CLAPPluginStatus::Processing;
}

bool CLAPPlugin::stopProcessing()
{
    if(plugin_)
    {
        plugin_->stop_processing(plugin_);
        pluginStatus_ = CLAPPluginStatus::Activated;
    }
    return true;
}

Musec::Base::PluginFormat CLAPPlugin::pluginFormat()
{
    return Musec::Base::FormatCLAP;
}

bool CLAPPlugin::attachToWindow(QWindow* window)
{
    auto supported = gui_->is_api_supported(plugin_, CLAP_WINDOW_API_WIN32, false);
    if(supported)
    {
        gui_->create(plugin_, CLAP_WINDOW_API_WIN32, false);
        gui_->set_scale(plugin_, 1.0);
        std::uint32_t width, height;
        auto getSizeResult = gui_->get_size(plugin_, &width, &height);
        if(getSizeResult)
        {
            window->setWidth(width);
            window->setHeight(height);
        }
        clapWindow_.api = CLAP_WINDOW_API_WIN32;
        clapWindow_.win32 = reinterpret_cast<clap_hwnd>(window->winId());
        gui_->set_parent(plugin_, &clapWindow_);
        Musec::Controller::AudioEngineController::AppProject().addPluginWindowMapping(this, window);
        window_ = window;
        // For some reason, some plugins will always return `false` even if the GUI is present
        gui_->show(plugin_);
        QObject::connect(window_, &QWindow::widthChanged,
            [this](int) { onWindowSizeChanged(); });
        QObject::connect(window_, &QWindow::heightChanged,
            [this](int) { onWindowSizeChanged(); });
        return true;
    }
    return false;
}

bool CLAPPlugin::detachWithWindow()
{
    if(!window_)
    {
        return true;
    }
    if(gui_)
    {
        gui_->destroy(plugin_);
        Musec::Controller::AudioEngineController::AppProject().removePluginWindowMapping(this);
        window_ = nullptr;
        return true;
    }
    return false;
}

QWindow* CLAPPlugin::window()
{
    return window_;
}

void CLAPPlugin::onWindowSizeChanged()
{
    if(gui_)
    {
        uint32_t width = window_->width();
        uint32_t height = window_->height();
        if(gui_->can_resize(plugin_))
        {
            gui_->adjust_size(plugin_, &width, &height);
            gui_->set_size(plugin_, width, height);
            window_->setWidth(width);
            window_->setHeight(height);
        }
        else
        {
            gui_->get_size(plugin_, &width, &height);
            window_->setWidth(width);
            window_->setHeight(height);
        }
    }
}

bool CLAPPlugin::hasUI()
{
    return gui_;
}

bool CLAPPlugin::activated()
{
    return pluginStatus_ >= CLAPPluginStatus::Activated;
}

bool CLAPPlugin::getBypass() const
{
    return pluginStatus_ < CLAPPluginStatus::Processing;
}

int CLAPPlugin::parameterCount()
{
    return params_->count(plugin_);
}

IParameter& CLAPPlugin::parameter(int index)
{
    return reinterpret_cast<CLAPPluginParameter*>(paramBlock_.data())[index];
}

int CLAPPlugin::latency()
{
    return latency_? latency_->get(plugin_): 0;
}

void CLAPPlugin::initHost()
{
    new(hostArea) Musec::Audio::Host::CLAPHost(*this);
}

bool CLAPPlugin::processing()
{
    return pluginStatus_ >= CLAPPluginStatus::Processing;
}

const IChannelGroupCollection& CLAPPlugin::audioInputSpeakerGroupCollection() const
{
    return inputSpeakerGroupCollection_;
}

const IChannelGroupCollection& CLAPPlugin::audioOutputSpeakerGroupCollection() const
{
    return outputSpeakerGroupCollection_;
}

}
