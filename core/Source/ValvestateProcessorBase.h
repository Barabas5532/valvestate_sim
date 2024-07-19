#pragma once

#include "juce_dsp/juce_dsp.h"

class ValvestateProcessorBase : public juce::dsp::ProcessorBase {
public:
  virtual void setParameters(float gain, bool od, float bass, float middle,
                             float treble, float contour, float volume) = 0;
};

