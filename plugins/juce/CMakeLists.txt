juce_add_plugin(valvestate_plugin_juce
    COMPANY_NAME "Barabas Raffai"
    PLUGIN_MANUFACTURER_CODE BaRa
    PLUGIN_CODE Valv
    FORMATS VST3 Standalone
    PRODUCT_NAME "Valvestate JUCE")

target_sources(valvestate_plugin_juce
    PRIVATE
        src/Clipping.cpp
        src/Contour.cpp
        src/CreatePluginFilter.cpp
        src/FMV.cpp
        src/GainControl.cpp
        src/InputFilter.cpp
        src/Valvestate.cpp
        src/WaveShape.cpp)

target_link_libraries(valvestate_plugin_juce
    PRIVATE
        valvestate::core
        valvestate::juce)
