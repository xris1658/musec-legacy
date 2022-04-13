#ifndef MUSEC_AUDIO_MEDIA_AUDIOSEQUENCE
#define MUSEC_AUDIO_MEDIA_AUDIOSEQUENCE

#include "base/FixedSizeMemoryBlock.hpp"

#include <QString>

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Media
{
enum SampleFormat
{
    Integer,
    Float
};
class AudioSequence
{
public:
    AudioSequence(double sampleRate, std::uint8_t channelCount, int bitDepth,
        SampleFormat sampleFormat, std::size_t sampleCountPerChannel);
    AudioSequence(const AudioSequence& rhs);
    AudioSequence(AudioSequence&& rhs) noexcept;
    ~AudioSequence() noexcept;
public:
    double sampleRate() const;
    std::uint8_t channelCount() const;
    int bitDepth() const;
    SampleFormat sampleFormat() const;
    const Musec::Base::FixedSizeMemoryBlock& content() const;
    Musec::Base::FixedSizeMemoryBlock& content();
private:
    double sampleRate_;
    std::uint8_t channelCount_;
    int bitDepth_;
    std::size_t sampleCountPerChannel_;
    Musec::Base::FixedSizeMemoryBlock content_;
};

std::vector<AudioSequence> loadAudioSequenceFromFile(const QString& path);

AudioSequence convertAudioSequence(AudioSequence& audioSequence, double sampleRate,
    std::uint8_t channelCount, int bitDepth, SampleFormat sampleFormat);
}
}
}

#endif //MUSEC_AUDIO_MEDIA_AUDIOSEQUENCE
