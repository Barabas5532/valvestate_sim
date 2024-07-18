juce_add_plugin(Valvestate
    COMPANY_NAME "Barabas Raffai"
    PLUGIN_MANUFACTURER_CODE BaRa
    PLUGIN_CODE Valv
    FORMATS VST3 Standalone
    PRODUCT_NAME "HYBRID 8100")

juce_generate_juce_header(Valvestate)

target_sources(Valvestate
    PRIVATE
        src/Clipping.cpp
        src/Contour.cpp
        src/CreatePluginFilter.cpp
        src/FMV.cpp
        src/GainControl.cpp
        src/InputFilter.cpp
        src/Valvestate.cpp
        src/WaveShape.cpp
        ../core/Source/PluginEditor.cpp
        ../core/Source/PluginProcessor.cpp)

target_include_directories(Valvestate
    PRIVATE
        dsp
        ../core/Source)

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
