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

#include "TableInterpolate.h"

float linear_interpolate_table(std::span<const float> table, float input) {
  float output;
  const float *data = table.data();
  size_t size = table.size();

  //calculate which entry of table the input corresponds to
  //the first entry is 0, last is 1
  float index = input * (size - 1);

  //if input is out of range, hard clip
  if(index <= 0)
  {
    output = data[0];
  }
  else if((int)index + 1 > (int)(size - 1))
  {
    output = data[size - 1];
  }
  else
  {
    // linear interpolation between two nearest elements in table.
    // conversion to int rounds down
    float ratio = index - ((int)index);
    output =
        data[(int)index] + ratio*(data[((int)index) + 1] - data[(int)index]);
  }

  return output;
}
