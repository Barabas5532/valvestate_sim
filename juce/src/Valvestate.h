#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Clipping.h"
#include "Contour.h"
#include "FMV.h"
#include "GainControl.h"
#include "InputFilter.h"
#include "ValvestateProcessorBase.h"
#include "WaveShape.h"

class ValvestateProcessor final : public ValvestateProcessorBase {
public:
  void prepare(const juce::dsp::ProcessSpec &) override;
  void process(const juce::dsp::ProcessContextReplacing<float> &) override;
  void reset() override;

  void setParameters(float gain, bool od, float bass, float middle,
                     float treble, float contour, float volume) override;

private:
  float volume;
  InputFilter input;
  GainControl gaincontrol;
  Clipping clipping;
  FMVFilter fmv;
  ContourFilter contour;
};
