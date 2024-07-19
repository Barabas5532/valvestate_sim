add_subdirectory(pd)

juce_add_plugin(valvestate_plugin_pure_data
    COMPANY_NAME "Barabas Raffai"
    PLUGIN_MANUFACTURER_CODE BaRa
    PLUGIN_CODE VSPD
    FORMATS VST3 Standalone
    PRODUCT_NAME "Valvestate Pure Data")

target_sources(valvestate_plugin_pure_data
    PRIVATE
    src/CreatePluginFilter.cpp
    src/Processor.cpp
        )

target_include_directories(valvestate_plugin_pure_data
    PRIVATE
        dsp
        )

target_link_libraries(valvestate_plugin_pure_data
    PRIVATE
        valvestate::core
        valvestate::juce)
