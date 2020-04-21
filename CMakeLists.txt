# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.1)

project (mirai-cpp)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif(CMAKE_TOOLCHAIN_FILE)

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
#find_package(boost REQUIRED COMPONENTS asio)

#find_path(BOOST_INCLUDE_DIR boost)
#include_directories(${BOOST_INCLUDE_DIR})

if (WIN32)
	set(BOOST_LIBRARIES ${Boost_LIBRARY_DIRS}/boost_system-vc140-mt.lib)
	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		set(BOOST_LIBRARIES ${Boost_LIBRARY_DIRS}/../debug/lib/boost_system-vc140-mt-gd.lib)
	endif()
endif()

if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    message(STATUS "optional: /UTF-8")   
endif(MSVC)

option(BUILD_SHARED_LIBS "Build ${PROJECT_NAME} as a shared library." ON)
if(BUILD_SHARED_LIBS)
  set(LIBRARY_TYPE SHARED)
else()
  set(LIBRARY_TYPE STATIC)
endif()

aux_source_directory(./src SRCS)

add_library(${PROJECT_NAME} ${LIBRARY_TYPE} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} CURL::libcurl)
#target_link_libraries(${PROJECT_NAME} INTERFACE ${BOOST_LIBRARIES})

option(MIRAI_CPP_BUILD_EXAMPLES "Build examples" ON)
if(MIRAI_CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(MIRAI_CPP_BUILD_EXAMPLES)

option(MIRAI_CPP_INSTALL "INSTALL_MIRAI_CPP" ON)
if(MIRAI_CPP_INSTALL)

    install(
        DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
endif(MIRAI_CPP_INSTALL)
