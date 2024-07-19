import("stdfaust.lib");
import("clipping.dsp");
import("contour.dsp");
import("fmv.dsp");
import("gain.dsp");
import("input.dsp");


process = _ : input : gain(gain_, channel) : clipping : fmv(bass,middle,treble) : contour(contour_) : _ with {
  gain_ = hslider("[0]gain", 0.5, 0, 1, 0.01) : si.smoo;
  channel = checkbox("[1]channel") : si.smoo;
  bass = hslider("[2]bass", 0.5, 0, 1, 0.01) : si.smoo;
  middle = hslider("[3]middle", 0.5, 0, 1, 0.01) : si.smoo;
  treble = hslider("[4]treble", 0.5, 0, 1, 0.01) : si.smoo;
  contour_ = hslider("[5]contour", 0.5, 0, 1, 0.01) : si.smoo;
};
