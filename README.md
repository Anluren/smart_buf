# SmartBuffer - Header-Only C++ Buffer Library

A template-based smart buffer library that automatically chooses between static and dynamic allocation based on buffer size.

## Features

- **Header-only**: Just include `smart_buffer.hpp` and you're ready to go
- **Automatic allocation strategy**: 
  - Buffers ≤ 32 bytes use static allocation (`std::array`)
  - Buffers > 32 bytes use dynamic allocation (`std::unique_ptr`)
- **Seamless interface**: `operator uint8_t*` provides direct buffer access
- **C++17 compatible**: Uses `if constexpr` for clean, efficient compile-time branching
- **Zero-overhead**: Minimal runtime cost for static buffers
- **Complete**: Copy/move constructors, assignment operators, and comprehensive API

## Quick Start

```cpp
#include "smart_buffer.hpp"

// Static allocation (≤ 32 bytes)
SmartBuffer<16> small_buffer;    // Uses std::array internally
small_buffer[0] = 0xFF;

// Dynamic allocation (> 32 bytes)
SmartBuffer<1024> large_buffer;  // Uses std::unique_ptr internally
large_buffer[100] = 0xAB;

// Automatic conversion to uint8_t*
uint8_t* ptr = small_buffer;     // Direct access to buffer data
memset(large_buffer, 0, large_buffer.size());
```

## API Reference

### Constructor
```cpp
SmartBuffer<Size> buffer;        // Zero-initialized buffer
```

### Core Operations
```cpp
operator uint8_t*();             // Convert to mutable pointer
operator const uint8_t*() const; // Convert to const pointer
uint8_t* data();                 // Get mutable pointer
const uint8_t* data() const;     // Get const pointer
```

### Array Access
```cpp
uint8_t& operator[](size_t index);       // Mutable element access
const uint8_t& operator[](size_t index) const; // Const element access
```

### Utility Methods
```cpp
constexpr size_t size() const noexcept;  // Get buffer size
constexpr bool is_static() const noexcept; // Check allocation type
void fill(uint8_t value);               // Fill with value
void clear();                           // Fill with zeros
```

### Copy/Move Semantics
```cpp
SmartBuffer(const SmartBuffer& other);           // Copy constructor
SmartBuffer(SmartBuffer&& other) noexcept;       // Move constructor
SmartBuffer& operator=(const SmartBuffer& other); // Copy assignment
SmartBuffer& operator=(SmartBuffer&& other) noexcept; // Move assignment
```

## Type Aliases

Convenient predefined types for common buffer sizes:

```cpp
SmartBuffer8    // SmartBuffer<8>     - static
SmartBuffer16   // SmartBuffer<16>    - static  
SmartBuffer32   // SmartBuffer<32>    - static (boundary case)
SmartBuffer64   // SmartBuffer<64>    - dynamic
SmartBuffer128  // SmartBuffer<128>   - dynamic
SmartBuffer256  // SmartBuffer<256>   - dynamic
SmartBuffer512  // SmartBuffer<512>   - dynamic
SmartBuffer1K   // SmartBuffer<1024>  - dynamic
SmartBuffer2K   // SmartBuffer<2048>  - dynamic
SmartBuffer4K   // SmartBuffer<4096>  - dynamic
```

## Examples

### Basic Usage
```cpp
#include "smart_buffer.hpp"
#include <cstring>

SmartBuffer<64> buffer;

// Fill with pattern
buffer.fill(0xAA);

// Use with C functions
memset(buffer, 0, buffer.size());
strcpy(reinterpret_cast<char*>(buffer.data()), "Hello");

// Array-style access
buffer[0] = 0x42;
buffer[63] = 0xFF;
```

### Working with Different Sizes
```cpp
// Small buffer - uses static allocation
SmartBuffer<16> small;
assert(small.is_static() == true);

// Large buffer - uses dynamic allocation  
SmartBuffer<512> large;
assert(large.is_static() == false);

// Boundary case - still static
SmartBuffer<32> boundary;
assert(boundary.is_static() == true);
```

### Copy and Move Operations
```cpp
SmartBuffer<128> original;
original.fill(0x55);

// Copy constructor
SmartBuffer<128> copy(original);
assert(copy[10] == 0x55);

// Move constructor (efficient for large buffers)
SmartBuffer<128> moved(std::move(original));
assert(moved[10] == 0x55);
```

## Performance Characteristics

| Buffer Size | Allocation Type | Performance Notes |
|-------------|----------------|-------------------|
| ≤ 32 bytes  | Static (stack) | Zero allocation overhead, compile-time size |
| > 32 bytes  | Dynamic (heap) | Single allocation, automatic cleanup |

## Compilation

The library requires C++17 or later and uses these standard headers:
- `<cstdint>` - for `uint8_t`
- `<memory>` - for `std::unique_ptr` and `std::make_unique`
- `<array>` - for `std::array`
- `<type_traits>` - for SFINAE and `std::conditional`
- `<algorithm>` - for `std::fill` and `std::copy`
- `<cstring>` - for compatibility with C string functions

### Why C++17?
- **`if constexpr`**: Enables compile-time conditional compilation without SFINAE complexity
- **`std::conditional_t`**: Cleaner type alias syntax
- **Better template metaprogramming**: Simplified code that's easier to read and maintain

### Build Systems

#### CMake (Recommended)
```bash
# Quick build with default settings
./build.sh

# Or manually:
mkdir build && cd build
cmake ..
cmake --build .

# Run tests
ctest

# Custom build options
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DSMARTBUFFER_BUILD_EXAMPLES=ON \
         -DSMARTBUFFER_BUILD_TESTS=ON \
         -DSMARTBUFFER_BUILD_BENCHMARKS=ON
```

#### Direct Compilation
```bash
g++ -std=c++17 -O2 example.cpp -o example
clang++ -std=c++17 -O2 example.cpp -o example
```

#### Makefile (Legacy)
```bash
make run      # Build and run example
make test     # Build and run tests
make benchmark # Build and run benchmarks
```

### CMake Integration

To use SmartBuffer in your CMake project:

```cmake
# Method 1: Add as subdirectory
add_subdirectory(path/to/smart_buf)
target_link_libraries(your_target PRIVATE SmartBuffer::smart_buffer)

# Method 2: Use find_package (after installation)
find_package(SmartBuffer REQUIRED)
target_link_libraries(your_target PRIVATE SmartBuffer::smart_buffer)

# Method 3: FetchContent
include(FetchContent)
FetchContent_Declare(
    SmartBuffer
    GIT_REPOSITORY https://github.com/Anluren/smart_buf.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(SmartBuffer)
target_link_libraries(your_target PRIVATE SmartBuffer::smart_buffer)
```

## Design Rationale

### Why 32 bytes as the threshold?
- Typical cache line size consideration
- Balance between stack usage and dynamic allocation overhead
- Small enough for embedded systems, large enough for most small data structures

### Why template-based size?
- Compile-time buffer size determination
- Enables optimal allocation strategy selection
- Zero runtime overhead for size queries

### Why SFINAE over `if constexpr`?
- C++17 requirement for `if constexpr`
- Modern, clean syntax that's easier to understand
- Better compiler error messages
- Maintains the same performance characteristics

## Thread Safety

- **Construction/Destruction**: Not thread-safe (like standard containers)
- **Data Access**: Thread-safe for read-only access from multiple threads
- **Modification**: Requires external synchronization for concurrent writes

## License

This is a header-only library. Feel free to use it in your projects.

## Building

### Quick Start with CMake
```bash
# Clone and build
git clone https://github.com/Anluren/smart_buf.git
cd smart_buf
./build.sh

# Run examples and tests
cd build
./smartbuffer_example
./smartbuffer_test
./smartbuffer_benchmark
```

### Build Options
The build script supports various options:
```bash
./build.sh --help                    # Show all options
./build.sh -t Debug -c               # Clean debug build
./build.sh --no-benchmarks -r        # Build without benchmarks, run tests
./build.sh -t Release -i             # Release build with installation
```

### Manual CMake
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
ctest                                # Run tests
```
