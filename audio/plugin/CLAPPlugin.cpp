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
        // 抛出异常
    }
    auto pathAsStdString = path.toStdString();
    auto initResult = entry_->init(pathAsStdString.c_str());
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

CLAPPlugin::CLAPPlugin(CLAPPlugin&& rhs) noexcept
{
    std::swap(*this, rhs);
}

CLAPPlugin& CLAPPlugin::operator=(CLAPPlugin&& rhs) noexcept
{
    if(this != &rhs)
    {
        std::swap(*this, rhs);
    }
    return *this;
}

bool CLAPPlugin::createPlugin(int index)
{
    if(factory_)
    {
        auto count = factory_->get_plugin_count(factory_);
        desc_ = factory_->get_plugin_descriptor(factory_, index);
        auto host = new(hostArea) Musec::Audio::Host::CLAPHost(*this);
        plugin_ = factory_->create_plugin(factory_, &(host->host()), desc_->id);
        // plugin_ = factory_->create_plugin(factory_, nullptr, desc_->id);
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
        stopProcessing();
    }
    if(pluginStatus_ == CLAPPluginStatus::Activated)
    {
        deactivate();
    }
    if(pluginStatus_ == CLAPPluginStatus::Initialized)
    {
        uninitialize();
    }
    pluginStatus_ = CLAPPluginStatus::NoPlugin;
    if(entry_)
    {
        entry_->deinit();
        entry_ = nullptr;
    }
} // 析构 rawInputs_ 时访问了无效的内存。原因未知。

void CLAPPlugin::swap(CLAPPlugin& rhs)
{
    std::swap<Musec::Native::WindowsLibraryRAII>(*this, rhs);
    for(auto i = 0; i < sizeof(Musec::Audio::Host::CLAPHost); i += 8)
    {
        std::swap(reinterpret_cast<double&>(hostArea[i]), reinterpret_cast<double&>(rhs.hostArea[i]));
    }
    std::swap(hostArea, rhs.hostArea);
    std::swap(plugin_, rhs.plugin_);
    std::swap(audioPorts_, rhs.audioPorts_);
    std::swap(entry_, rhs.entry_);
    std::swap(factory_, rhs.factory_);
    std::swap(desc_, rhs.desc_);
    std::swap(gui_, rhs.gui_);
    std::swap(params_, rhs.params_);
    std::swap(sampleRate_, rhs.sampleRate_);
    std::swap(minBlockSize_, rhs.minBlockSize_);
    std::swap(maxBlockSize_, rhs.maxBlockSize_);
    std::swap(processData_, rhs.processData_);
    std::swap(window_,rhs.window_);
    std::swap(clapWindow_, rhs.clapWindow_);
    std::swap(pluginStatus_, rhs.pluginStatus_);
    std::swap(processDataInput_, rhs.processDataInput_);
    std::swap(processDataOutput_, rhs.processDataOutput_);
    std::swap_ranges(reinterpret_cast<char*>(&eventInputList_), reinterpret_cast<char*>(&eventInputList_) + sizeof(clap::helpers::EventList), reinterpret_cast<char*>(&rhs.eventInputList_));
    std::swap_ranges(reinterpret_cast<char*>(&eventOutputList_), reinterpret_cast<char*>(&eventOutputList_) + sizeof(clap::helpers::EventList), reinterpret_cast<char*>(&rhs.eventOutputList_));
    std::swap(rawInputs_, rhs.rawInputs_);
    std::swap(rawOutputs_, rhs.rawOutputs_);
    std::swap(paramBlock_, rhs.paramBlock_);
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
    // FIXME: 基于 JUCE 的插件的其处理函数会因为事件列表出问题而读取野指针
    // plugin_->process(plugin_, &processData_);
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
    // 为何 CLAP 要在初始化插件时添加最小缓冲区大小？
    minBlockSize_ = 1;
    maxBlockSize_ = maxSampleCount;
    if(plugin_)
    {
        if(plugin_->init(plugin_))
        {
            pluginStatus_ = CLAPPluginStatus::Initialized;
            initializeEditor();
            return true;
        }
    }
    return false;
}

bool CLAPPlugin::uninitialize()
{
    if(window_)
    {
        uninitializeEditor();
    }
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

bool CLAPPlugin::initializeEditor()
{
    params_ = reinterpret_cast<const clap_plugin_params*>(plugin_->get_extension(plugin_, CLAP_EXT_PARAMS));
    gui_ = reinterpret_cast<const clap_plugin_gui*>(plugin_->get_extension(plugin_, CLAP_EXT_GUI));
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

bool CLAPPlugin::uninitializeEditor()
{
    detachWithWindow();
    paramBlock_ = {0};
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
        gui_->set_scale(plugin_,1.0);
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
        gui_->show(plugin_); // 某些插件实现永远返回 false，因此不检查值
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
        gui_->hide(plugin_);
        Musec::Controller::AudioEngineController::AppProject().removePluginWindowMapping(this);
        gui_->destroy(plugin_);
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

}

namespace std
{
template<> void swap(Musec::Audio::Plugin::CLAPPlugin& lhs, Musec::Audio::Plugin::CLAPPlugin& rhs)
noexcept(std::is_move_constructible_v<Musec::Audio::Plugin::CLAPPlugin>
         && std::is_move_assignable_v<Musec::Audio::Plugin::CLAPPlugin>)
{
    lhs.swap(rhs);
}
}
