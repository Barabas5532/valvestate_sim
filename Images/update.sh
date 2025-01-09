#!/usr/bin/env bash

set -eo pipefail

if [[ $# -eq 0 ]] ; then
    echo 'Input directory name is required'
    exit 0
fi

input_dir=$1

cp $1/Image0000.png Images/background.png

declare -a knobs=("bass" "contour" "gain" "middle" "treble" "volume")

for knob in "${knobs[@]}"
do
  echo "$knob"
  magick montage -mode concatenate -background transparent -tile 1x $1/$knob/* Images/$knob.png
done

# Copy images from render output to project, and fix alpha channel data

# left pad with zeros to 4 characters
#string(LENGTH "${knob_index}" length)
#set(padded_knob_index "${knob_index}")
#while(length LESS 4)
#    set(padded_knob_index "0${padded_knob_index}")
#    math(EXPR length "${length} + 1")
#endwhile()
#
#set(knob_file "${knob_name}/Image${padded_knob_index}.png")
#set(out_file "${CMAKE_CURRENT_BINARY_DIR}/${knob_name}/Image${padded_knob_index}.png")
#add_custom_command(OUTPUT ${out_file}
#                   COMMAND ${IMAGEMAGICK_CONVERT} ${CMAKE_CURRENT_LIST_DIR}/${knob_file} -channel A -gamma 0.45 +channel ${out_file}
#                   MAIN_DEPENDENCY ${knob_file}
#)

