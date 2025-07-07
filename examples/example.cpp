#include <smart_buffer.hpp>
#include <iostream>
#include <cassert>
#include <cstring>

void test_alignment() {
    std::cout << "=== Testing 8-Byte Alignment ===" << std::endl;
    
    // Test various sizes to show alignment
    SmartBuffer<1> buffer1;
    SmartBuffer<5> buffer5;
    SmartBuffer<8> buffer8;
    SmartBuffer<9> buffer9;
    SmartBuffer<15> buffer15;
    SmartBuffer<16> buffer16;
    SmartBuffer<17> buffer17;
    
    std::cout << "Requested size: 1,  Actual size: " << buffer1.actual_size() << std::endl;
    std::cout << "Requested size: 5,  Actual size: " << buffer5.actual_size() << std::endl;
    std::cout << "Requested size: 8,  Actual size: " << buffer8.actual_size() << std::endl;
    std::cout << "Requested size: 9,  Actual size: " << buffer9.actual_size() << std::endl;
    std::cout << "Requested size: 15, Actual size: " << buffer15.actual_size() << std::endl;
    std::cout << "Requested size: 16, Actual size: " << buffer16.actual_size() << std::endl;
    std::cout << "Requested size: 17, Actual size: " << buffer17.actual_size() << std::endl;
    
    // Check alignment of buffer addresses
    std::cout << "\nBuffer address alignment check:" << std::endl;
    std::cout << "buffer1 address: " << static_cast<void*>(buffer1.data()) 
              << " (aligned: " << (reinterpret_cast<uintptr_t>(buffer1.data()) % 8 == 0 ? "Yes" : "No") << ")" << std::endl;
    std::cout << "buffer17 address: " << static_cast<void*>(buffer17.data()) 
              << " (aligned: " << (reinterpret_cast<uintptr_t>(buffer17.data()) % 8 == 0 ? "Yes" : "No") << ")" << std::endl;
    
    std::cout << std::endl;
}

void test_static_buffer() {
    std::cout << "=== Testing Static Buffer (32 bytes) ===" << std::endl;
    
    SmartBuffer<32> buffer;
    
    // Test basic properties
    std::cout << "Requested size: " << buffer.size() << " bytes" << std::endl;
    std::cout << "Actual size: " << buffer.actual_size() << " bytes" << std::endl;
    std::cout << "Is static: " << (buffer.is_static() ? "Yes" : "No") << std::endl;
    std::cout << "Static threshold: " << buffer.static_threshold() << " bytes" << std::endl;
    
    // Test operator uint8_t*
    uint8_t* ptr = buffer;
    std::cout << "Buffer address: " << static_cast<void*>(ptr) << std::endl;
    
    // Test array access
    buffer[0] = 0xAA;
    buffer[1] = 0xBB;
    buffer[31] = 0xFF;
    
    std::cout << "buffer[0] = 0x" << std::hex << static_cast<int>(buffer[0]) << std::endl;
    std::cout << "buffer[1] = 0x" << std::hex << static_cast<int>(buffer[1]) << std::endl;
    std::cout << "buffer[31] = 0x" << std::hex << static_cast<int>(buffer[31]) << std::endl;
    
    // Test fill vs fill_all
    buffer.fill(0x55);
    std::cout << "After fill(0x55), buffer[10] = 0x" << std::hex << static_cast<int>(buffer[10]) << std::endl;
    
    buffer.clear();
    std::cout << "After clear(), buffer[10] = 0x" << std::hex << static_cast<int>(buffer[10]) << std::endl;
    
    // Test copy constructor
    buffer[5] = 0x99;
    SmartBuffer<32> buffer_copy(buffer);
    std::cout << "Copy constructor: buffer_copy[5] = 0x" << std::hex << static_cast<int>(buffer_copy[5]) << std::endl;
    
    std::cout << std::dec << std::endl;
}

void test_dynamic_buffer() {
    std::cout << "=== Testing Dynamic Buffer (1024 bytes) ===" << std::endl;
    
    SmartBuffer<1024> buffer;
    
    // Test basic properties
    std::cout << "Requested size: " << buffer.size() << " bytes" << std::endl;
    std::cout << "Actual size: " << buffer.actual_size() << " bytes" << std::endl;
    std::cout << "Is static: " << (buffer.is_static() ? "Yes" : "No") << std::endl;
    std::cout << "Static threshold: " << buffer.static_threshold() << " bytes" << std::endl;
    
    // Test operator uint8_t*
    uint8_t* ptr = buffer;
    std::cout << "Buffer address: " << static_cast<void*>(ptr) << std::endl;
    
    // Test array access
    buffer[0] = 0xCC;
    buffer[1023] = 0xDD;
    
    std::cout << "buffer[0] = 0x" << std::hex << static_cast<int>(buffer[0]) << std::endl;
    std::cout << "buffer[1023] = 0x" << std::hex << static_cast<int>(buffer[1023]) << std::endl;
    
    // Test fill
    buffer.fill(0x77);
    std::cout << "After fill(0x77), buffer[500] = 0x" << std::hex << static_cast<int>(buffer[500]) << std::endl;
    
    std::cout << std::dec << std::endl;
}

void test_boundary_case() {
    std::cout << "=== Testing Boundary Case (32 bytes with default threshold) ===" << std::endl;
    
    SmartBuffer<32> buffer32_default;  // Should be static (32 <= 32)
    SmartBuffer<33> buffer33_default;  // Should be dynamic (40 > 32, since 33 rounds up to 40)
    
    std::cout << "32-byte buffer: requested=" << buffer32_default.size() 
              << ", actual=" << buffer32_default.actual_size()
              << ", static=" << (buffer32_default.is_static() ? "Yes" : "No") << std::endl;
    std::cout << "33-byte buffer: requested=" << buffer33_default.size() 
              << ", actual=" << buffer33_default.actual_size()
              << ", static=" << (buffer33_default.is_static() ? "Yes" : "No") << std::endl;
              
    std::cout << std::endl;
}

void test_custom_thresholds() {
    std::cout << "=== Testing Custom Thresholds ===" << std::endl;
    
    // Test with 64-byte threshold
    SmartBuffer<64, 64> buffer64_thresh64;   // Should be static (64 <= 64)
    SmartBuffer<65, 64> buffer65_thresh64;   // Should be dynamic (72 > 64, since 65 rounds up to 72)
    
    std::cout << "64-byte buffer with 64-byte threshold: " 
              << "requested=" << buffer64_thresh64.size()
              << ", actual=" << buffer64_thresh64.actual_size()
              << ", static=" << (buffer64_thresh64.is_static() ? "Yes" : "No") 
              << " (threshold: " << buffer64_thresh64.static_threshold() << ")" << std::endl;
    std::cout << "65-byte buffer with 64-byte threshold: " 
              << "requested=" << buffer65_thresh64.size()
              << ", actual=" << buffer65_thresh64.actual_size()
              << ", static=" << (buffer65_thresh64.is_static() ? "Yes" : "No") 
              << " (threshold: " << buffer65_thresh64.static_threshold() << ")" << std::endl;
    
    // Test always dynamic (threshold = 0)
    SmartBuffer<16, 0> buffer_always_dynamic;
    std::cout << "16-byte buffer with 0-byte threshold: " 
              << "requested=" << buffer_always_dynamic.size()
              << ", actual=" << buffer_always_dynamic.actual_size()
              << ", static=" << (buffer_always_dynamic.is_static() ? "Yes" : "No") 
              << " (threshold: " << buffer_always_dynamic.static_threshold() << ")" << std::endl;
    
    // Test always static (threshold = SIZE_MAX)
    SmartBuffer<1024, SIZE_MAX> buffer_always_static;
    std::cout << "1024-byte buffer with SIZE_MAX threshold: " 
              << "requested=" << buffer_always_static.size()
              << ", actual=" << buffer_always_static.actual_size()
              << ", static=" << (buffer_always_static.is_static() ? "Yes" : "No") 
              << " (threshold: " << buffer_always_static.static_threshold() << ")" << std::endl;
              
    std::cout << std::endl;
}

void test_convenience_aliases() {
    std::cout << "=== Testing Convenience Aliases ===" << std::endl;
    
    // Test default aliases
    SmartBuffer64 buffer64_default;
    std::cout << "SmartBuffer64: requested=" << buffer64_default.size()
              << ", actual=" << buffer64_default.actual_size()
              << ", static=" << (buffer64_default.is_static() ? "Yes" : "No") << std::endl;
    
    // Test custom threshold aliases
    SmartBufferStatic64<64> buffer64_custom;
    SmartBufferStatic128<128> buffer128_custom;
    SmartBufferAlwaysDynamic<16> buffer_dynamic;
    SmartBufferAlwaysStatic<1024> buffer_static;
    
    std::cout << "SmartBufferStatic64<64>: static=" << (buffer64_custom.is_static() ? "Yes" : "No") << std::endl;
    std::cout << "SmartBufferStatic128<128>: static=" << (buffer128_custom.is_static() ? "Yes" : "No") << std::endl;
    std::cout << "SmartBufferAlwaysDynamic<16>: static=" << (buffer_dynamic.is_static() ? "Yes" : "No") << std::endl;
    std::cout << "SmartBufferAlwaysStatic<1024>: static=" << (buffer_static.is_static() ? "Yes" : "No") << std::endl;
    
    std::cout << std::endl;
}

void test_with_c_functions() {
    std::cout << "=== Testing Compatibility with C Functions ===" << std::endl;
    
    SmartBuffer<256> buffer;
    
    // Use with memset (C function)
    std::memset(buffer, 0x42, buffer.size());  // Only fill requested size
    std::cout << "After memset(0x42): buffer[100] = 0x" << std::hex << static_cast<int>(buffer[100]) << std::endl;
    
    // Use with memcpy (C function)
    const char* message = "Hello, SmartBuffer with alignment!";
    std::memcpy(buffer, message, std::strlen(message) + 1);
    std::cout << "After memcpy: buffer contains: " << reinterpret_cast<const char*>(buffer.data()) << std::endl;
    
    std::cout << std::dec << std::endl;
}

int main() {
    std::cout << "SmartBuffer Library Example - Configurable Thresholds with 8-Byte Alignment" << std::endl;
    std::cout << "============================================================================" << std::endl << std::endl;
    
    test_alignment();
    test_static_buffer();
    test_dynamic_buffer();
    test_boundary_case();
    test_custom_thresholds();
    test_convenience_aliases();
    test_with_c_functions();
    
    std::cout << "All tests completed successfully!" << std::endl;
    
    return 0;
}
