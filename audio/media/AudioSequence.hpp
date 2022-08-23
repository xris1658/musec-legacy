#ifndef MUSEC_AUDIO_MEDIA_AUDIOSEQUENCE
#define MUSEC_AUDIO_MEDIA_AUDIOSEQUENCE

#include "base/FixedSizeMemoryBlock.hpp"

#include <QObject>
#include <QString>

#include <cstddef>
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

// Not interleaving
class AudioSequence: public QObject
{
    Q_OBJECT
public:
    AudioSequence(double sampleRate, std::uint8_t channelCount, int bitDepth,
        SampleFormat sampleFormat, std::size_t sampleCountPerChannel);
    AudioSequence(const AudioSequence& rhs);
    AudioSequence(AudioSequence&& rhs) noexcept;
    ~AudioSequence() noexcept;
private:
    std::byte* raw() const;
public:
    double sampleRate() const;
    std::uint8_t channelCount() const;
    int bitDepth() const;
    int byteCountPerSample() const;
    SampleFormat sampleFormat() const;
    std::size_t sampleCountPerChannel() const;
    const Musec::Base::FixedSizeMemoryBlock& content() const;
    Musec::Base::FixedSizeMemoryBlock& content();
    const std::byte* operator()(std::uint8_t channelIndex, std::size_t sampleIndex = 0) const;
    std::byte* operator()(std::uint8_t channelIndex, std::size_t sampleIndex = 0);
private:
    double sampleRate_;
    std::uint8_t channelCount_;
    int bitDepth_;
    std::size_t sampleCountPerChannel_;
    Musec::Base::FixedSizeMemoryBlock content_;
};

std::vector<AudioSequence> loadAudioSequenceFromFile(const QString& path);
}
}
}

#endif //MUSEC_AUDIO_MEDIA_AUDIOSEQUENCE
