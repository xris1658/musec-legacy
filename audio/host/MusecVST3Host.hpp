#ifndef MUSEC_AUDIO_HOST_MUSECVST3HOST
#define MUSEC_AUDIO_HOST_MUSECVST3HOST

#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/ivsthostapplication.h>

namespace Musec
{
namespace Audio
{
namespace Host
{
using namespace Steinberg;
using namespace Steinberg::Vst;

class MusecVST3Host: public IHostApplication
{
private:
    MusecVST3Host();
public:
    MusecVST3Host(const MusecVST3Host&) = delete;
    virtual ~MusecVST3Host();
public:
    static MusecVST3Host& instance();
public:
    DECLARE_FUNKNOWN_METHODS
public:
    tresult PLUGIN_API getName(String128 name) override;
    tresult PLUGIN_API createInstance(TUID cid, TUID _iid, void** obj) override;
};
}
}
}

#endif // MUSEC_AUDIO_HOST_MUSECVST3HOST
