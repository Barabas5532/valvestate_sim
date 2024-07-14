import("stdfaust.lib");
import("clipping.dsp");
import("fmv.dsp");

process = _ : clipping : fmv(bass,middle,treble) : _ with {
  bass = hslider("bass", 0.5, 0, 1, 0.01);
  middle = hslider("middle", 0.5, 0, 1, 0.01);
  treble = hslider("treble", 0.5, 0, 1, 0.01);
};
