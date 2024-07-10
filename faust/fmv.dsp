import("stdfaust.lib");
import("iir3.dsp");

fmv(l, m, t) = _ : iir3(B0, B1, B2, B3, A0, A1, A2, A3)
with {
    R1 = 220e3;
    R2 = 1e6;
    R3 = 22e3;
    R4 = 33e3;

    C1 = 470e-12;
    C2 = 22e-9;
    C3 = 22e-9;

    b1 = t * C1 * R1 + m * C3 * R3 + l * (C1 * R2 + C2 * R2) +
        (C1 * R3 + C2 * R3);

    b2 = t * (C1 * C2 * R1 * R4 + C1 * C3 * R1 * R4 )
        - m * m * (C1 * C3 * R3 * R3  + C2 * C3 * R3 * R3 )
        + m * (C1 * C3 * R1 * R3 + C1 * C3 * R3 * R3 + C2 * C3 * R3 * R3)
        + l * (C1 * C2 * R1 * R2 + C1 * C2 * R2 * R4 + C1 * C3 * R2 * R4)
        + l * m * (C1 * C3 * R2 * R3 + C2 * C3 * R2 * R3)
        + (C1 * C2 * R1 * R3 + C1 * C2 * R3 * R4 + C1 * C3 * R3 * R4);

    b3 = l*m*(C1 * C2 * C3 * R1 * R2 * R3 + C1 * C2 * C3 * R2 * R3 * R4 )
        - m * m * (C1 * C2 * C3 * R1 * R3 * R3 + C1 * C2 * C3 * R3 * R3 * R4 )
        + m * (C1 * C2 * C3 * R1 * R3 * R3 + C1 * C2 * C3 * R3 * R3 * R4 )
        + t * C1 * C2 * C3 * R1 * R3 * R4 - t * m * C1 * C2 * C3 * R1 * R3 * R4
        + t * l * C1 * C2 * C3 * R1 * R2 * R4;

    a0 = 1;

    a1 = (C1 * R1 + C1 * R3 + C2 * R3 + C2 * R4 + C3 * R4 )
        + m * C3 * R3 + l * (C1 * R2 + C2 * R2);

    a2 = m * (C1 * C3 * R1 * R3 - C2 * C3 * R3 * R4 + C1 * C3 * R3 * R3
        + C2 * C3 * R3 * R3 ) + l * m * (C1 * C3 * R2 * R3 + C2 * C3 * R2 * R3 )
        - m*m * (C1 * C3 * R3 * R3 + C2 * C3 * R3 * R3 ) + l*(C1 * C2 * R2 * R4
        + C1 * C2 * R1 * R2 + C1 * C3 * R2 * R4 + C2 * C3 * R2 * R4 )
        + (C1 * C2 * R1 * R4 + C1 * C3 * R1 * R4 + C1 * C2 * R3 * R4
        + C1 * C2 * R1 * R3 + C1 * C3 * R3 * R4 + C2 * C3 * R3 * R4 );

    a3 = l*m*(C1 * C2 * C3 * R1 * R2 * R3 + C1 * C2 * C3 * R2 * R3 * R4 )
        - m * m * (C1 * C2 * C3 * R1 * R3 * R3 + C1 * C2 * C3 * R3 * R3 * R4)
        + m * (C1 * C2 * C3 * R3 * R3 * R4 + C1 * C2 * C3 * R1 * R3 * R3
                - C1 * C2 * C3 * R1 * R3 * R4 )
        + l * C1 * C2 * C3 * R1 * R2 * R4 + C1 * C2 * C3 * R1 * R3 * R4;

    //bilinear transform
    c = 2*ma.SR;
    c2 = c*c;
    c3 = c*c*c;

    B0 = -b1 * c- b2 * c2 - b3 * c3;
    B1 = -b1 * c+ b2 * c2 + 3 * b3 * c3;
    B2 = b1 * c+ b2 * c2 - 3 * b3 * c3;
    B3 = b1 * c- b2 * c2 + b3 * c3;

    A0 = -a0 - a1 * c- a2 * c2 - a3 * c3;
    A1 = -3 * a0 - a1 * c+ a2 * c2 + 3 * a3 * c3;
    A2 = -3 * a0 + a1 * c+ a2 * c2 - 3 * a3 * c3;
    A3 = -a0 + a1 * c- a2 * c2 + a3 * c3;
} : _;
