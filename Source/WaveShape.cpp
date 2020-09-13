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

const float shape[100] = {
 260.874     , 260.36155152, 259.68101616, 258.83249394, 258.12373131,
 257.32490505, 256.30403636, 255.30199798, 254.34203232, 253.08117273,
 251.64408485, 250.62568889, 249.36276061, 247.85529798, 245.82016162,
 243.16351212, 239.88231717, 237.3534798 , 235.59948182, 233.75043737,
 231.81578283, 230.6437697 , 229.62558889, 228.59006869, 227.53750909,
 226.46871717, 225.38408687, 224.28356364, 222.27520101, 219.96075758,
 217.55801212, 216.27730909, 214.99263636, 213.66246667, 212.28120808,
 210.84921515, 209.37504545, 207.87093333, 206.3475404 , 204.81160303,
 203.2669899 , 201.43521919, 197.36075758, 193.30988586, 189.22967778,
 185.12009091, 180.98376869, 176.82380606, 172.64047273, 169.22337879,
 166.17794545, 163.12334848, 159.32633939, 155.88505556, 152.5707    ,
 149.25255556, 145.93375152, 142.61785152, 139.30987273, 137.36382626,
 135.72008182, 134.08206162, 132.45135758, 130.82926364, 129.21729596,
 127.21884545, 124.0715    , 121.00046061, 118.0256899 , 115.17009697,
 112.45427374, 109.70993535, 106.19916364, 103.49122626, 102.10980404,
 100.81236667,  99.59889232,  98.08463889,  97.36694788,  96.69223283,
  96.06147778,  95.47537636,  94.93400081,  94.24110576,  93.39900939,
  92.42075859,  91.82170939,  91.30980818,  90.88505333,  89.66791747,
  88.56323182,  87.84395242,  87.30069909,  86.73431242,  86.28330707,
  85.9333599 ,  85.61168818,  85.32950929,  85.08680323,  85.06124   
};

const int shape_size = 100;

float waveshape(float input)
{
    float output;
    //if input is out of range, hard clip
    //TODO: maybe it's better if we follow the gradient at the end of the shape
    if(input <= -1)
    {
        output = shape[0];
    }
    else if(input >= 1)
    {
       output = shape[shape_size - 1];
    }
    else
    {
        //calculate which sample of shape the input sample corresponds to
        //the first sample is -1, last is 1
        float index = (input + 1.0f)/2.0f * (shape_size-1);

        //linear interpolation
        //find ratio of x
        float ratio = (index-floor(index)) / (ceil(index)-floor(index));

        //y = c + mx
        output = shape[(int)floor(index)] +
            ratio*(shape[(int)ceil(index)]-shape[(int)floor(index)]);
    }

    return (output - shape[shape_size/2])/300.0f;
}
