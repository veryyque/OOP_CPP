cmake_minimum_required(VERSION 3.10.0)
project(lab1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab1 main.cpp BitArray.cpp BitArray.h)

add_executable(lab1_tests test.cpp BitArray.cpp BitArray.h)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(lab1_tests PRIVATE gtest_main)

enable_testing()