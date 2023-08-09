cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(SlugVk-lib-common)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS_SLUGVK_COMMON}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINK_FLAGS_SLUGVK_COMMON}")
set(LINK_FLAGS_SLUGVK_COMMON
    "-lglfw"
    "-lvulkan"
    "-ldl"
    "-lpthread"
    "-lX11"
    "-lXxf86vm"
    "-lXrandr"
    "-lXi"
)

option(BUILD_DEBUG ON)
option(BUILD_TESTS ON)

if(BUILD_DEBUG)
    message("Building DEBUG...")
    set(CMAKE_BUILD_TYPE DEBUG)
    add_compile_options(
        -std=c++17
        -g
    )
    add_compile_definitions(DEBUG=true)
else()
    message("Building RELEASE...")
    set(CMAKE_BUILD_TYPE RELEASE)
    add_compile_options(
        -std=c++17
        -O3
    )
    add_compile_definitions(DEBUG=false)
endif()

add_library(SlugVk-lib-common
    Allocator.cpp
    Camera.cpp
    Clock.cpp
    CMakeLists.txt
    File.cpp
    glTF.cpp
    Image.cpp
    QuartCamera.cpp
    Spv.cpp
    String.cpp
    VulkanErrors.cpp
    Window.cpp

    external/tlsf.cpp
)
target_include_directories(SlugVk-lib-common PUBLIC . ./external ./external/nlohmann)
target_compile_options(SlugVk-lib-common PUBLIC "${COMPILE_FLAGS_SLUGVK_COMMON}")
target_link_libraries(SlugVk-lib-common PUBLIC "${LINK_FLAGS_SLUGVK_COMMON}")

if (BUILD_TESTS)
    message("Building TESTS...")

    add_executable(SlugVk-lib-common-TestAll
        test/test_entrypoint.cpp
        test/test.cpp
        test/spv_test.cpp
        test/gltf_test.cpp
    )
    target_include_directories(SlugVk-lib-common-TestAll PRIVATE .)
    target_compile_options(SlugVk-lib-common-TestAll PRIVATE "${COMPILE_FLAGS_SLUGVK_COMMON}")
    target_link_libraries(SlugVk-lib-common-TestAll PRIVATE SlugVk-lib-common)
endif()
