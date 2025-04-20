cmake_minimum_required(VERSION 3.26)
project(Analyzer LANGUAGES CXX)

# Set policies to ensure compatibility with dependencies
cmake_policy(VERSION 3.26)

# Standard and compiler settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Clang-specific settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Use separate variables for clarity
    set(CLANG_FLAGS "-stdlib=libc++ -fmodules")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_FLAGS}")
    
    # For better diagnostics
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Enable module support
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# GLFW configuration
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

# GLFW build options
option(GLFW_BUILD_WAYLAND "Build Wayland support" OFF)
option(GLFW_BUILD_EXAMPLES "Build GLFW examples" OFF)
option(GLFW_BUILD_TESTS "Build GLFW tests" OFF)
option(GLFW_BUILD_DOCS "Build GLFW documentation" OFF)

# GLAD configuration - modified to work with newer CMake
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    # Override the minimum version requirement
    OVERRIDE_FIND_PACKAGE
)

# Set GLAD options before making it available
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=4.6" CACHE STRING "OpenGL API version")
set(GLAD_GENERATOR "c++" CACHE STRING "GLAD generator")

# Make dependencies available
FetchContent_MakeAvailable(glfw glad)

# Main executable
add_executable(analyzer
    src/main.cpp
)

# Link dependencies
target_link_libraries(analyzer PRIVATE 
    glfw
    glad
)

# Include directories
target_include_directories(analyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(analyzer PRIVATE OpenGL::GL)