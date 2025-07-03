#include "smart_buffer.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

// Simple test framework
#define TEST(name) void test_##name()
#define RUN_TEST(name) do { \
    std::cout << "Running " #name "... "; \
    test_##name(); \
    std::cout << "PASSED" << std::endl; \
} while(0)

TEST(static_allocation_threshold) {
    // Test the 32-byte boundary
    SmartBuffer<1> buf1;
    SmartBuffer<16> buf16;
    SmartBuffer<32> buf32;
    SmartBuffer<33> buf33;
    SmartBuffer<64> buf64;
    
    assert(buf1.is_static() == true);
    assert(buf16.is_static() == true);
    assert(buf32.is_static() == true);
    assert(buf33.is_static() == false);
    assert(buf64.is_static() == false);
}

TEST(buffer_initialization) {
    SmartBuffer<16> static_buf;
    SmartBuffer<64> dynamic_buf;
    
    // Buffers should be zero-initialized
    for (size_t i = 0; i < static_buf.size(); ++i) {
        assert(static_buf[i] == 0);
    }
    
    for (size_t i = 0; i < dynamic_buf.size(); ++i) {
        assert(dynamic_buf[i] == 0);
    }
}

TEST(operator_conversion) {
    SmartBuffer<16> static_buf;
    SmartBuffer<64> dynamic_buf;
    
    // Test conversion to uint8_t*
    uint8_t* static_ptr = static_buf;
    uint8_t* dynamic_ptr = dynamic_buf;
    
    assert(static_ptr != nullptr);
    assert(dynamic_ptr != nullptr);
    
    // Test that the pointer is valid
    static_ptr[0] = 0xAA;
    dynamic_ptr[0] = 0xBB;
    
    assert(static_buf[0] == 0xAA);
    assert(dynamic_buf[0] == 0xBB);
}

TEST(array_access) {
    SmartBuffer<32> static_buf;
    SmartBuffer<128> dynamic_buf;
    
    // Test array subscript operator
    static_buf[0] = 0x11;
    static_buf[15] = 0x22;
    static_buf[31] = 0x33;
    
    assert(static_buf[0] == 0x11);
    assert(static_buf[15] == 0x22);
    assert(static_buf[31] == 0x33);
    
    dynamic_buf[0] = 0x44;
    dynamic_buf[64] = 0x55;
    dynamic_buf[127] = 0x66;
    
    assert(dynamic_buf[0] == 0x44);
    assert(dynamic_buf[64] == 0x55);
    assert(dynamic_buf[127] == 0x66);
}

TEST(fill_and_clear) {
    SmartBuffer<32> static_buf;
    SmartBuffer<128> dynamic_buf;
    
    // Test fill
    static_buf.fill(0xAA);
    for (size_t i = 0; i < static_buf.size(); ++i) {
        assert(static_buf[i] == 0xAA);
    }
    
    dynamic_buf.fill(0xBB);
    for (size_t i = 0; i < dynamic_buf.size(); ++i) {
        assert(dynamic_buf[i] == 0xBB);
    }
    
    // Test clear
    static_buf.clear();
    for (size_t i = 0; i < static_buf.size(); ++i) {
        assert(static_buf[i] == 0);
    }
    
    dynamic_buf.clear();
    for (size_t i = 0; i < dynamic_buf.size(); ++i) {
        assert(dynamic_buf[i] == 0);
    }
}

TEST(copy_constructor) {
    SmartBuffer<16> static_original;
    SmartBuffer<128> dynamic_original;
    
    // Fill with test pattern
    for (size_t i = 0; i < static_original.size(); ++i) {
        static_original[i] = static_cast<uint8_t>(i);
    }
    
    for (size_t i = 0; i < dynamic_original.size(); ++i) {
        dynamic_original[i] = static_cast<uint8_t>(i % 256);
    }
    
    // Test copy constructor
    SmartBuffer<16> static_copy(static_original);
    SmartBuffer<128> dynamic_copy(dynamic_original);
    
    // Verify copies are identical
    for (size_t i = 0; i < static_original.size(); ++i) {
        assert(static_copy[i] == static_original[i]);
    }
    
    for (size_t i = 0; i < dynamic_original.size(); ++i) {
        assert(dynamic_copy[i] == dynamic_original[i]);
    }
    
    // Verify they're independent
    static_copy[0] = 0xFF;
    dynamic_copy[0] = 0xFF;
    
    assert(static_original[0] != 0xFF);
    assert(dynamic_original[0] != 0xFF);
}

TEST(move_constructor) {
    SmartBuffer<16> static_original;
    SmartBuffer<128> dynamic_original;
    
    // Fill with test pattern
    static_original[5] = 0x99;
    dynamic_original[50] = 0x88;
    
    // Test move constructor
    SmartBuffer<16> static_moved(std::move(static_original));
    SmartBuffer<128> dynamic_moved(std::move(dynamic_original));
    
    // Verify move worked
    assert(static_moved[5] == 0x99);
    assert(dynamic_moved[50] == 0x88);
}

TEST(assignment_operators) {
    SmartBuffer<16> static_buf1, static_buf2;
    SmartBuffer<128> dynamic_buf1, dynamic_buf2;
    
    // Fill first buffers
    static_buf1.fill(0xAA);
    dynamic_buf1.fill(0xBB);
    
    // Test copy assignment
    static_buf2 = static_buf1;
    dynamic_buf2 = dynamic_buf1;
    
    assert(static_buf2[8] == 0xAA);
    assert(dynamic_buf2[64] == 0xBB);
    
    // Test self-assignment (should not crash)
    static_buf1 = static_buf1;
    dynamic_buf1 = dynamic_buf1;
    
    assert(static_buf1[8] == 0xAA);
    assert(dynamic_buf1[64] == 0xBB);
}

TEST(data_method) {
    SmartBuffer<16> static_buf;
    SmartBuffer<128> dynamic_buf;
    
    // Test data() method
    uint8_t* static_ptr = static_buf.data();
    uint8_t* dynamic_ptr = dynamic_buf.data();
    
    assert(static_ptr != nullptr);
    assert(dynamic_ptr != nullptr);
    
    // Should be same as conversion operator
    assert(static_ptr == static_cast<uint8_t*>(static_buf));
    assert(dynamic_ptr == static_cast<uint8_t*>(dynamic_buf));
}

TEST(const_operations) {
    const SmartBuffer<16> static_buf;
    const SmartBuffer<128> dynamic_buf;
    
    // Test const operations
    assert(static_buf.size() == 16);
    assert(dynamic_buf.size() == 128);
    assert(static_buf.is_static() == true);
    assert(dynamic_buf.is_static() == false);
    
    // Test const data access
    const uint8_t* static_ptr = static_buf.data();
    const uint8_t* dynamic_ptr = dynamic_buf.data();
    
    assert(static_ptr != nullptr);
    assert(dynamic_ptr != nullptr);
    
    // Test const conversion operator
    const uint8_t* static_conv = static_buf;
    const uint8_t* dynamic_conv = dynamic_buf;
    
    assert(static_conv == static_ptr);
    assert(dynamic_conv == dynamic_ptr);
}

TEST(c_function_compatibility) {
    SmartBuffer<64> buffer;
    
    // Test with memset
    std::memset(buffer, 0x42, buffer.size());
    for (size_t i = 0; i < buffer.size(); ++i) {
        assert(buffer[i] == 0x42);
    }
    
    // Test with memcpy
    const char* test_string = "Hello SmartBuffer";
    std::memcpy(buffer, test_string, std::strlen(test_string) + 1);
    
    assert(std::strcmp(reinterpret_cast<const char*>(buffer.data()), test_string) == 0);
}

TEST(type_aliases) {
    SmartBuffer8 buf8;
    SmartBuffer16 buf16;
    SmartBuffer32 buf32;
    SmartBuffer64 buf64;
    SmartBuffer128 buf128;
    SmartBuffer256 buf256;
    SmartBuffer512 buf512;
    SmartBuffer1K buf1k;
    SmartBuffer2K buf2k;
    SmartBuffer4K buf4k;
    
    // Verify sizes
    assert(buf8.size() == 8);
    assert(buf16.size() == 16);
    assert(buf32.size() == 32);
    assert(buf64.size() == 64);
    assert(buf128.size() == 128);
    assert(buf256.size() == 256);
    assert(buf512.size() == 512);
    assert(buf1k.size() == 1024);
    assert(buf2k.size() == 2048);
    assert(buf4k.size() == 4096);
    
    // Verify allocation types
    assert(buf8.is_static() == true);
    assert(buf16.is_static() == true);
    assert(buf32.is_static() == true);
    assert(buf64.is_static() == false);
    assert(buf128.is_static() == false);
    assert(buf256.is_static() == false);
    assert(buf512.is_static() == false);
    assert(buf1k.is_static() == false);
    assert(buf2k.is_static() == false);
    assert(buf4k.is_static() == false);
}

int main() {
    std::cout << "Running SmartBuffer Unit Tests" << std::endl;
    std::cout << "==============================" << std::endl;
    
    RUN_TEST(static_allocation_threshold);
    RUN_TEST(buffer_initialization);
    RUN_TEST(operator_conversion);
    RUN_TEST(array_access);
    RUN_TEST(fill_and_clear);
    RUN_TEST(copy_constructor);
    RUN_TEST(move_constructor);
    RUN_TEST(assignment_operators);
    RUN_TEST(data_method);
    RUN_TEST(const_operations);
    RUN_TEST(c_function_compatibility);
    RUN_TEST(type_aliases);
    
    std::cout << std::endl << "All tests passed! ✓" << std::endl;
    
    return 0;
}
