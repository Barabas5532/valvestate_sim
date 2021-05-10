#include "Clipping.h"
#include "WaveShape.h"
#define OVERSAMPLING_ORDER 3

/* TODO using IIR filter here causes the audio to mute after some random amount
 * of time, generally a few seconds. Investigate why that's happening.
 *
 *  - Using double processing doesn't help, and it breaks even when all other
 *  processsing is bypassed, so probably not related to numerical stability of
 *  IIR filter
 */
Clipping::Clipping() : oversampling(1, OVERSAMPLING_ORDER,
        dsp::Oversampling<float>::filterHalfBandFIREquiripple)
{
    waveshaper.functionToUse = waveshape;
}

Clipping::~Clipping() {}

void Clipping::process(dsp::AudioBlock<float> block)
{
    auto oversampledBlock = oversampling.processSamplesUp(block);

    dsp::ProcessContextReplacing<float> ctx(oversampledBlock);
    waveshaper.process(ctx);

    oversampling.processSamplesDown(block);
}

void Clipping::prepare(dsp::ProcessSpec spec)
{
    oversampling.initProcessing(spec.maximumBlockSize);

    juce::Logger::outputDebugString("Oversampling latency (samples): " + std::to_string(oversampling.getLatencyInSamples()));
}

void Clipping::reset()
{
    oversampling.reset();
}
