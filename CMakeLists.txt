cmake_minimum_required(VERSION 3.15)
project(Valvestate VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(JUCE)

juce_add_plugin(Valvestate
    COMPANY_NAME "Barabas Raffai"
    PLUGIN_MANUFACTURER_CODE BaRa
    PLUGIN_CODE Valv
    FORMATS VST3 Standalone
    PRODUCT_NAME "HYBRID 8100")

juce_generate_juce_header(Valvestate)

target_sources(Valvestate
    PRIVATE
        Source/Clipping.cpp
        Source/Contour.cpp
        Source/FMV.cpp
        Source/GainControl.cpp
        Source/InputFilter.cpp
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/WaveShape.cpp)


target_include_directories(Valvestate
    PRIVATE
        Source)

target_compile_definitions(Valvestate
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Valvestate
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
