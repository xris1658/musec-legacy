#ifndef MUSEC_AUDIO_BASE_AUDIOBUFFERVIEW
#define MUSEC_AUDIO_BASE_AUDIOBUFFERVIEW

#include <cstdlib>
#include <stdexcept>
#include <vector>

namespace Musec
{
namespace Audio
{
namespace Base
{
// 不持有缓冲区内存的音频缓冲区查看器
template<typename SampleType>
class AudioBufferView
{
    static_assert(std::is_arithmetic_v<SampleType>);
    using Self = AudioBufferView<SampleType>;
public:
    AudioBufferView(SampleType* samples, size_t size):
        samples_(samples),
        size_(size)
    {}
    AudioBufferView():
        samples_(nullptr),
        size_(0)
    {}
    AudioBufferView(const Self&) = default;
    Self& operator=(const Self&) = default;
    AudioBufferView(Self&&) noexcept = default;
    Self& operator=(Self&&) noexcept = default;
    ~AudioBufferView() noexcept = default;
public:
    SampleType* getSamples() const noexcept
    {
        return samples_;
    }
    size_t size() const noexcept
    {
        return size_;
    }
public:
    void init()
    {
        std::memset(samples_, 0, size_ * sizeof(SampleType));
    }
public:
    const SampleType& operator[](std::size_t index) const
    {
        return samples_[index];
    }
    SampleType& operator[](std::size_t index)
    {
        return samples_[index];
    }
    const SampleType& at(std::size_t index) const
    {
        if(index >= size_)
        {
            throw std::out_of_range("");
        }
        return (*this)[index];
    }
    SampleType& at(std::size_t index)
    {
        if(index >= size_)
        {
            throw std::out_of_range("");
        }
        return (*this)[index];
    }
private:
    SampleType* samples_;
    std::size_t size_;
};

void clip(AudioBufferView<float>& bufferView);

void clip(AudioBufferView<double>& bufferView);

void clip(AudioBufferView<long double>& bufferView);

template<typename SampleType>
bool blank(const AudioBufferView<SampleType>& bufferView)
{
    for(decltype(bufferView.size()) i = 0; i < bufferView.size(); ++i)
    {
        if(bufferView[i] != SampleType(0))
        {
            return false;
        }
    }
    return true;
}
}
}
}

#endif //MUSEC_AUDIO_BASE_AUDIOBUFFERVIEW
