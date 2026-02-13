cmake_minimum_required(VERSION 3.10.0)
project(lab0a VERSION 0.1.0 LANGUAGES C CXX)


add_executable(lab0a 
    main.cpp 
    src/module1/module1.cpp
    src/module1/module1.h
    src/module2/module2.cpp
    src/module2/module2.h
)

target_include_directories(lab0a PUBLIC 
    src/module1 
    src/module2
)