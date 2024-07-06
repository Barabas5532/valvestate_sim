cmake_minimum_required(VERSION 3.26)

project(puredata_externals)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(../../thirdparty/pd.build/pd.cmake)

set_pd_sources(../../thirdparty/pure-data/src)
set_pd_external_path(${PROJECT_SOURCE_DIR}/binaries/)
set(PD_FLOATSIZE64 OFF)

add_library(iir STATIC)
target_sources(iir PRIVATE ./IIR-Filter/iir/src/iir.c)
target_include_directories(iir PUBLIC ./IIR-Filter/iir/src)

add_pd_external(iir3 iir3~ "")
target_sources(iir3 PRIVATE iir3.c)
target_link_libraries(iir3 PRIVATE iir)

# add_pd_external(iir4_project iir4~)
