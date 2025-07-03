# CMake Build System for SmartBuffer

This directory contains a modern CMake build system for the SmartBuffer library.

## Quick Start

```bash
# Build everything with default settings
./build.sh

# Or manually
mkdir build && cd build
cmake ..
cmake --build .
ctest
```

## Build Targets

- **smart_buffer** - Header-only library target
- **smartbuffer_example** - Example application
- **smartbuffer_test** - Unit tests
- **smartbuffer_benchmark** - Performance benchmarks

## CMake Options

- `SMARTBUFFER_BUILD_EXAMPLES` - Build examples (default: ON)
- `SMARTBUFFER_BUILD_TESTS` - Build unit tests (default: ON)
- `SMARTBUFFER_BUILD_BENCHMARKS` - Build benchmarks (default: ON)

## Installation

```bash
cmake --install build --prefix /usr/local
```

## Integration in Other Projects

```cmake
# As subdirectory
add_subdirectory(smart_buf)
target_link_libraries(your_target PRIVATE SmartBuffer::smart_buffer)

# With FetchContent
include(FetchContent)
FetchContent_Declare(SmartBuffer
    GIT_REPOSITORY https://github.com/Anluren/smart_buf.git
    GIT_TAG main)
FetchContent_MakeAvailable(SmartBuffer)
target_link_libraries(your_target PRIVATE SmartBuffer::smart_buffer)
```

## Files

- `CMakeLists.txt` - Main CMake configuration
- `cmake/SmartBufferConfig.cmake.in` - Package config template
- `build.sh` - Convenient build script with options
