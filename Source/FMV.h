#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FMVFilter
{
    public:
    FMVFilter();
    ~FMVFilter();

    void prepare(dsp::ProcessSpec spec);
    void process(dsp::AudioBlock<float> block);
    void reset();

    private:
    dsp::IIR::Coefficients<float> design(float l, float m, float t);

    dsp::IIR::Filter<float> filter;
    float sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FMVFilter);
}
