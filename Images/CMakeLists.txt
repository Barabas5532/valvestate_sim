cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(KNOB_IMAGE_COUNT 256)

juce_add_binary_data(ValvestateBackgroundBinaryData SOURCES
                     background.png
                     HEADER_NAME BackgroundBinaryData.h
                     NAMESPACE BackgroundBinaryData
)

find_program(IMAGEMAGICK_CONVERT magick convert REQUIRED)

function(add_knob_binary_data target knob_name)
set(knob_list "")
foreach(knob_index RANGE 1 ${KNOB_IMAGE_COUNT})
    # left pad with zeros to 4 characters
    string(LENGTH "${knob_index}" length)
    set(padded_knob_index "${knob_index}")
    while(length LESS 4)
        set(padded_knob_index "0${padded_knob_index}")
        math(EXPR length "${length} + 1")
    endwhile()

    set(knob_file "${knob_name}/Image${padded_knob_index}.png")
    set(out_file "${CMAKE_CURRENT_BINARY_DIR}/${knob_name}/Image${padded_knob_index}.png")
    add_custom_command(OUTPUT ${out_file}
                       COMMAND ${IMAGEMAGICK_CONVERT} ${CMAKE_CURRENT_LIST_DIR}/${knob_file} -channel A -gamma 0.45 +channel ${out_file}
                       MAIN_DEPENDENCY ${knob_file}
    )

    list(APPEND knob_list ${out_file})
endforeach()

message(WARNING "${knob_list}")

juce_add_binary_data(${target} SOURCES
                     ${knob_list}
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