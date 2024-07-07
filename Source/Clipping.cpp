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
#include "TableInterpolate.h"

#define OVERSAMPLING_ORDER (3)
#define FILTER_TYPE (dsp::Oversampling<float>::filterHalfBandFIREquiripple)

static float waveshape(float sample) {
    static constexpr float shape_arr[] = {
        260.874,    260.7874,    260.6957,    260.6018,    260.5046,
        260.4037,     260.299,    260.1902,     260.077,    259.9591,
        259.836,    259.7075,     259.573,     259.432,    259.2841,
        259.1285,    258.9645,    258.7914,    258.6082,    258.4137,
        258.2067,    257.9857,    257.7491,    257.4948,    257.2203,
        256.9227,    256.5983,    256.2427,    255.8505,    255.4145,
        254.9258,    254.3727,    253.7396,    253.0053,    252.1408,
        251.1054,    249.8192,    248.2382,     246.239,    243.6996,
        240.5233,    236.6973,    232.3052,    227.4654,    222.2534,
        216.6172,    210.1611,    202.9404,     195.589,    188.0678,
        180.4541,    172.7579,     164.991,    157.1672,    149.3067,
        141.4431,    133.6388,    126.0095,    118.7501,    112.1218,
        106.3677,    101.5698,    97.70392,      94.798,    92.84232,
        91.56661,    90.65301,    90.00365,    89.49298,    89.06787,
        88.70779,    88.39709,    88.12441,    87.88187,    87.66372,
        87.4657,    87.28455,     87.1177,    86.96315,    86.81925,
        86.68467,    86.55831,    86.43925,     86.3267,       86.22,
        86.11859,    86.02197,     85.9297,    85.84144,    85.75683,
        85.67559,    85.59747,    85.52222,    85.44965,    85.37957,
        85.31182,    85.24625,     85.1827,    85.12107,    85.06123,
    };
    static constexpr auto shape = std::span{shape_arr};
    
    float output = linear_interpolate_table(shape, (sample + 1.f) / 2.f);
    return (output - shape[shape.size()/2])/300.0f;
}

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

void Clipping::process(dsp::AudioBlock<float> block)
{
    auto oversampledBlock = oversampling.processSamplesUp(block);

    dsp::ProcessContextReplacing<float> ctx(oversampledBlock);
    waveshaper.process(ctx);

    oversampling.processSamplesDown(block);
}

void Clipping::prepare(dsp::ProcessSpec spec)
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
