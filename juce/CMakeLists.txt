juce_add_plugin(Valvestate
    COMPANY_NAME "Barabas Raffai"
    PLUGIN_MANUFACTURER_CODE BaRa
    PLUGIN_CODE Valv
    FORMATS VST3 Standalone
    PRODUCT_NAME "HYBRID 8100")

target_sources(Valvestate
    PRIVATE
        src/Clipping.cpp
        src/Contour.cpp
        src/CreatePluginFilter.cpp
        src/FMV.cpp
        src/GainControl.cpp
        src/InputFilter.cpp
        src/Valvestate.cpp
        src/WaveShape.cpp)

target_include_directories(Valvestate
    PRIVATE
        dsp
        ../core/Source)

target_link_libraries(Valvestate
    PRIVATE
        valvestate::core
        valvestate::juce)
