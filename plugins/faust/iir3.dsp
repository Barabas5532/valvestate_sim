// These filters are not exactly Direct Form II Transposed structures. The
// recursive composition operator (~) in faust forces the delay to go just
// before the recursive part, so the recursive part of the structure is
// slightly different.
//
// Maybe using `with` or `letrec` block to implement the body will allow us to
// make it completely correct. At first glance it doesn't seem to work. When
// using `with`, the compiler can't handle the recursion from feedback. Maybe
// `letrec` with inputs (*(-a1),*(-a2),...) will work? Then the coefficient
// multiplies are before the feedback.

// Direct form II Transposed 3rd order IIR filter with minimal delay elements.
iir3(b0,b1,b2,b3,a1,a2,a3) =
    _ : (_,_,_,(_ <: *(b3),*(b2),*(b1),*(b0))
      : iir3_delayline :> _)~*(-a1)~*(-a2)~*(-a3) : _ with {
    iir3_delayline(fb0,fb1,fb2,ff3,ff2,ff1,ff0) = out with {
        out = ff0 + fb0 + d0';
        d0  = ff1 + fb1 + d1';
        d1  = ff2 + fb2 + d2';
        d2  = ff3;
    };
};

// Direct form II Transposed 4th order IIR filter with minimal delay elements.
iir4(b0,b1,b2,b3,b4,a1,a2,a3,a4) =
    _ : (_,_,_,_,(_ <: *(b4),*(b3),*(b2),*(b1),*(b0))
      : iir4_delayline :> _)~*(-a1)~*(-a2)~*(-a3)~*(-a4) : _ with {
    iir4_delayline(fb0,fb1,fb2,fb3,ff4,ff3,ff2,ff1,ff0) = out with {
        out = ff0 + fb0 + d0';
        d0  = ff1 + fb1 + d1';
        d1  = ff2 + fb2 + d2';
        d2  = ff3 + fb3 + d3';
        d3  = ff4;
    };
};

