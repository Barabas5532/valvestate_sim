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

#pragma once

#include "BackgroundBinaryData.h"
#include "bass_binary_data.h"
#include "contour_binary_data.h"
#include "gain_binary_data.h"
#include "middle_binary_data.h"
#include "treble_binary_data.h"
#include "volume_binary_data.h"

const char *get_gain_image_resource(int &dataSizeInBytes);
const char *get_bass_image_resource(int &dataSizeInBytes);
const char *get_middle_image_resource(int &dataSizeInBytes);
const char *get_treble_image_resource(int &dataSizeInBytes);
const char *get_contour_image_resource(int &dataSizeInBytes);
const char *get_volume_image_resource(int &dataSizeInBytes);
const char *get_led_image_resource(int &dataSizeInBytes);
