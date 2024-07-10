import("stdfaust.lib");
import("clipping.dsp");
import("fmv.dsp");

process = _ : clipping : fmv(0.5,0.5,0.5) : _;
