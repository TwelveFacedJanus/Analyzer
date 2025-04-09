cmake_minimum_required(VERSION 3.26)
project(Analyzer LANGUAGES CXX)

# Use Ninja generator for module support
set(CMAKE_GENERATOR "Ninja" CACHE STRING "Generator to use" FORCE)

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

FetchContent_MakeAvailable(glfw)

# Module libraries
add_library(numbers_common)
target_sources(numbers_common
  PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules
    FILES modules/numbers_common.cppm
)

add_library(application)
target_sources(application
  PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules
    FILES modules/application.cppm
)

# Explicit module dependencies
target_link_libraries(application PRIVATE numbers_common)

# Main executable
add_executable(analyzer
    src/main.cpp
)

# Link dependencies
target_link_libraries(analyzer PRIVATE 
    application
    numbers_common
    glfw
)

# Include directories
target_include_directories(analyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# GLFW include (already handled by target_link_libraries)
# Modern CMake targets export their includes automatically