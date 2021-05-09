/*
  ==============================================================================

    Clipping.h
    Created: 14 Jun 2019 9:46:07am
    Author:  barabas

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Clipping
{
    public:
        Clipping();
        ~Clipping();

        void process(dsp::AudioBlock<float> block);
        void prepare(dsp::ProcessSpec spec);
        void reset();

    private:
        dsp::WaveShaper<float> waveshaper;
        dsp::Oversampling<float> oversampling;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Clipping)
};
