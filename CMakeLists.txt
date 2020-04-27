cmake_minimum_required (VERSION 3.1)

project (mirai-cpp VERSION 0.4.4)

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



option(BUILD_SHARED_LIBS "Build ${PROJECT_NAME} as a shared library." OFF)
if(BUILD_SHARED_LIBS)
  set(LIBRARY_TYPE SHARED)
else()
  set(LIBRARY_TYPE STATIC)
endif()

aux_source_directory(./src SRCS)

add_library(${PROJECT_NAME} ${LIBRARY_TYPE} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
target_link_libraries(${PROJECT_NAME} nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} CURL::libcurl)
#target_link_libraries(${PROJECT_NAME} INTERFACE ${BOOST_LIBRARIES})

if(BUILD_SHARED_LIBS)
    if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN_EXPORT)
    endif()
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE MIRAICPP_STATICLIB)
endif()



option(MIRAI_CPP_BUILD_EXAMPLES "Build examples" ON)
if(MIRAI_CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(MIRAI_CPP_BUILD_EXAMPLES)


option(MIRAI_CPP_INSTALL "INSTALL_MIRAI_CPP" ON)
if(MIRAI_CPP_INSTALL)

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            mirai-cppConfigVersion.cmake
            VERSION ${PACKAGE_VERSION}
            COMPATIBILITY AnyNewerVersion
            )

    install(
        DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
    
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}Targets
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
    )

    install(
        EXPORT ${PROJECT_NAME}Targets
        FILE mirai-cppTargets.cmake
 #       NAMESPACE MIRAICPP::
        DESTINATION lib/cmake/${PROJECT_NAME}
    )

    configure_file(mirai-cppConfig.cmake.in mirai-cppConfig.cmake @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mirai-cppConfig.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/mirai-cppConfigVersion.cmake"
            DESTINATION lib/cmake/mirai-cpp
            )
        
endif(MIRAI_CPP_INSTALL)
