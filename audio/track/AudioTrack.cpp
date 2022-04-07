#include "AudioTrack.hpp"

namespace Musec::Audio::Track
{
template<typename SampleType>
void AudioTrack<SampleType>::process()
{
    for(auto& sequence: pluginSequences_)
    {
        for(auto& plugin: sequence)
        {
            // TODO
        }
    }
}

template class AudioTrack<float>;
template class AudioTrack<double>;
}
