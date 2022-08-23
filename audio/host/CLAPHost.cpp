#include "CLAPHost.hpp"

#include "audio/plugin/CLAPPlugin.hpp"
#include "base/Constants.hpp"

#include <clap/ext/gui.h>
#include <clap/ext/params.h>

#include <cstring>

namespace Musec::Audio::Host
{
CLAPHost::CLAPHost(Musec::Audio::Plugin::CLAPPlugin& plugin):
    plugin_(&plugin),
    host_
    {
        CLAP_VERSION,
        this, // Points to the `CLAPHost` that's being constructed
        Musec::Base::ProductName,
        Musec::Base::CompanyName,
        Musec::Base::Url,
        Musec::Base::Version,
        &hostGetExtension,
        &hostRequestRestart,
        &hostRequestProcess,
        &hostRequestCallback
    },
    gui_
    {
        &hostGUIResizeHintsChanged,
        &hostGUIRequestResize,
        &hostGUIRequestShow,
        &hostGUIRequestHide,
        &hostGUIClosed
    },
    params_
    {
        &hostParamsRescan,
        &hostParamsClear,
        &hostParamsRequestFlush
    }
{
    std::strcpy(magic_, MUSEC_CLAP_HOST_MAGIC);
}

const clap_host& CLAPHost::host() noexcept
{
    return host_;
}

const clap_host_gui& CLAPHost::gui() noexcept
{
    return gui_;
}

const clap_host_params& CLAPHost::params() noexcept
{
    return params_;
}

CLAPHost* CLAPHost::checkHost(const clap_host* host)
{
    if(auto* ptr = reinterpret_cast<char*>(host->host_data))
    {
        if(std::strcmp(ptr, MUSEC_CLAP_HOST_MAGIC) == 0)
        {
            return reinterpret_cast<CLAPHost*>(ptr);
        }
    }
    return nullptr;
}

const void* CLAPHost::hostGetExtension(const clap_host* host, const char* extensionId)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        if(std::strcmp(extensionId, CLAP_EXT_GUI) == 0)
        {
            return &(hostClass->gui());
        }
        if(std::strcmp(extensionId, CLAP_EXT_PARAMS) == 0)
        {
            return &(hostClass->params());
        }
    }
    return nullptr;
}

void CLAPHost::hostRequestRestart(const clap_host* host)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        // Adds a schedule to deactivate -> activate the plugin
        // Is stopProcessing -> startProcessing needed?
    }
}

void CLAPHost::hostRequestProcess(const clap_host* host)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        // Adds a schedule to activate -> startProcessing the plugin
    }
}

void CLAPHost::hostRequestCallback(const clap_host* host)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        // Adds a schedule of the following code:
        // ```
        // auto clapPlugin = hostClass->plugin_->plugin();
        // clapPlugin->on_main_thread(clapPlugin);
        // ```
    }
}

void CLAPHost::hostGUIResizeHintsChanged(const clap_host* host)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        auto clapPlugin = hostClass->plugin_->plugin();
        auto clapPluginGUI = hostClass->plugin_->pluginGUI();
        clap_gui_resize_hints resizeHints;
        clapPluginGUI->get_resize_hints(clapPlugin, &resizeHints);
        // TODO: Use resizeHints
    }
}

bool CLAPHost::hostGUIRequestResize(const clap_host* host, uint32_t width, uint32_t height)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        auto window = hostClass->plugin_->window();
        window->setWidth(width);
        window->setHeight(height);
        return width == window->width() && height == window->height();
    }
    return false;
}

bool CLAPHost::hostGUIRequestShow(const clap_host* host)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        auto window = hostClass->plugin_->window();
        window->showNormal();
        return true;
    }
    return false;
}

bool CLAPHost::hostGUIRequestHide(const clap_host* host)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        auto window = hostClass->plugin_->window();
        window->hide();
        return true;
    }
    return false;
}

void CLAPHost::hostGUIClosed(const clap_host* host, bool wasDestroyed)
{
    if(wasDestroyed)
    {
        auto hostClass = checkHost(host);
        if(hostClass)
        {
            hostClass->plugin_->detachWithWindow();
        }
    }
}

void CLAPHost::hostParamsRescan(const clap_host* host, clap_param_rescan_flags flags)
{
    //
}

void CLAPHost::hostParamsClear(const clap_host* host, clap_id paramId, clap_param_clear_flags flags)
{
    //
}

void CLAPHost::hostParamsRequestFlush(const clap_host* host)
{
    //
}
}