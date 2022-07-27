#ifndef MUSEC_AUDIO_HOST_CLAPHOST
#define MUSEC_AUDIO_HOST_CLAPHOST

#define MUSEC_CLAP_HOST_MAGIC "Musec CLAP Host"

// #include "audio/plugin/CLAPPlugin.hpp"

#include <clap/host.h>
#include <clap/ext/gui.h>
#include <clap/ext/params.h>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
class CLAPPlugin;
}
namespace Host
{
class CLAPHost
{
public:
    CLAPHost(Musec::Audio::Plugin::CLAPPlugin& plugin);
    CLAPHost(const CLAPHost&) = delete;
    CLAPHost(CLAPHost&& rhs) = delete;
    ~CLAPHost() noexcept = default;
public:
    const clap_host& host() noexcept;
    const clap_host_gui& gui() noexcept;
    const clap_host_params& params() noexcept;
private:
    static CLAPHost* checkHost(const clap_host* host);
private:
    static const void* hostGetExtension(const clap_host* host, const char* extensionId);
    static void hostRequestRestart(const clap_host* host);
    static void hostRequestProcess(const clap_host* host);
    static void hostRequestCallback(const clap_host* host);
private:
    static void hostGUIResizeHintsChanged(const clap_host* host);
    static bool hostGUIRequestResize(const clap_host* host, uint32_t width, uint32_t height);
    static bool hostGUIRequestShow(const clap_host* host);
    static bool hostGUIRequestHide(const clap_host* host);
    static void hostGUIClosed(const clap_host* host, bool wasDestroyed);
private:
    static void hostParamsRescan(const clap_host* host, clap_param_rescan_flags flags);
    static void hostParamsClear(const clap_host* host, clap_id paramId, clap_param_clear_flags flags);
    static void hostParamsRequestFlush(const clap_host* host);
private:
    char magic_[16];
    Musec::Audio::Plugin::CLAPPlugin* plugin_ = nullptr;
    clap_host host_;
    clap_host_gui gui_;
    clap_host_params params_;
};
}
}
}

#endif //MUSEC_AUDIO_HOST_CLAPHOST
