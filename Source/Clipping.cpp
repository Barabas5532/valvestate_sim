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
