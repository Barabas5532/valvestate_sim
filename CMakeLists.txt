cmake_minimum_required(VERSION 3.15)
project(Valvestate VERSION 0.0.1)

# VST3 build must use PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CTest)

add_subdirectory(JUCE)
add_subdirectory(Images)
add_subdirectory(Source)
add_subdirectory(test)

juce_add_plugin(Valvestate
    BUNDLE_ID "me.barabas.valvestate"
    COMPANY_NAME "Barabas Raffai"
    PLUGIN_MANUFACTURER_CODE BaRa
    PLUGIN_CODE Valv
    FORMATS VST3 Standalone
    PRODUCT_NAME "HYBRID 8100")

target_link_libraries(Valvestate PUBLIC valvestate::lib)
