//
//  main.c
//  genwav
//
// Copyright (c) 2024 wry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <math.h>
#include <stdint.h>
#include <unistd.h>

#ifndef TAU
#   define TAU 6.283185307179586
#endif

// http://soundfile.sapp.org/doc/WaveFormat/
typedef struct WaveHeader
{
    // ChunkID ("RIFF")
    char chunkID[4];
    // ChunkSize (4 + (8+SubChunk1Size) + (8 + SubChunk2Size))
    uint32_t chunkSize;
    // Format ("WAVE")
    char format[4];

    // Subchunk1ID ("fmt ")
    char subChunk1ID[4];
    // Subchunk1Size
    uint32_t subChunk1Size;
    // AudioFormat (1 == uncompressed)
    uint16_t audioFormat;
    // NumChannels
    uint16_t numChannels;
    // SampleRate
    uint32_t sampleRate;
    // ByteRate (SampleRate * NumChannels * BitsPerSample / 8)
    uint32_t byteRate;
    // BlockAlign (NumChannels * BitsPerSample / 8)
    uint16_t blockAlign;
    // BitsPerSample
    uint16_t bitsPerSample;

    // Subchunk2ID ("data")
    char subChunk2ID[4];
    // Subchunk2Size (NumSamples * NumChannels * BitsPerSample / 8)
    uint32_t subChunk2Size;
} WaveHeader;

static void WaveHeader_init(WaveHeader *hdr, uint32_t sampleRate, uint16_t bitsPerSample, uint16_t numChannels, uint32_t duration)
{
    const uint32_t subChunk2Size = (sampleRate * duration) * numChannels * bitsPerSample / 8;
    
    hdr->chunkID[0] = 'R';
    hdr->chunkID[1] = 'I';
    hdr->chunkID[2] = 'F';
    hdr->chunkID[3] = 'F';
    hdr->chunkSize = 36 + subChunk2Size;
    hdr->format[0] = 'W';
    hdr->format[1] = 'A';
    hdr->format[2] = 'V';
    hdr->format[3] = 'E';

    hdr->subChunk1ID[0] = 'f';
    hdr->subChunk1ID[1] = 'm';
    hdr->subChunk1ID[2] = 't';
    hdr->subChunk1ID[3] = ' ';
    hdr->subChunk1Size = 16;
    hdr->audioFormat = 1;
    hdr->numChannels = numChannels;
    hdr->sampleRate = sampleRate;
    hdr->byteRate = sampleRate * numChannels * bitsPerSample / 8;
    hdr->blockAlign = numChannels * bitsPerSample / 8;
    hdr->bitsPerSample = bitsPerSample;

    hdr->subChunk2ID[0] = 'd';
    hdr->subChunk2ID[1] = 'a';
    hdr->subChunk2ID[2] = 't';
    hdr->subChunk2ID[3] = 'a';
    hdr->subChunk2Size = subChunk2Size;
}

typedef struct SineOscillator
{
    float frequency;
    float amplitude;
    float angle;
    float step;
} SineOscillator;

static void SineOscillator_init(SineOscillator *osc, float sampleRate, float frequency, float amplitude)
{
    osc->frequency = frequency;
    osc->amplitude = amplitude;
    osc->angle = 0.0f;
    osc->step = (float)TAU * frequency / sampleRate;
}

static float SineOscillator_sample(SineOscillator *osc)
{
    const float sample = osc->amplitude * sinf(osc->angle);
    osc->angle += osc->step;
    return sample;
}

int main(void)
{
    const int sampleRate = 44100;
    const int bitsPerSample = 16;
    const int duration = 2;
    const int maxAmp = (1 << bitsPerSample)-1;

    WaveHeader wav = {0};
    SineOscillator osc = {0};

    WaveHeader_init(&wav, sampleRate, bitsPerSample, 1, duration);
    write(STDOUT_FILENO, (void *)&wav, sizeof(wav));

    SineOscillator_init(&osc, sampleRate, 440, 0.5);
    
    for (int i = 0; i < sampleRate * duration; i++)
    {
        float samplef = SineOscillator_sample(&osc);
        int samplei = (int)(samplef * (float)maxAmp);
        write(STDOUT_FILENO, (void *)&samplei, bitsPerSample / 8);
    }

    return 0;
}
