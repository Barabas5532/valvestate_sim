#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class InputFilter
{
    public:
    InputFilter();
    ~InputFilter();

    void prepare(dsp::ProcessSpec spec);
    void process(dsp::ProcessContextReplacing<float> context);
    void reset();

    private:
    dsp::IIR::Filter<float> filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputFilter);
};
