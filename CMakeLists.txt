# SmartBuffer - Header-Only C++ Buffer Library
# Main CMakeLists.txt for building examples, tests, and benchmarks

cmake_minimum_required(VERSION 3.12)
project(SmartBuffer VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 as the default standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(SMARTBUFFER_BUILD_EXAMPLES "Build SmartBuffer examples" ON)
option(SMARTBUFFER_BUILD_TESTS "Build SmartBuffer tests" ON)
option(SMARTBUFFER_BUILD_BENCHMARKS "Build SmartBuffer benchmarks" ON)
option(SMARTBUFFER_ENABLE_INSTALL "Enable installation of SmartBuffer" ON)

# Add the header-only library
add_subdirectory(include)

# Build examples
if(SMARTBUFFER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Build tests
if(SMARTBUFFER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Build benchmarks
if(SMARTBUFFER_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Print configuration summary
message(STATUS "SmartBuffer configuration:")
message(STATUS "  Build examples: ${SMARTBUFFER_BUILD_EXAMPLES}")
message(STATUS "  Build tests: ${SMARTBUFFER_BUILD_TESTS}")
message(STATUS "  Build benchmarks: ${SMARTBUFFER_BUILD_BENCHMARKS}")
message(STATUS "  Enable install: ${SMARTBUFFER_ENABLE_INSTALL}")
