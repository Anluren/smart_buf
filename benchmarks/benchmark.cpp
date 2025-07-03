#include <smart_buffer.hpp>
#include <chrono>
#include <iostream>
#include <vector>

// Simple benchmark helper
class Timer {
public:
    Timer(const std::string& name) : name_(name), start_(std::chrono::high_resolution_clock::now()) {}
    
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        std::cout << name_ << ": " << duration.count() << " μs" << std::endl;
    }
    
private:
    std::string name_;
    std::chrono::high_resolution_clock::time_point start_;
};

void benchmark_static_vs_dynamic() {
    const int iterations = 100000;
    
    std::cout << "=== Allocation Benchmark (" << iterations << " iterations) ===" << std::endl;
    
    // Benchmark static allocation (16 bytes)
    {
        Timer timer("Static allocation (16 bytes)");
        for (int i = 0; i < iterations; ++i) {
            SmartBuffer<16> buffer;
            buffer[0] = static_cast<uint8_t>(i);
            buffer[15] = static_cast<uint8_t>(i + 1);
        }
    }
    
    // Benchmark dynamic allocation (16 bytes)
    {
        Timer timer("Dynamic allocation (std::vector<uint8_t>(16))");
        for (int i = 0; i < iterations; ++i) {
            std::vector<uint8_t> buffer(16);
            buffer[0] = static_cast<uint8_t>(i);
            buffer[15] = static_cast<uint8_t>(i + 1);
        }
    }
    
    // Benchmark SmartBuffer with larger size (uses dynamic allocation)
    {
        Timer timer("SmartBuffer dynamic (128 bytes)");
        for (int i = 0; i < iterations; ++i) {
            SmartBuffer<128> buffer;
            buffer[0] = static_cast<uint8_t>(i);
            buffer[127] = static_cast<uint8_t>(i + 1);
        }
    }
    
    // Benchmark std::vector with larger size
    {
        Timer timer("std::vector<uint8_t>(128)");
        for (int i = 0; i < iterations; ++i) {
            std::vector<uint8_t> buffer(128);
            buffer[0] = static_cast<uint8_t>(i);
            buffer[127] = static_cast<uint8_t>(i + 1);
        }
    }
    
    std::cout << std::endl;
}

void benchmark_copy_operations() {
    const int iterations = 50000;
    
    std::cout << "=== Copy Operations Benchmark (" << iterations << " iterations) ===" << std::endl;
    
    // Prepare source buffers
    SmartBuffer<32> static_source;
    SmartBuffer<128> dynamic_source;
    static_source.fill(0xAA);
    dynamic_source.fill(0xBB);
    
    // Benchmark static buffer copy
    {
        Timer timer("Static buffer copy (32 bytes)");
        for (int i = 0; i < iterations; ++i) {
            SmartBuffer<32> copy(static_source);
            (void)copy; // Suppress unused variable warning
        }
    }
    
    // Benchmark dynamic buffer copy
    {
        Timer timer("Dynamic buffer copy (128 bytes)");
        for (int i = 0; i < iterations; ++i) {
            SmartBuffer<128> copy(dynamic_source);
            (void)copy; // Suppress unused variable warning
        }
    }
    
    std::cout << std::endl;
}

void benchmark_memory_access() {
    const int iterations = 1000000;
    
    std::cout << "=== Memory Access Benchmark (" << iterations << " iterations) ===" << std::endl;
    
    SmartBuffer<32> static_buffer;
    SmartBuffer<128> dynamic_buffer;
    
    uint32_t checksum1 = 0, checksum2 = 0;
    
    // Benchmark static buffer access
    {
        Timer timer("Static buffer access (32 bytes)");
        for (int i = 0; i < iterations; ++i) {
            static_buffer[i % static_buffer.size()] = static_cast<uint8_t>(i);
            checksum1 += static_buffer[i % static_buffer.size()];
        }
    }
    
    // Benchmark dynamic buffer access
    {
        Timer timer("Dynamic buffer access (128 bytes)");
        for (int i = 0; i < iterations; ++i) {
            dynamic_buffer[i % dynamic_buffer.size()] = static_cast<uint8_t>(i);
            checksum2 += dynamic_buffer[i % dynamic_buffer.size()];
        }
    }
    
    // Use checksums to prevent optimization
    std::cout << "Checksums (to prevent dead code elimination): " 
              << checksum1 << ", " << checksum2 << std::endl << std::endl;
}

void demonstrate_automatic_selection() {
    std::cout << "=== Automatic Allocation Selection ===" << std::endl;
    
    struct BufferInfo {
        size_t size;
        bool is_static;
        const char* description;
    };
    
    std::vector<BufferInfo> buffers = {
        {1, SmartBuffer<1>{}.is_static(), "Tiny buffer (1 byte)"},
        {8, SmartBuffer<8>{}.is_static(), "Small buffer (8 bytes)"},
        {16, SmartBuffer<16>{}.is_static(), "Medium-small buffer (16 bytes)"},
        {32, SmartBuffer<32>{}.is_static(), "Boundary case (32 bytes)"},
        {33, SmartBuffer<33>{}.is_static(), "Just over boundary (33 bytes)"},
        {64, SmartBuffer<64>{}.is_static(), "Medium buffer (64 bytes)"},
        {128, SmartBuffer<128>{}.is_static(), "Large buffer (128 bytes)"},
        {1024, SmartBuffer<1024>{}.is_static(), "Very large buffer (1KB)"},
        {4096, SmartBuffer<4096>{}.is_static(), "Huge buffer (4KB)"}
    };
    
    for (const auto& info : buffers) {
        std::cout << info.description << " -> " 
                  << (info.is_static ? "Static" : "Dynamic") 
                  << " allocation" << std::endl;
    }
    
    std::cout << std::endl;
}

int main() {
    std::cout << "SmartBuffer Performance Benchmark" << std::endl;
    std::cout << "==================================" << std::endl << std::endl;
    
    demonstrate_automatic_selection();
    benchmark_static_vs_dynamic();
    benchmark_copy_operations();
    benchmark_memory_access();
    
    std::cout << "Performance notes:" << std::endl;
    std::cout << "- Static allocation shows minimal overhead" << std::endl;
    std::cout << "- Dynamic allocation uses heap but provides automatic management" << std::endl;
    std::cout << "- The 32-byte threshold balances stack usage vs. allocation overhead" << std::endl;
    
    return 0;
}
