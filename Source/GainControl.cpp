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

#include "GainControl.h"

GainControl::GainControl(){}
GainControl::~GainControl(){}

void GainControl::setParameters(float g, float channel)
{
    float B0, B1, B2, A0, A1, A2;
    float K = 2*sampleRate;

    jassert(channel >= 0.0f && channel <= 1.0f);

    if (channel < 0.5)
    {
        //OD1
        B0 = 0.00916582638188652*K*g;
        B1 = 0;
        B2 = -0.00916582638188652*K*g;

        A0 = 4.30793839948666e-8*std::pow(K, 2)*g + 2.35e-5*K*g +
            0.000916582638188652*K + 0.5;
        A1 = -8.61587679897332e-8*std::pow(K, 2)*g + 1.0;
        A2 = 4.30793839948666e-8*std::pow(K, 2)*g - 2.35e-5*K*g -
            0.000916582638188652*K + 0.5;
    }
    else
    {
        //OD2
        B0 = 0.11*K*g;
        B1 = 0;
        B2 = -0.11*K*g;

        A0 = 4.69953e-8*std::pow(K, 2)*g + 2.35e-5*K*g + 0.0009999*K + 0.5;
        A1 = -9.39906e-8*std::pow(K, 2)*g + 1.0;
        A2 = 4.69953e-8*std::pow(K, 2)*g - 2.35e-5*K*g - 0.0009999*K + 0.5;
    }

    filter.coefficients = 
            new dsp::IIR::Coefficients<float> (B0, B1, B2, A0, A1, A2);
}

void GainControl::prepare(dsp::ProcessSpec spec)
{
    filter.prepare(spec);
    sampleRate = spec.sampleRate;
}

void GainControl::process(dsp::ProcessContextReplacing<float> context)
{
    filter.process(context);
}

void GainControl::reset()
{
    filter.reset();
}
