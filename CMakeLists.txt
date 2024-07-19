cmake_minimum_required(VERSION 3.15)
project(Valvestate VERSION 0.0.1)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(thirdparty)

add_subdirectory(core)

add_subdirectory(juce)
