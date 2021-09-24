cmake_minimum_required (VERSION 3.1)

project (mirai-cpp VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)


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

if(BUILD_SHARED_LIBS)
    if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN_EXPORT)
    endif()
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE MIRAICPP_STATICLIB)
endif()

if (WIN32)
    target_link_libraries (${PROJECT_NAME} wsock32 ws2_32)
endif(WIN32)

option(MIRAI_CPP_BUILD_EXAMPLES "Build examples" OFF)
if(MIRAI_CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(MIRAI_CPP_BUILD_EXAMPLES)

option(MIRAI_CPP_BUILD_UNIT_TESTS "Build UnitTests" OFF)
if(MIRAI_CPP_BUILD_UNIT_TESTS)
    add_subdirectory(UnitTest)
endif(MIRAI_CPP_BUILD_UNIT_TESTS)

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
