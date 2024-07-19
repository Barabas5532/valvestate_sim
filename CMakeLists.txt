cmake_minimum_required(VERSION 3.15)
project(Valvestate VERSION 0.0.1)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(thirdparty)

add_subdirectory(core)

add_subdirectory(plugins)
