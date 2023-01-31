#ifndef MUSEC_AUDIO_PLUGIN_VST3STREAM
#define MUSEC_AUDIO_PLUGIN_VST3STREAM

#include <pluginterfaces/base/ibstream.h>

#include <deque>
#include <sstream>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
class VST3Stream: public Steinberg::IBStream
{
public:
    VST3Stream() = default;
    ~VST3Stream() = default;
public:
    Steinberg::tresult queryInterface(const char* _iid, void** obj) override;
    Steinberg::uint32 addRef() override;
    Steinberg::uint32 release() override;
public:
    Steinberg::tresult read(void* buffer, Steinberg::int32 numBytes, Steinberg::int32* numBytesRead) override;
    Steinberg::tresult write(void* buffer, Steinberg::int32 numBytes, Steinberg::int32* numBytesWritten) override;
    Steinberg::tresult seek(Steinberg::int64 pos, Steinberg::int32 mode, Steinberg::int64* result) override;
    Steinberg::tresult tell(Steinberg::int64* pos) override;
private:
    std::stringstream sstr_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3STREAM
