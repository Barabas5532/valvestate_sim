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

#pragma once
#include <juce_dsp/juce_dsp.h>

using namespace juce;

class ContourFilter
{
    public:
    ContourFilter();
    ~ContourFilter();

    void prepare(dsp::ProcessSpec spec);
    void process(dsp::ProcessContextReplacing<float> context);
    void reset();

    void setParameter(float p);

    private:
    dsp::IIR::Coefficients<float> design(float p);

    dsp::IIR::Filter<float> filter;
    float sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContourFilter)
};
