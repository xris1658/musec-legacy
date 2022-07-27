#include "CLAPHost.hpp"

#include "audio/plugin/CLAPPlugin.hpp"
#include "base/Constants.hpp"

#include <clap/ext/gui.h>
#include <clap/ext/params.h>

#include <cstring>
#include <cassert>

namespace Musec::Audio::Host
{
CLAPHost::CLAPHost(Musec::Audio::Plugin::CLAPPlugin& plugin):
    plugin_(&plugin),
    host_
    {
        CLAP_VERSION,
        this, // 是否指向正在构造的 CLAPHost?
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
        // 计划将插件 deactivate -> activate
        // 是否需要 stopProcessing 和 startProcessing?
    }
}

void CLAPHost::hostRequestProcess(const clap_host* host)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        // 计划将插件 activate -> startProcessing
    }
}

void CLAPHost::hostRequestCallback(const clap_host* host)
{
    auto hostClass = checkHost(host);
    if(hostClass)
    {
        // // 计划执行以下代码:
        // auto clapPlugin = hostClass->plugin_->plugin();
        // clapPlugin->on_main_thread(clapPlugin);
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
        // TODO: 使用 resizeHints
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
    // 直接对 CLAPPlugin 对象进行操作无法将窗口的显示状态传递给 Musec::Entities::Plugin 对象，
    // 无法更新 UI 显示。因此这一函数目前什么都不做，只返回 false
    return false;
}

bool CLAPHost::hostGUIRequestHide(const clap_host* host)
{
    // 同上
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