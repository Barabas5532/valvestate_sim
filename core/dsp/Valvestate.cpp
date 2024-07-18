#include "Valvestate.h"

void ValvestateProcessor::prepare(const dsp::ProcessSpec &spec) {
  input.prepare(spec);
  gaincontrol.prepare(spec);
  clipping.prepare(spec);
  fmv.prepare(spec);
  contour.prepare(spec);
}

void ValvestateProcessor::process(
    const dsp::ProcessContextReplacing<float> &context) {
  input.process(context);
  gaincontrol.process(context);
  clipping.process(context.getOutputBlock());
  fmv.process(context);
  contour.process(context);
  context.getOutputBlock().multiplyBy(Decibels::decibelsToGain(volume));
}

void ValvestateProcessor::reset() {
  input.reset();
  gaincontrol.reset();
  clipping.reset();
  fmv.reset();
  contour.reset();
}

void ValvestateProcessor::setParameters(float gain, bool od, float bass,
                                        float middle, float treble,
                                        float a_contour, float a_volume) {
  gaincontrol.setParameters(gain, od);
  fmv.setParameters(bass, middle, treble);
  contour.setParameter(a_contour);
  volume = a_volume;
}
