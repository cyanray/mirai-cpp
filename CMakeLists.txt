# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.1)

project (mirai-cpp)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} INTERFACE CURL::libcurl)

option(MIRAI_CPP_BUILD_EXAMPLES "Build examples" ON)
if(MIRAI_CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(MIRAI_CPP_BUILD_EXAMPLES)
