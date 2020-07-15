cmake_minimum_required(VERSION 3.0)
project(unit_test)

# GTest
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Unit Tests
# Add test cpp file
add_executable(unit_test main.cpp)

target_link_libraries(unit_test mirai-cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})
add_test( unit_test unit_test )