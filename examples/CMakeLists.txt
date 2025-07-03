# Examples CMakeLists.txt

# Create example executable
add_executable(smartbuffer_example example.cpp)

# Link with the SmartBuffer library
target_link_libraries(smartbuffer_example PRIVATE SmartBuffer::smart_buffer)

# Set target properties
set_target_properties(smartbuffer_example PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add compiler warnings
target_compile_options(smartbuffer_example PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
