#include "AudioBufferView.hpp"

namespace Musec::Audio::Base
{
void clip(AudioBufferView<float>& bufferView)
{
    for(decltype(bufferView.size()) i = 0; i < bufferView.size(); ++i)
    {
        if(bufferView[i] > 1.0f)
        {
            bufferView[i] = 1.0f;
        }
        else if(bufferView[i] < -1.0f)
        {
            bufferView[i] = -1.0f;
        }
    }
}

void clip(AudioBufferView<double>& bufferView)
{
    for(decltype(bufferView.size()) i = 0; i < bufferView.size(); ++i)
    {
        if(bufferView[i] > 1.0)
        {
            bufferView[i] = 1.0;
        }
        else if(bufferView[i] < -1.0)
        {
            bufferView[i] = -1.0;
        }
    }
}

void clip(AudioBufferView<long double>& bufferView)
{
    for(decltype(bufferView.size()) i = 0; i < bufferView.size(); ++i)
    {
        if(bufferView[i] > 1.0l)
        {
            bufferView[i] = 1.0l;
        }
        else if(bufferView[i] < -1.0l)
        {
            bufferView[i] = -1.0l;
        }
    }
}
}