cmake_minimum_required (VERSION 3.8)
project(SimpleUUIDTest)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(EXE_SOURCES ../UltraSimpleUUID.h ConversionTest.cpp)

add_executable(SimpleUUIDTest ${EXE_SOURCES})