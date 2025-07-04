#include <smart_buffer.hpp>
#include <gtest/gtest.h>
#include <cstring>

TEST(SmartBufferTest, StaticAllocationThreshold) {
    // Test the 32-byte boundary
    SmartBuffer<1> buf1;
    SmartBuffer<16> buf16;
    SmartBuffer<32> buf32;
    SmartBuffer<33> buf33;
    SmartBuffer<64> buf64;
    
    EXPECT_TRUE(buf1.is_static());
    EXPECT_TRUE(buf16.is_static());
    EXPECT_TRUE(buf32.is_static());
    EXPECT_FALSE(buf33.is_static());
    EXPECT_FALSE(buf64.is_static());
}

TEST(SmartBufferTest, BufferInitialization) {
    SmartBuffer<16> static_buf;
    SmartBuffer<64> dynamic_buf;
    
    // Buffers should be zero-initialized
    for (size_t i = 0; i < static_buf.size(); ++i) {
        EXPECT_EQ(static_buf[i], 0);
    }
    
    for (size_t i = 0; i < dynamic_buf.size(); ++i) {
        EXPECT_EQ(dynamic_buf[i], 0);
    }
}

TEST(SmartBufferTest, OperatorConversion) {
    SmartBuffer<16> static_buf;
    SmartBuffer<64> dynamic_buf;
    
    // Test conversion to uint8_t*
    uint8_t* static_ptr = static_buf;
    uint8_t* dynamic_ptr = dynamic_buf;
    
    EXPECT_NE(static_ptr, nullptr);
    EXPECT_NE(dynamic_ptr, nullptr);
    
    // Test that the pointer is valid
    static_ptr[0] = 0xAA;
    dynamic_ptr[0] = 0xBB;
    
    EXPECT_EQ(static_buf[0], 0xAA);
    EXPECT_EQ(dynamic_buf[0], 0xBB);
}

TEST(SmartBufferTest, ArrayAccess) {
    SmartBuffer<32> static_buf;
    SmartBuffer<128> dynamic_buf;
    
    // Test array subscript operator
    static_buf[0] = 0x11;
    static_buf[15] = 0x22;
    static_buf[31] = 0x33;
    
    EXPECT_EQ(static_buf[0], 0x11);
    EXPECT_EQ(static_buf[15], 0x22);
    EXPECT_EQ(static_buf[31], 0x33);
    
    dynamic_buf[0] = 0x44;
    dynamic_buf[64] = 0x55;
    dynamic_buf[127] = 0x66;
    
    EXPECT_EQ(dynamic_buf[0], 0x44);
    EXPECT_EQ(dynamic_buf[64], 0x55);
    EXPECT_EQ(dynamic_buf[127], 0x66);
}

TEST(SmartBufferTest, FillAndClear) {
    SmartBuffer<32> static_buf;
    SmartBuffer<128> dynamic_buf;
    
    // Test fill
    static_buf.fill(0xAA);
    for (size_t i = 0; i < static_buf.size(); ++i) {
        EXPECT_EQ(static_buf[i], 0xAA);
    }
    
    dynamic_buf.fill(0xBB);
    for (size_t i = 0; i < dynamic_buf.size(); ++i) {
        EXPECT_EQ(dynamic_buf[i], 0xBB);
    }
    
    // Test clear
    static_buf.clear();
    for (size_t i = 0; i < static_buf.size(); ++i) {
        EXPECT_EQ(static_buf[i], 0);
    }
    
    dynamic_buf.clear();
    for (size_t i = 0; i < dynamic_buf.size(); ++i) {
        EXPECT_EQ(dynamic_buf[i], 0);
    }
}

TEST(SmartBufferTest, CopyConstructor) {
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
        EXPECT_EQ(static_copy[i], static_original[i]);
    }
    
    for (size_t i = 0; i < dynamic_original.size(); ++i) {
        EXPECT_EQ(dynamic_copy[i], dynamic_original[i]);
    }
    
    // Verify they're independent
    static_copy[0] = 0xFF;
    dynamic_copy[0] = 0xFF;
    
    EXPECT_NE(static_original[0], 0xFF);
    EXPECT_NE(dynamic_original[0], 0xFF);
}

TEST(SmartBufferTest, MoveConstructor) {
    SmartBuffer<16> static_original;
    SmartBuffer<128> dynamic_original;
    
    // Fill with test pattern
    static_original[5] = 0x99;
    dynamic_original[50] = 0x88;
    
    // Test move constructor
    SmartBuffer<16> static_moved(std::move(static_original));
    SmartBuffer<128> dynamic_moved(std::move(dynamic_original));
    
    // Verify move worked
    EXPECT_EQ(static_moved[5], 0x99);
    EXPECT_EQ(dynamic_moved[50], 0x88);
}

TEST(SmartBufferTest, AssignmentOperators) {
    SmartBuffer<16> static_buf1, static_buf2;
    SmartBuffer<128> dynamic_buf1, dynamic_buf2;
    
    // Fill first buffers
    static_buf1.fill(0xAA);
    dynamic_buf1.fill(0xBB);
    
    // Test copy assignment
    static_buf2 = static_buf1;
    dynamic_buf2 = dynamic_buf1;
    
    EXPECT_EQ(static_buf2[8], 0xAA);
    EXPECT_EQ(dynamic_buf2[64], 0xBB);
    
    // Test self-assignment (should not crash)
    static_buf1 = static_buf1;
    dynamic_buf1 = dynamic_buf1;
    
    EXPECT_EQ(static_buf1[8], 0xAA);
    EXPECT_EQ(dynamic_buf1[64], 0xBB);
}

TEST(SmartBufferTest, DataMethod) {
    SmartBuffer<16> static_buf;
    SmartBuffer<128> dynamic_buf;
    
    // Test data() method
    uint8_t* static_ptr = static_buf.data();
    uint8_t* dynamic_ptr = dynamic_buf.data();
    
    EXPECT_NE(static_ptr, nullptr);
    EXPECT_NE(dynamic_ptr, nullptr);
    
    // Should be same as conversion operator
    EXPECT_EQ(static_ptr, static_cast<uint8_t*>(static_buf));
    EXPECT_EQ(dynamic_ptr, static_cast<uint8_t*>(dynamic_buf));
}

TEST(SmartBufferTest, ConstOperations) {
    const SmartBuffer<16> static_buf;
    const SmartBuffer<128> dynamic_buf;
    
    // Test const operations
    EXPECT_EQ(static_buf.size(), 16);
    EXPECT_EQ(dynamic_buf.size(), 128);
    EXPECT_TRUE(static_buf.is_static());
    EXPECT_FALSE(dynamic_buf.is_static());
    
    // Test const data access
    const uint8_t* static_ptr = static_buf.data();
    const uint8_t* dynamic_ptr = dynamic_buf.data();
    
    EXPECT_NE(static_ptr, nullptr);
    EXPECT_NE(dynamic_ptr, nullptr);
    
    // Test const conversion operator
    const uint8_t* static_conv = static_buf;
    const uint8_t* dynamic_conv = dynamic_buf;
    
    EXPECT_EQ(static_conv, static_ptr);
    EXPECT_EQ(dynamic_conv, dynamic_ptr);
}

TEST(SmartBufferTest, CFunctionCompatibility) {
    SmartBuffer<64> buffer;
    
    // Test with memset
    std::memset(buffer, 0x42, buffer.size());
    for (size_t i = 0; i < buffer.size(); ++i) {
        EXPECT_EQ(buffer[i], 0x42);
    }
    
    // Test with memcpy
    const char* test_string = "Hello SmartBuffer";
    std::memcpy(buffer, test_string, std::strlen(test_string) + 1);
    
    EXPECT_STREQ(reinterpret_cast<const char*>(buffer.data()), test_string);
}

TEST(SmartBufferTest, TypeAliases) {
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
    EXPECT_EQ(buf8.size(), 8);
    EXPECT_EQ(buf16.size(), 16);
    EXPECT_EQ(buf32.size(), 32);
    EXPECT_EQ(buf64.size(), 64);
    EXPECT_EQ(buf128.size(), 128);
    EXPECT_EQ(buf256.size(), 256);
    EXPECT_EQ(buf512.size(), 512);
    EXPECT_EQ(buf1k.size(), 1024);
    EXPECT_EQ(buf2k.size(), 2048);
    EXPECT_EQ(buf4k.size(), 4096);
    
    // Verify allocation types
    EXPECT_TRUE(buf8.is_static());
    EXPECT_TRUE(buf16.is_static());
    EXPECT_TRUE(buf32.is_static());
    EXPECT_FALSE(buf64.is_static());
    EXPECT_FALSE(buf128.is_static());
    EXPECT_FALSE(buf256.is_static());
    EXPECT_FALSE(buf512.is_static());
    EXPECT_FALSE(buf1k.is_static());
    EXPECT_FALSE(buf2k.is_static());
    EXPECT_FALSE(buf4k.is_static());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
