cmake_minimum_required(VERSION 3.1)
project(unit_test)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_definitions(-DUSE_GTEST)
add_definitions(-D_VARIADIC_MAX=10)

add_executable(unit_test main.cpp)
target_link_libraries(unit_test mirai-cpp)
target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(unit_test pthread)
endif()

#  π”√ mirai-cpp æ≤Ã¨ø‚
target_compile_definitions(unit_test PRIVATE MIRAICPP_STATICLIB)

add_test( unit_test unit_test )
#gtest_discover_tests(unit_test unit_test)
gtest_add_tests(unit_test unit_test AUTO)