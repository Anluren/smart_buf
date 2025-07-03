#include <smart_buffer.hpp>
#include <iostream>
#include <cassert>
#include <cstring>

void test_static_buffer() {
    std::cout << "=== Testing Static Buffer (32 bytes) ===" << std::endl;
    
    SmartBuffer<32> buffer;
    
    // Test basic properties
    std::cout << "Buffer size: " << buffer.size() << " bytes" << std::endl;
    std::cout << "Is static: " << (buffer.is_static() ? "Yes" : "No") << std::endl;
    
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
    
    // Test fill and clear
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
    std::cout << "Buffer size: " << buffer.size() << " bytes" << std::endl;
    std::cout << "Is static: " << (buffer.is_static() ? "Yes" : "No") << std::endl;
    
    // Test operator uint8_t*
    uint8_t* ptr = buffer;
    std::cout << "Buffer address: " << static_cast<void*>(ptr) << std::endl;
    
    // Test array access
    buffer[0] = 0xCC;
    buffer[512] = 0xDD;
    buffer[1023] = 0xEE;
    
    std::cout << "buffer[0] = 0x" << std::hex << static_cast<int>(buffer[0]) << std::endl;
    std::cout << "buffer[512] = 0x" << std::hex << static_cast<int>(buffer[512]) << std::endl;
    std::cout << "buffer[1023] = 0x" << std::hex << static_cast<int>(buffer[1023]) << std::endl;
    
    // Test fill and clear
    buffer.fill(0x77);
    std::cout << "After fill(0x77), buffer[100] = 0x" << std::hex << static_cast<int>(buffer[100]) << std::endl;
    
    buffer.clear();
    std::cout << "After clear(), buffer[100] = 0x" << std::hex << static_cast<int>(buffer[100]) << std::endl;
    
    // Test move constructor
    buffer[256] = 0x88;
    SmartBuffer<1024> buffer_moved(std::move(buffer));
    std::cout << "Move constructor: buffer_moved[256] = 0x" << std::hex << static_cast<int>(buffer_moved[256]) << std::endl;
    
    std::cout << std::dec << std::endl;
}

void test_boundary_case() {
    std::cout << "=== Testing Boundary Case (32 bytes exactly) ===" << std::endl;
    
    SmartBuffer<32> buffer32;
    SmartBuffer<33> buffer33;
    
    std::cout << "32 bytes buffer is static: " << (buffer32.is_static() ? "Yes" : "No") << std::endl;
    std::cout << "33 bytes buffer is static: " << (buffer33.is_static() ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
}

void test_convenience_aliases() {
    std::cout << "=== Testing Convenience Type Aliases ===" << std::endl;
    
    SmartBuffer8 buf8;
    SmartBuffer16 buf16;
    SmartBuffer32 buf32;
    SmartBuffer64 buf64;
    SmartBuffer1K buf1k;
    
    std::cout << "SmartBuffer8 size: " << buf8.size() << ", static: " << buf8.is_static() << std::endl;
    std::cout << "SmartBuffer16 size: " << buf16.size() << ", static: " << buf16.is_static() << std::endl;
    std::cout << "SmartBuffer32 size: " << buf32.size() << ", static: " << buf32.is_static() << std::endl;
    std::cout << "SmartBuffer64 size: " << buf64.size() << ", static: " << buf64.is_static() << std::endl;
    std::cout << "SmartBuffer1K size: " << buf1k.size() << ", static: " << buf1k.is_static() << std::endl;
    std::cout << std::endl;
}

void test_with_c_functions() {
    std::cout << "=== Testing with C Functions ===" << std::endl;
    
    SmartBuffer<256> buffer;
    
    // Use with memset (C function)
    std::memset(buffer, 0x42, buffer.size());
    std::cout << "After memset(0x42): buffer[100] = 0x" << std::hex << static_cast<int>(buffer[100]) << std::endl;
    
    // Use with memcpy (C function)
    const char* message = "Hello, SmartBuffer!";
    std::memcpy(buffer, message, std::strlen(message) + 1);
    std::cout << "After memcpy: buffer contains: " << reinterpret_cast<const char*>(buffer.data()) << std::endl;
    
    std::cout << std::dec << std::endl;
}

int main() {
    std::cout << "SmartBuffer Library Example" << std::endl;
    std::cout << "===========================" << std::endl << std::endl;
    
    test_static_buffer();
    test_dynamic_buffer();
    test_boundary_case();
    test_convenience_aliases();
    test_with_c_functions();
    
    std::cout << "All tests completed successfully!" << std::endl;
    
    return 0;
}
