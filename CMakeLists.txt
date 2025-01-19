cmake_minimum_required(VERSION 3.15)
project(Valvestate VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(JUCE)

juce_add_plugin(Valvestate
    BUNDLE_ID "me.barabas.valvestate"
    COMPANY_NAME "Barabas Raffai"
    PLUGIN_MANUFACTURER_CODE BaRa
    PLUGIN_CODE Valv
    FORMATS VST3 Standalone
    PRODUCT_NAME "HYBRID 8100")

target_sources(Valvestate
    PRIVATE
        Source/PluginProcessor.cpp
        Source/dsp/Clipping.cpp
        Source/dsp/Contour.cpp
        Source/dsp/FMV.cpp
        Source/dsp/GainControl.cpp
        Source/dsp/InputFilter.cpp
        Source/dsp/WaveShape.cpp
        Source/ui/PluginEditor.cpp)


target_include_directories(Valvestate
    PRIVATE
        Source)

target_compile_definitions(Valvestate
    PUBLIC
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
