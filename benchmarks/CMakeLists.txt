# Benchmarks CMakeLists.txt

# Create benchmark executable
add_executable(smartbuffer_benchmark benchmark.cpp)

# Link with the SmartBuffer library
target_link_libraries(smartbuffer_benchmark PRIVATE SmartBuffer::smart_buffer)

# Set target properties
set_target_properties(smartbuffer_benchmark PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add compiler warnings and optimization flags for benchmarks
target_compile_options(smartbuffer_benchmark PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -O3>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -O3>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /O2>
)
