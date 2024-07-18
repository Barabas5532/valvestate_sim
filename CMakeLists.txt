cmake_minimum_required(VERSION 3.15)
project(Valvestate VERSION 0.0.1)

add_subdirectory(thirdparty/JUCE)

# TODO set up a JUCE static library to link to, and use it in core and the
# other projects. Right now, every project hardcodes paths to files in core to
# use them.
add_subdirectory(core)
add_subdirectory(juce)
