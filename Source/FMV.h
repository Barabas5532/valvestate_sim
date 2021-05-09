#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FMVFilter
{
    public:
    FMVFilter();
    ~FMVFilter();

    void setParameters(float l, float m, float t);

    void prepare(dsp::ProcessSpec spec);
    void process(dsp::ProcessContextReplacing<float> context);
    void reset();

    private:

    dsp::IIR::Filter<float> filter;
    float sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FMVFilter)
};
