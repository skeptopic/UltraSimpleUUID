cmake_minimum_required (VERSION 3.8)
project(UltraSimpleUUIDTests)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(DEEP_TEST_ULTRA_SIMPLE_UUIDS)
SET(EXE_SOURCES ../UltraSimpleUUID.h AccuracyTests.cpp)
add_executable(AccuracyTests ${EXE_SOURCES})

#remove_definitions(DEEP_TEST_ULTRA_SIMPLE_UUIDS)
#SET(EXE_SOURCES ../UltraSimpleUUID.h SpeedTests.cpp)
#add_executable(SpeedTests ${EXE_SOURCES})

remove_definitions(DEEP_TEST_ULTRA_SIMPLE_UUIDS)
SET(EXE_SOURCES ../UltraSimpleUUID.h CollisionTests.cpp)
add_executable(CollisionTests ${EXE_SOURCES})

