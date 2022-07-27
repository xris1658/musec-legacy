#include "AudioBufferView.hpp"

#include <algorithm>

namespace Musec::Audio::Base
{
void clip(AudioBufferView<float>& bufferView)
{
    for(decltype(bufferView.size()) i = 0; i < bufferView.size(); ++i)
    {
        bufferView[i] = std::clamp(bufferView[i], -1.0f, 1.0f);
    }
}

void clip(AudioBufferView<double>& bufferView)
{
    for(decltype(bufferView.size()) i = 0; i < bufferView.size(); ++i)
    {
        bufferView[i] = std::clamp(bufferView[i], -1.0, 1.0);
    }
}

void clip(AudioBufferView<long double>& bufferView)
{
    for(decltype(bufferView.size()) i = 0; i < bufferView.size(); ++i)
    {
        bufferView[i] = std::clamp(bufferView[i], -1.0l, 1.0l);
    }
}
}