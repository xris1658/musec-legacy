#ifndef MUSEC_AUDIO_HOST_VST3HOST
#define MUSEC_AUDIO_HOST_VST3HOST

#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/ivsthostapplication.h>
#include <pluginterfaces/vst/ivstprocesscontext.h>

namespace Musec
{
namespace Audio
{
namespace Host
{
using namespace Steinberg;
using namespace Steinberg::Vst;

class VST3Host: public IHostApplication
{
private:
    VST3Host() = default;
public:
    VST3Host(const VST3Host&) = delete;
    ~VST3Host() = default;
public:
    DECLARE_FUNKNOWN_METHODS
public:
    tresult PLUGIN_API getName(String128 name) override;
    tresult PLUGIN_API createInstance(TUID cid, TUID iid_, void** obj) override;
public:
    static VST3Host& instance();
    static ProcessContext& AppProcessContext();
};
}
}
}

#endif // MUSEC_AUDIO_HOST_VST3HOST
