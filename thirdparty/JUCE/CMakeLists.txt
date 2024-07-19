add_subdirectory(JUCE)

add_library(valvestate_juce STATIC)
add_library(valvestate::juce ALIAS valvestate_juce)

target_link_libraries(valvestate_juce
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)

target_compile_definitions(valvestate_juce
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_STANDALONE_APPLICATION=JucePlugin_Build_Standalone
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_WEB_BROWSER=0
        JucePlugin_Build_Standalone=1
    INTERFACE
        $<TARGET_PROPERTY:valvestate_juce,COMPILE_DEFINITIONS>)

target_include_directories(valvestate_juce
    INTERFACE
        $<TARGET_PROPERTY:valvestate_juce,INCLUDE_DIRECTORIES>)
