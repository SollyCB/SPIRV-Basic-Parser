cmake_minimum_required(VERSION 3.2)
project(exe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR}/src)

set(GCC_COVERAGE_COMPILE_FLAGS
    "-std=c++17"
    "-g"
)

set(SOURCE_FILES
    src/Allocator.cpp
    src/File.cpp
    src/Spv.cpp
    src/tlsf.cpp
)
add_library(source ${SOURCE_FILES})

option(DEBUG ON)
if(DEBUG)
add_compile_definitions(DEBUG=true)
endif()

add_executable(exe src/main.cpp)

target_compile_options(exe PRIVATE "${GCC_COVERAGE_COMPILE_FLAGS}")
target_link_libraries(exe PRIVATE "${GCC_COVERAGE_LINK_FLAGS}" source)

option(BUILD_TESTS ON)
if (BUILD_TESTS)
    message("Building Tests...")
    include_directories(${CMAKE_SOURCE_DIR}/src/test)

    set(TEST_FILES test/spv_test.cpp test/test.cpp)
    add_executable(test_all test/test_entrypoint.cpp ${TEST_FILES})

    target_compile_options(test_all PRIVATE "${GCC_COVERAGE_COMPILE_FLAGS}" "-ggdb")
    target_link_libraries(test_all PRIVATE "${GCC_COVERAGE_LINK_FLAGS}" source)
endif()
