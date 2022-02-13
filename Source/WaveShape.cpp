/*
    Copyright 2020 Barabas Raffai

    This file is part of Valvestate Sim.

    Valvestate Sim is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    Valvestate Sim is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License along
    with Valvestate Sim.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "WaveShape.h"
#include <math.h>
#include <stddef.h>

#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

static const float SHAPE[] = {
     260.874,    260.7874,    260.6957,    260.6018,    260.5046,
    260.4037,     260.299,    260.1902,     260.077,    259.9591,
     259.836,    259.7075,     259.573,     259.432,    259.2841,
    259.1285,    258.9645,    258.7914,    258.6082,    258.4137,
    258.2067,    257.9857,    257.7491,    257.4948,    257.2203,
    256.9227,    256.5983,    256.2427,    255.8505,    255.4145,
    254.9258,    254.3727,    253.7396,    253.0053,    252.1408,
    251.1054,    249.8192,    248.2382,     246.239,    243.6996,
    240.5233,    236.6973,    232.3052,    227.4654,    222.2534,
    216.6172,    210.1611,    202.9404,     195.589,    188.0678,
    180.4541,    172.7579,     164.991,    157.1672,    149.3067,
    141.4431,    133.6388,    126.0095,    118.7501,    112.1218,
    106.3677,    101.5698,    97.70392,      94.798,    92.84232,
    91.56661,    90.65301,    90.00365,    89.49298,    89.06787,
    88.70779,    88.39709,    88.12441,    87.88187,    87.66372,
     87.4657,    87.28455,     87.1177,    86.96315,    86.81925,
    86.68467,    86.55831,    86.43925,     86.3267,       86.22,
    86.11859,    86.02197,     85.9297,    85.84144,    85.75683,
    85.67559,    85.59747,    85.52222,    85.44965,    85.37957,
    85.31182,    85.24625,     85.1827,    85.12107,    85.06123,
};

float waveshape(float input)
{
    float output;
    const float *shape = SHAPE;
    size_t shape_size = ARRAY_LENGTH(SHAPE);

    //calculate which sample of shape the input sample corresponds to
    //the first sample is -1, last is 1
    float index = (input + 1.0f) / 2.0f * (shape_size - 1);

    //if input is out of range, hard clip
    if(index <= 0)
    {
        output = shape[0];
    }
    else if((int)index + 1 > (int)(shape_size - 1))
    {
        output = shape[shape_size - 1];
    }
    else
    {
        // linear interpolation between two nearest elements in shape.
        // conversion to int rounds down
        float ratio = index - ((int)index);
        output = shape[(int)index] + ratio*(shape[((int)index) + 1] - shape[(int)index]);
    }

    return (output - shape[shape_size/2])/300.0f;
}
