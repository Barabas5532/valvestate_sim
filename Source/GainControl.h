#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class GainControl
{

    public:
        GainControl();
        ~GainControl();

        void setParameters(float g, float channel);

        void prepare(dsp::ProcessSpec spec);
        void process(dsp::ProcessContextReplacing<float> context);
        void reset();

    private:
        float sampleRate;

        dsp::IIR::Filter<float> filter;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainControl);
};
