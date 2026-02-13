cmake_minimum_required(VERSION 3.10)
project(SoundProcessor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sound_processor
    src/main.cpp
    src/Parser.cpp
    src/WAVReader.cpp
    src/WAVWriter.cpp
    src/ConfigParser.cpp
    src/ConverterFactory.cpp
    src/MuteConverter.cpp
    src/MixConverter.cpp
    src/RobotVoiceConverter.cpp
)

add_executable(tests
    src/test.cpp
    src/Parser.cpp
    src/WAVReader.cpp
    src/WAVWriter.cpp
    src/ConfigParser.cpp
    src/ConverterFactory.cpp
    src/MuteConverter.cpp
    src/MixConverter.cpp
    src/RobotVoiceConverter.cpp
)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(tests PRIVATE gtest_main)

enable_testing()