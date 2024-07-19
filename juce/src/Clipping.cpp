/*
    Copyright 2020 Barabas Raffai

    This file is part of Valvestate Sim.

    Valvestate Sim is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    Valvestate Sim is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with Valvestate Sim.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Clipping.h"
#include "WaveShape.h"
#define OVERSAMPLING_ORDER (3)
#define FILTER_TYPE (juce::dsp::Oversampling<float>::filterHalfBandFIREquiripple)

/* TODO using IIR filter here causes the audio to mute after some random amount
 * of time, generally a few seconds. Investigate why that's happening.
 *
 *  - Using double processing doesn't help, and it breaks even when all other
 *  processsing is bypassed, so probably not related to numerical stability of
 *  IIR filter
 */
Clipping::Clipping() : oversampling(1, 0, FILTER_TYPE)
{
    waveshaper.functionToUse = waveshape;
}

Clipping::~Clipping() {}

void Clipping::process(juce::dsp::AudioBlock<float> block)
{
    auto oversampledBlock = oversampling.processSamplesUp(block);

    juce::dsp::ProcessContextReplacing<float> ctx(oversampledBlock);
    waveshaper.process(ctx);

    oversampling.processSamplesDown(block);
}

void Clipping::prepare(juce::dsp::ProcessSpec spec)
{
    // Add stages to oversampling based on the samplerate. We want 8x
    // oversampling when the input rate is 44.1kHz or 48kHz.
    oversampling.clearOversamplingStages();

    auto samplerate_ratio = spec.sampleRate / 44.1e3;
    juce::Logger::outputDebugString("samplerate_ratio: " + std::to_string(samplerate_ratio));
    if(samplerate_ratio < pow(2, OVERSAMPLING_ORDER))
    {
        int log_ratio = log2(samplerate_ratio);
        for(int n = 0; n < (OVERSAMPLING_ORDER - log_ratio); n++)
        {
            juce::Logger::outputDebugString("Added oversampling stage");
            /* Copied from the Oversampling constructor. For some reason there
             * is no function to add a stage using the parameters passed to the
             * constructor */
            bool isMaximumQuality = true;
            auto twUp   = (isMaximumQuality ? 0.10f : 0.12f) * (n == 0 ? 0.5f : 1.0f);
            auto twDown = (isMaximumQuality ? 0.12f : 0.15f) * (n == 0 ? 0.5f : 1.0f);

            auto gaindBStartUp    = (isMaximumQuality ? -90.0f : -70.0f);
            auto gaindBStartDown  = (isMaximumQuality ? -75.0f : -60.0f);
            auto gaindBFactorUp   = (isMaximumQuality ? 10.0f  : 8.0f);
            auto gaindBFactorDown = (isMaximumQuality ? 10.0f  : 8.0f);

            oversampling.addOversamplingStage (FILTER_TYPE,
                    twUp, gaindBStartUp + gaindBFactorUp * (float) n,
                    twDown, gaindBStartDown + gaindBFactorDown * (float) n);
        }
    }
    else
    {
        juce::Logger::outputDebugString("No oversampling necessary");
        oversampling.addDummyOversamplingStage();
    }

    oversampling.initProcessing(spec.maximumBlockSize);

    juce::Logger::outputDebugString("Oversampling latency (samples): " + std::to_string(oversampling.getLatencyInSamples()));
}

void Clipping::reset()
{
    oversampling.reset();
}
