#include "AudioSequence.hpp"

#include <avcpp/av.h>
#include <avcpp/codec.h>
#include <avcpp/codeccontext.h>
#include <avcpp/format.h>
#include <avcpp/formatcontext.h>

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Media
{
namespace Impl
{

class AvCppInit
{
private:
    AvCppInit() { av::init(); }
public:
    static void init()
    {
        static AvCppInit avCppInit;
    }
};
}
AudioSequence::AudioSequence(double sampleRate, std::uint8_t channelCount, int bitDepth,
    SampleFormat sampleFormat, std::size_t sampleCountPerChannel):
    sampleRate_(sampleRate),
    channelCount_(channelCount),
    bitDepth_(sampleFormat == SampleFormat::Integer? bitDepth: bitDepth * (-1)),
    sampleCountPerChannel_(sampleCountPerChannel),
    content_(sampleCountPerChannel_ * bitDepth * channelCount)
{
}

AudioSequence::AudioSequence(const AudioSequence& rhs):
    sampleRate_(rhs.sampleRate_),
    channelCount_(rhs.channelCount_),
    bitDepth_(rhs.bitDepth_),
    sampleCountPerChannel_(rhs.sampleCountPerChannel_),
    content_(sampleCountPerChannel_ * rhs.bitDepth() * channelCount_)
{
    std::memcpy(content_.data(), rhs.content_.data(), content_.blockSize());
}

AudioSequence::AudioSequence(AudioSequence&& rhs) noexcept:
    sampleRate_(rhs.sampleRate_),
    channelCount_(rhs.channelCount_),
    bitDepth_(rhs.bitDepth_),
    sampleCountPerChannel_(rhs.sampleCountPerChannel_),
    content_(std::move(rhs.content_))
{
    //
}

AudioSequence::~AudioSequence() noexcept
{

}

double AudioSequence::sampleRate() const
{
    return sampleRate_;
}

std::uint8_t AudioSequence::channelCount() const
{
    return channelCount_;
}

int AudioSequence::bitDepth() const
{
    return bitDepth_ < 0? bitDepth_ * (-1): bitDepth_;
}

SampleFormat AudioSequence::sampleFormat() const
{
    return bitDepth_ < 0? SampleFormat::Float: SampleFormat::Integer;
}

const Musec::Base::FixedSizeMemoryBlock& AudioSequence::content() const
{
    return content_;
}

Musec::Base::FixedSizeMemoryBlock& AudioSequence::content()
{
    return content_;
}

std::vector<AudioSequence> loadAudioSequenceFromFile(const QString& path)
{
    using namespace av;
    Impl::AvCppInit::init();
    auto uri = path.toStdString();
    av::AudioDecoderContext audioDecoderContext;
    av::Stream audioStream;
    std::vector<int> audioStreamIndexes;
    std::vector<AudioSequence> ret;
    std::error_code errorCode;
    {
        InputFormat inputFormat;
        FormatContext inputContext;
        inputContext.openInput(uri, inputFormat, errorCode);
        if(errorCode)
        {
            //
        }
        inputContext.findStreamInfo(errorCode);
        auto streamsCount = inputContext.streamsCount();
        audioStreamIndexes.reserve(streamsCount);
        ret.reserve(streamsCount);
        for(int i = 0; i < inputContext.streamsCount(); ++i)
        {
            auto stream = inputContext.stream(i);
            if(stream.isValid() && stream.isAudio())
            {
                audioStreamIndexes.emplace_back(i);
            }
        }
        auto audioStreamCount = audioStreamIndexes.size();
        for(int i = 0; i < audioStreamCount; ++i)
        {
            auto audioStream = inputContext.stream(audioStreamIndexes[i]);
            audioDecoderContext = AudioDecoderContext(audioStream);
            auto codecId = audioDecoderContext.raw()->codec_id;
            Codec codec = findDecodingCodec(codecId);
            if(codec.isNull())
            {
                // 找不到解码器
            }
            audioDecoderContext.setCodec(codec);
            audioDecoderContext.setRefCountedFrames(true);
            audioDecoderContext.open(errorCode);
            auto bitDepth = audioDecoderContext.sampleFormat().bitsPerSample();
            auto estimateSampleCountPerChannel = static_cast<size_t>(
                static_cast<double>(audioStream.timeBase().getNumerator()
                * audioStream.raw()->duration
                / static_cast<double>(audioStream.timeBase().getDenominator())
                * audioDecoderContext.sampleRate())
            );
            inputContext.substractStartTime(false);
            auto bytesPerSample = bitDepth / CHAR_BIT;
            auto sampleFormat = audioDecoderContext.sampleFormat().get();
            ret.emplace_back(
                audioDecoderContext.sampleRate(),
                audioDecoderContext.channels(),
                audioDecoderContext.sampleFormat().bitsPerSample(),
                sampleFormat == AV_SAMPLE_FMT_FLT
                || sampleFormat == AV_SAMPLE_FMT_DBL
                || sampleFormat == AV_SAMPLE_FMT_FLTP
                || sampleFormat == AV_SAMPLE_FMT_DBLP?
                SampleFormat::Float: SampleFormat::Integer,
                estimateSampleCountPerChannel
                );
            Packet packet;
            while(true)
            {
                packet = inputContext.readPacket(errorCode);
                if(errorCode)
                {
                    break;
                }
                if(!packet)
                {
                    break;
                }
                if(packet.streamIndex() != audioStreamIndexes[i])
                {
                    continue;
                }
                av::AudioSamples samples = audioDecoderContext.decode(packet, errorCode);
                if(errorCode)
                {
                    break;
                }
                if(!samples)
                {
                    break;
                }
                auto samplesCount = samples.samplesCount();
                auto count = samples.samplesCount() * bytesPerSample;
                if(audioDecoderContext.sampleFormat().isPlanar())
                {
                    for(decltype(count) i = 0; i < count; i += bytesPerSample)
                    {
                        for(int j = 0; j < audioDecoderContext.channels(); ++j)
                        {
                            // TODO: 写入 ret.back().content().data()
                            auto data = reinterpret_cast<const char*>(samples.data(j)) + i;
                            auto size = bytesPerSample;
                        }
                    }
                }
                else
                {
                    // TODO: 写入 ret.back().content().data()
                    auto data = reinterpret_cast<const char*>(samples.data());
                    auto size = samples.samplesCount() * samples.channelsCount() * bytesPerSample;
                }
            }
        }
    }
}

AudioSequence convertAudioSequence(AudioSequence& audioSequence, double sampleRate, std::uint8_t channelCount,
    int bitDepth, SampleFormat sampleFormat)
{
    return AudioSequence(0, 0, 0, Float, 0);
}
}
}
}