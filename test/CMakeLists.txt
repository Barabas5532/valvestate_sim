cmake_minimum_required(VERSION 3.22)
project (ValvestateTest LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  test_clipping
  test_clipping.cpp
)
target_link_libraries(
  test_clipping
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_clipping)