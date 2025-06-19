cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

juce_add_binary_data(ValvestateBackgroundBinaryData SOURCES
                     background.png
                     led.png
                     HEADER_NAME BackgroundBinaryData.h
                     NAMESPACE BackgroundBinaryData
)

find_program(IMAGEMAGICK_CONVERT magick convert REQUIRED)

function(add_knob_binary_data target knob_name)
set(knob_file "${knob_name}.png")
set(out_file "${CMAKE_CURRENT_BINARY_DIR}/${knob_name}.png")
add_custom_command(OUTPUT ${out_file}
                   COMMAND ${IMAGEMAGICK_CONVERT} ${CMAKE_CURRENT_LIST_DIR}/${knob_file} -channel A -gamma 0.45 +channel ${out_file}
                   MAIN_DEPENDENCY ${knob_file}
)

juce_add_binary_data(${target} SOURCES
                     ${out_file}
                     HEADER_NAME "${knob_name}_binary_data.h"
                     NAMESPACE "${knob_name}_binary_data"
)
endfunction()

add_knob_binary_data(ValvestateBassBinaryData bass)
add_knob_binary_data(ValvestateContourBinaryData contour)
add_knob_binary_data(ValvestateGainBinaryData gain)
add_knob_binary_data(ValvestateMiddleBinaryData middle)
add_knob_binary_data(ValvestateTrebleBinaryData treble)
add_knob_binary_data(ValvestateVolumeBinaryData volume)