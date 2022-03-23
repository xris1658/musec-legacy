#ifndef MUSEC_AUDIO_BASE_AUDIOBUFFERVIEW
#define MUSEC_AUDIO_BASE_AUDIOBUFFERVIEW

#include <cstdlib>
#include <stdexcept>

namespace Musec
{
namespace Audio
{
namespace Base
{
template<typename SampleType>
class AudioBufferView
{
    using Self = AudioBufferView<SampleType>;
public:
    AudioBufferView(SampleType* samples, size_t size): samples_(samples), size_(size)
    {}
    AudioBufferView(const Self&) = default;
    Self& operator=(const Self&) = default;
    AudioBufferView(Self&&) noexcept = default;
    Self& operator=(Self&&) noexcept = default;
    ~AudioBufferView() noexcept = default;
public:
    SampleType* getSamples() const
    {
        return samples_;
    }
    size_t size() const
    {
        return size_;
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
}
}
}

#endif //MUSEC_AUDIO_BASE_AUDIOBUFFERVIEW
