#include "VST3Stream.hpp"

#include <algorithm>

namespace Musec::Audio::Plugin
{
using namespace Steinberg;

Steinberg::tresult VST3Stream::queryInterface(const char* _iid, void** obj)
{
    QUERY_INTERFACE(_iid, obj, FUnknown::iid, IBStream)
    QUERY_INTERFACE(_iid, obj, IBStream::iid, IBStream)
    *obj = nullptr;
    return Steinberg::kNoInterface;
}

Steinberg::uint32 VST3Stream::addRef()
{
    return 1;
}

Steinberg::uint32 VST3Stream::release()
{
    return 1;
}

tresult VST3Stream::read(void* buffer, int32 numBytes, int32* numBytesRead)
{
    auto dest = reinterpret_cast<char*>(buffer);
    auto bytesRead = sstr_.readsome(dest, numBytes);
    if(numBytesRead)
    {
        *numBytesRead = bytesRead;
    }
    return Steinberg::kResultOk;
}

tresult VST3Stream::write(void* buffer, int32 numBytes, int32* numBytesWritten)
{
    auto src = reinterpret_cast<char*>(buffer);
    auto start = sstr_.tellp();
    sstr_.write(src, numBytes);
    auto flags = sstr_.flags();
    if(flags & std::ios_base::badbit)
    {
        sstr_.clear();
        if(numBytesWritten)
        {
            *numBytesWritten = sstr_.tellp() - start;
        }
        return Steinberg::kResultOk;
    }
    if(numBytesWritten)
    {
        *numBytesWritten = numBytes;
    }
    return Steinberg::kResultOk;
}

tresult VST3Stream::seek(int64 pos, int32 mode, int64* result)
{
    std::ios_base::seekdir seekDir = 0;
    switch(mode)
    {
    case Steinberg::IBStream::IStreamSeekMode::kIBSeekSet:
        seekDir = std::ios_base::beg;
        break;
    case Steinberg::IBStream::IStreamSeekMode::kIBSeekCur:
        seekDir = std::ios_base::cur;
        break;
    case Steinberg::IBStream::IStreamSeekMode::kIBSeekEnd:
        seekDir = std::ios_base::end;
        break;
    default:
        return Steinberg::kInvalidArgument;
    }
    auto oldPos = sstr_.tellg();
    sstr_.seekg(pos, seekDir);
    auto flags = sstr_.flags();
    if(flags & std::ios_base::failbit)
    {
        sstr_.clear();
        sstr_.seekg(oldPos, std::ios_base::beg);
        return Steinberg::kResultFalse;
    }
    sstr_.seekp(pos, seekDir);
    flags = sstr_.flags();
    if(flags & std::ios_base::failbit)
    {
        sstr_.clear();
        sstr_.seekg(oldPos, std::ios_base::beg);
        sstr_.seekp(oldPos, std::ios_base::beg);
        return Steinberg::kResultFalse;
    }
    if(result)
    {
        *result = sstr_.tellp();
    }
    return Steinberg::kResultOk;
}

tresult VST3Stream::tell(int64* pos)
{
    if(pos)
    {
        *pos = sstr_.tellp();
        return Steinberg::kResultOk;
    }
    return Steinberg::kInvalidArgument;
}
}