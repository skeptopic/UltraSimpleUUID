cmake_minimum_required (VERSION 3.8)
project(UltraSimpleUUIDTests)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

SET(EXE_SOURCES ../UltraSimpleUUID.h UnitTest.cpp)
add_executable(UnitTest ${EXE_SOURCES})
target_compile_definitions(UnitTest PRIVATE DEEP_TEST_ULTRA_SIMPLE_UUIDS)
add_test(NAME UnitTesting COMMAND UnitTest)

SET(EXE_SOURCES ../UltraSimpleUUID.h RandomnessTest.cpp)
add_executable(RandomnessTest ${EXE_SOURCES})

SET(EXE_SOURCES ../UltraSimpleUUID.h PerformanceTests.cpp)
add_executable(PerformanceTests ${EXE_SOURCES})
