add_library(valvestate_core STATIC)
add_library(valvestate::core ALIAS valvestate_core)

target_sources(valvestate_core
    PRIVATE
    Source/PluginEditor.cpp
    Source/PluginProcessor.cpp)

target_include_directories(valvestate_core PUBLIC Source)

target_link_libraries(valvestate_core PUBLIC valvestate::juce)