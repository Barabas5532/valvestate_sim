add_library(valvestate_lib STATIC)
add_library(valvestate::lib ALIAS valvestate_lib )

target_sources(valvestate_lib
               PRIVATE
               PluginProcessor.cpp
               dsp/Clipping.cpp
               dsp/Contour.cpp
               dsp/FMV.cpp
               dsp/GainControl.cpp
               dsp/InputFilter.cpp
               dsp/WaveShape.cpp
               ui/PluginEditor.cpp)

target_include_directories(valvestate_lib PRIVATE .)

target_link_libraries(valvestate_lib
                      PRIVATE
                      ValvestateBackgroundBinaryData
                      ValvestateBassBinaryData
                      ValvestateContourBinaryData
                      ValvestateGainBinaryData
                      ValvestateGainBinaryData
                      ValvestateMiddleBinaryData
                      ValvestateTrebleBinaryData
                      ValvestateVolumeBinaryData
                      PUBLIC
                      valvestate::juce_modules
                      juce::juce_recommended_config_flags
                      juce::juce_recommended_lto_flags
                      juce::juce_recommended_warning_flags)
