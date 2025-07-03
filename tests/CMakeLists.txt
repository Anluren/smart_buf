# Tests CMakeLists.txt

# Create test executable
add_executable(smartbuffer_test test.cpp)

# Link with the SmartBuffer library
target_link_libraries(smartbuffer_test PRIVATE SmartBuffer::smart_buffer)

# Set target properties
set_target_properties(smartbuffer_test PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add compiler warnings
target_compile_options(smartbuffer_test PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Add test to CTest
add_test(NAME SmartBufferUnitTests COMMAND smartbuffer_test)
