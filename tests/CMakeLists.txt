cmake_minimum_required (VERSION 3.8)
project(SIMPLE_UUID_TESTER)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(EXE_SOURCES UltraSimpleUUID.h ConversionTest.cpp)

add_executable(SIMPLE_UUID_TESTER ${EXE_SOURCES})