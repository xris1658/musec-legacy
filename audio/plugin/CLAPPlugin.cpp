#include "CLAPPlugin.hpp"

#include "audio/host/CLAPHost.hpp"
#include "controller/AudioEngineController.hpp"
#include "native/Native.hpp"

namespace Musec::Audio::Plugin
{
template<typename SampleType>
CLAPPlugin<SampleType>::CLAPPlugin()
{

}

template<typename SampleType>
CLAPPlugin<SampleType>::CLAPPlugin(const QString& path):
    Musec::Native::WindowsLibraryRAII(path)
{
    entry_ = Musec::Native::getExport<const clap_plugin_entry*>(*this, "clap_entry");
    if(!entry_)
    {
        // 抛出异常
    }
    auto pathAsStdString = path.toStdString();
    entry_->init(pathAsStdString.c_str());
    factory_ = reinterpret_cast<const clap_plugin_factory*>(entry_->get_factory(CLAP_PLUGIN_FACTORY_ID));
    if(factory_)
    {
        pluginStatus_ = CLAPPluginStatus::Factory;
    }
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::createPlugin(int index)
{
    if(factory_)
    {
        auto count = factory_->get_plugin_count(factory_);
        desc_ = factory_->get_plugin_descriptor(factory_, index);
        plugin_ = factory_->create_plugin(factory_, &(Musec::Audio::Host::AppCLAPHost()), desc_->id);
        pluginStatus_ = CLAPPluginStatus::Created;
        return plugin_;
    }
    return false;
}

template<typename SampleType>
CLAPPlugin<SampleType> CLAPPlugin<SampleType>::fromPathAndIndex(const QString& path, int index)
{
    auto ret = CLAPPlugin<SampleType>(path);
    ret.createPlugin(index);
    return ret;
}

template<typename SampleType>
CLAPPlugin<SampleType>::~CLAPPlugin()
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
    }
}

template<typename SampleType>
std::uint8_t CLAPPlugin<SampleType>::inputCount() const
{
    std::uint8_t ret = 0;
    clap_audio_port_info portInfo;
    auto inputBusCount = audioPorts_->count(plugin_, true);
    for(decltype(inputBusCount) i = 0; i < inputBusCount; ++i)
    {
        audioPorts_->get(plugin_, i, true, &portInfo);
        ret += portInfo.channel_count;
    }
    return ret;
}

template<typename SampleType>
std::uint8_t CLAPPlugin<SampleType>::outputCount() const
{
    std::uint8_t ret = 0;
    clap_audio_port_info portInfo;
    auto inputBusCount = audioPorts_->count(plugin_, false);
    for(decltype(inputBusCount) i = 0; i < inputBusCount; ++i)
    {
        audioPorts_->get(plugin_, i, false, &portInfo);
        ret += portInfo.channel_count;
    }
    return ret;
}

template<typename SampleType>
void CLAPPlugin<SampleType>::process(const Musec::Audio::Base::AudioBufferViews<SampleType>& inputs,
    const Musec::Audio::Base::AudioBufferViews<SampleType>& outputs)
{
    processData_.steady_time = Musec::Native::currentTimeInNanosecond();
    processData_.frames_count = Musec::Controller::AudioEngineController::getCurrentBlockSize();
    processData_.transport = nullptr;
    // TODO
}

template<typename SampleType>
const clap_plugin* CLAPPlugin<SampleType>::plugin() const
{
    return plugin_;
}

template<typename SampleType>
const clap_plugin_factory* CLAPPlugin<SampleType>::factory() const
{
    return factory_;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::initialize(double sampleRate, std::int32_t maxSampleCount)
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

template<typename SampleType>
bool CLAPPlugin<SampleType>::uninitialize()
{
    if(window_)
    {
        uninitializeEditor();
    }
    if(plugin_)
    {
        plugin_->destroy(plugin_);
        pluginStatus_ = CLAPPluginStatus::Created;
    }
    return true;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::initializeEditor()
{
    params_ = reinterpret_cast<const clap_plugin_params*>(plugin_->get_extension(plugin_, CLAP_EXT_PARAMS));
    gui_ = reinterpret_cast<const clap_plugin_gui*>(plugin_->get_extension(plugin_, CLAP_EXT_GUI));
    return params_;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::uninitializeEditor()
{
    detachWithWindow();
    return true;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::activate()
{
    if(plugin_)
    {
        audioPorts_ = reinterpret_cast<decltype(audioPorts_)>(plugin_->get_extension(plugin_, CLAP_EXT_AUDIO_PORTS));
        if(plugin_->activate(plugin_, sampleRate_, minBlockSize_, maxBlockSize_))
        {
            pluginStatus_ = CLAPPluginStatus::Activated;
            return true;
        }
    }
    return false;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::deactivate()
{
    if(plugin_)
    {
        plugin_->deactivate(plugin_);
        pluginStatus_ = CLAPPluginStatus::Initialized;
    }
    return true;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::startProcessing()
{
    if(plugin_ && plugin_->start_processing(plugin_))
    {
        pluginStatus_ = CLAPPluginStatus::Processing;
    }
    return pluginStatus_ == CLAPPluginStatus::Processing;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::stopProcessing()
{
    if(plugin_)
    {
        plugin_->stop_processing(plugin_);
        pluginStatus_ = CLAPPluginStatus::Activated;
    }
    return true;
}

template<typename SampleType>
Musec::Base::PluginFormat CLAPPlugin<SampleType>::pluginFormat()
{
    return Musec::Base::FormatCLAP;
}

template<typename SampleType>
QString CLAPPlugin<SampleType>::getName() const
{
    if(desc_)
    {
        return QString(desc_->name);
    }
    return QString();
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::attachToWindow(QWindow* window)
{
    auto supported = gui_->is_api_supported(plugin_, CLAP_WINDOW_API_WIN32, false);
    if(supported)
    {
        gui_->create(plugin_, CLAP_WINDOW_API_WIN32, false);
        gui_->set_scale(plugin_,1.0);
        std::uint32_t initialSize[2];
        auto getSizeResult = gui_->get_size(plugin_, initialSize, initialSize + 1);
        if(getSizeResult)
        {
            window->setWidth(initialSize[0]);
            window->setHeight(initialSize[1]);
        }
        clapWindow_.api = CLAP_WINDOW_API_WIN32;
        clapWindow_.win32 = reinterpret_cast<clap_hwnd>(window->winId());
        gui_->set_parent(plugin_, &clapWindow_);
        window->setTitle(getName());
        Musec::Controller::AudioEngineController::AppProject().addPluginWindowMapping(this, window);
        window_ = window;
        return gui_->show(plugin_);
    }
    return false;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::detachWithWindow()
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
        window_->close();
        window_ = nullptr;
        return true;
    }
    return false;
}

template<typename SampleType>
QWindow* CLAPPlugin<SampleType>::window()
{
    return window_;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::hasUI()
{
    return gui_;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::activated()
{
    return pluginStatus_ >= CLAPPluginStatus::Activated;
}

template<typename SampleType>
bool CLAPPlugin<SampleType>::getBypass() const
{
    return pluginStatus_ <= CLAPPluginStatus::Processing;
}

template class CLAPPlugin<float>;
template class CLAPPlugin<double>;

}