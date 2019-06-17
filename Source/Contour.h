#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContourFilter);
};
