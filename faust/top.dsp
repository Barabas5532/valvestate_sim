import("stdfaust.lib");
import("clipping.dsp");
import("contour.dsp");
import("fmv.dsp");


process = _ : clipping : fmv(bass,middle,treble) : contour(contour_) : _ with {
  bass = hslider("[0]bass", 0.5, 0, 1, 0.01) : si.smoo;
  middle = hslider("[1]middle", 0.5, 0, 1, 0.01) : si.smoo;
  treble = hslider("[2]treble", 0.5, 0, 1, 0.01) : si.smoo;
  contour_ = hslider("[3]contour", 0.5, 0, 1, 0.01) : si.smoo;
};
