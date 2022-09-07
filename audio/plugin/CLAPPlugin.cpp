#include "CLAPPlugin.hpp"

#include "audio/host/CLAPEvents.hpp"
#include "audio/host/CLAPHost.hpp"
#include "audio/plugin/CLAPPluginParameter.hpp"
#include "controller/AudioEngineController.hpp"
#include "native/Native.hpp"

namespace Musec::Audio::Plugin
{
CLAPPlugin::CLAPPlugin():
    Musec::Native::WindowsLibraryRAII()
{
    //
}

CLAPPlugin::CLAPPlugin(const QString& path):
    Musec::Native::WindowsLibraryRAII(path)
{
    entry_ = Musec::Native::getExport<const clap_plugin_entry*>(*this, "clap_entry");
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
}

bool CLAPPlugin::createPlugin(int index)
{
    if(factory_)
    {
        // auto count = factory_->get_plugin_count(factory_);
        desc_ = factory_->get_plugin_descriptor(factory_, index);
        auto host = new(hostArea) Musec::Audio::Host::CLAPHost(*this);
        plugin_ = factory_->create_plugin(factory_, &(host->host()), desc_->id);
        pluginStatus_ = CLAPPluginStatus::Created;
        return plugin_;
    }
    return false;
}

CLAPPlugin::CLAPPlugin(const QString& path, int index):
    CLAPPlugin(path)
{
    createPlugin(index);
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

std::uint8_t CLAPPlugin::inputCount() const
{
    auto features = desc_->features;
    for(int j = 0; features[j]; ++j)
    {
        auto feature = features[j];
        if(std::strcmp(feature, CLAP_PLUGIN_FEATURE_INSTRUMENT) == 0)
        {
            return 0;
        }
    }
    return 2;
}

std::uint8_t CLAPPlugin::outputCount() const
{
    return 2;
}

void CLAPPlugin::process(Musec::Audio::Base::AudioBufferView<SampleType>* inputs, int inputBufferCount,
    Musec::Audio::Base::AudioBufferView<SampleType>* outputs, int outputBufferCount)
{
    // const auto& eventTransport = Musec::Audio::Host::AppCLAPEventTransport();
    // eventInputList_.push(&(eventTransport.header));
    processData_.steady_time = Musec::Native::currentTimeInNanosecond();
    processData_.frames_count = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    rawInputs_.resize(this->inputCount());
    rawOutputs_.resize(this->outputCount());
    processData_.audio_inputs_count = this->inputCount()? 1: 0;
    processData_.audio_outputs_count = 1;
    for(int i = 0; i < inputBufferCount; ++i)
    {
        rawInputs_[i] = inputs[i].getSamples();
    }
    for(int i = 0; i < outputBufferCount; ++i)
    {
        rawOutputs_[i] = outputs[i].getSamples();
    }
    processDataInput_.channel_count = inputCount();
    processDataOutput_.channel_count = outputCount();
    processDataInput_.data32 = rawInputs_.data();
    processDataOutput_.data32 = rawOutputs_.data();
    processData_.audio_inputs = &processDataInput_;
    processData_.audio_outputs = &processDataOutput_;
    plugin_->process(plugin_, &processData_);
}

const clap_plugin* CLAPPlugin::plugin() const
{
    return plugin_;
}

const clap_plugin_factory* CLAPPlugin::factory() const
{
    return factory_;
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
    params_ = reinterpret_cast<const clap_plugin_params*>(plugin_->get_extension(plugin_, CLAP_EXT_PARAMS));
    if(params_)
    {
        auto paramCount = parameterCount();
        paramBlock_ = {sizeof(CLAPPluginParameter) * paramCount};
        auto paramBlockAsArray = reinterpret_cast<CLAPPluginParameter*>(paramBlock_.data());
        for(decltype(paramCount) i = 0; i < paramCount; ++i)
        {
            paramBlockAsArray[i] = CLAPPluginParameter(plugin_, params_, i);
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
    gui_ = reinterpret_cast<const clap_plugin_gui*>(plugin_->get_extension(plugin_, CLAP_EXT_GUI));
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
        audioPorts_ = reinterpret_cast<decltype(audioPorts_)>(plugin_->get_extension(plugin_, CLAP_EXT_AUDIO_PORTS));
        processDataInput_.latency = 0;
        processDataOutput_.latency = 0;
        if(plugin_->activate(plugin_, sampleRate_, minBlockSize_, maxBlockSize_))
        {
            pluginStatus_ = CLAPPluginStatus::Activated;
            processData_.transport = &Musec::Audio::Host::AppCLAPEventTransport();
            processData_.in_events = eventInputList_.clapInputEvents();
            processData_.out_events = eventOutputList_.clapOutputEvents();
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

QString CLAPPlugin::getName() const
{
    if(desc_)
    {
        return QString(desc_->name);
    }
    return QString();
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
        window->setTitle(getName());
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
    throw std::out_of_range("");
}

void CLAPPlugin::initHost()
{
    new(hostArea) Musec::Audio::Host::CLAPHost(*this);
}

bool CLAPPlugin::processing()
{
    return pluginStatus_ >= CLAPPluginStatus::Processing;
}

}
