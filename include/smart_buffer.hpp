#pragma once

#include <cstdint>
#include <memory>
#include <array>
#include <type_traits>
#include <algorithm>

/**
 * @brief A template-based smart buffer that automatically chooses between 
 *        static and dynamic allocation based on buffer size and configurable threshold.
 * 
 * @tparam Size The size of the buffer in bytes (will be rounded up to 8-byte alignment)
 * @tparam StaticThreshold Threshold for static vs dynamic allocation (default: 32)
 * 
 * For buffers <= StaticThreshold bytes: uses static allocation (std::array)
 * For buffers > StaticThreshold bytes: uses dynamic allocation (std::unique_ptr)
 * 
 * Buffer size is automatically rounded up to the nearest 8-byte boundary for optimal alignment.
 * 
 * @requires C++17 or later
 */
template<std::size_t Size, std::size_t StaticThreshold = 32>
class SmartBuffer {
private:
    // Round up to 8-byte alignment
    static constexpr std::size_t round_up_to_8(std::size_t size) noexcept {
        return (size + 7) & ~std::size_t(7);
    }
    
    static constexpr std::size_t ACTUAL_SIZE = round_up_to_8(Size);
    static constexpr std::size_t STATIC_THRESHOLD = StaticThreshold;
    static constexpr bool use_static = ACTUAL_SIZE <= STATIC_THRESHOLD;
    
    // Type alias for cleaner code
    using StaticBuffer = std::array<std::uint8_t, ACTUAL_SIZE>;
    using DynamicBuffer = std::unique_ptr<std::uint8_t[]>;
    
    // Storage - either static array or dynamic pointer
    typename std::conditional_t<use_static, StaticBuffer, DynamicBuffer> buffer_;

public:
    /**
     * @brief Default constructor
     * For static buffers: initializes array to zero
     * For dynamic buffers: allocates memory and initializes to zero
     */
    SmartBuffer() {
        if constexpr (use_static) {
            buffer_.fill(0);
        } else {
            buffer_ = std::make_unique<std::uint8_t[]>(ACTUAL_SIZE);
            std::fill_n(buffer_.get(), ACTUAL_SIZE, 0);
        }
    }
    
    /**
     * @brief Copy constructor
     */
    SmartBuffer(const SmartBuffer& other) {
        if constexpr (use_static) {
            buffer_ = other.buffer_;
        } else {
            buffer_ = std::make_unique<std::uint8_t[]>(ACTUAL_SIZE);
            std::copy_n(other.buffer_.get(), ACTUAL_SIZE, buffer_.get());
        }
    }
    
    /**
     * @brief Move constructor
     */
    SmartBuffer(SmartBuffer&& other) noexcept {
        if constexpr (use_static) {
            buffer_ = std::move(other.buffer_);
        } else {
            buffer_ = std::move(other.buffer_);
        }
    }
    
    /**
     * @brief Copy assignment operator
     */
    SmartBuffer& operator=(const SmartBuffer& other) {
        if (this != &other) {
            if constexpr (use_static) {
                buffer_ = other.buffer_;
            } else {
                if (!buffer_) {
                    buffer_ = std::make_unique<std::uint8_t[]>(ACTUAL_SIZE);
                }
                std::copy_n(other.buffer_.get(), ACTUAL_SIZE, buffer_.get());
            }
        }
        return *this;
    }
    
    /**
     * @brief Move assignment operator
     */
    SmartBuffer& operator=(SmartBuffer&& other) noexcept {
        if (this != &other) {
            buffer_ = std::move(other.buffer_);
        }
        return *this;
    }
    
    /**
     * @brief Destructor (default is sufficient)
     */
    ~SmartBuffer() = default;
    
    /**
     * @brief Conversion operator to uint8_t*
     * @return Pointer to the buffer data
     */
    operator std::uint8_t*() {
        if constexpr (use_static) {
            return buffer_.data();
        } else {
            return buffer_.get();
        }
    }
    
    /**
     * @brief Conversion operator to const uint8_t*
     * @return Const pointer to the buffer data
     */
    operator const std::uint8_t*() const {
        if constexpr (use_static) {
            return buffer_.data();
        } else {
            return buffer_.get();
        }
    }
    
    /**
     * @brief Get the buffer data pointer
     * @return Pointer to the buffer data
     */
    std::uint8_t* data() {
        return static_cast<std::uint8_t*>(*this);
    }
    
    /**
     * @brief Get the buffer data pointer (const version)
     * @return Const pointer to the buffer data
     */
    const std::uint8_t* data() const {
        return static_cast<const std::uint8_t*>(*this);
    }
    
    /**
     * @brief Get the requested size of the buffer
     * @return Requested size of the buffer in bytes
     */
    constexpr std::size_t size() const noexcept {
        return Size;
    }
    
    /**
     * @brief Get the actual allocated size of the buffer (rounded up to 8-byte alignment)
     * @return Actual allocated size of the buffer in bytes
     */
    constexpr std::size_t actual_size() const noexcept {
        return ACTUAL_SIZE;
    }
    
    /**
     * @brief Check if the buffer uses static allocation
     * @return true if static allocation is used, false for dynamic
     */
    constexpr bool is_static() const noexcept {
        return use_static;
    }
    
    /**
     * @brief Get the static threshold value
     * @return The threshold value used for static vs dynamic allocation
     */
    constexpr std::size_t static_threshold() const noexcept {
        return STATIC_THRESHOLD;
    }
    
    /**
     * @brief Array subscript operator
     * @param index Index to access (should be within requested size, not actual size)
     * @return Reference to the byte at the given index
     */
    std::uint8_t& operator[](std::size_t index) {
        return data()[index];
    }
    
    /**
     * @brief Array subscript operator (const version)
     * @param index Index to access (should be within requested size, not actual size)
     * @return Const reference to the byte at the given index
     */
    const std::uint8_t& operator[](std::size_t index) const {
        return data()[index];
    }
    
    /**
     * @brief Fill the buffer with a specific value (only fills requested size, not padding)
     * @param value Value to fill the buffer with
     */
    void fill(std::uint8_t value) {
        std::fill_n(data(), Size, value);  // Only fill requested size
    }
    
    /**
     * @brief Clear the buffer (fill with zeros, only clears requested size, not padding)
     */
    void clear() {
        fill(0);
    }
    
    /**
     * @brief Fill the entire allocated buffer including padding
     * @param value Value to fill the buffer with
     */
    void fill_all(std::uint8_t value) {
        std::fill_n(data(), ACTUAL_SIZE, value);  // Fill entire allocated buffer
    }
    
    /**
     * @brief Clear the entire allocated buffer including padding
     */
    void clear_all() {
        fill_all(0);
    }
};

// Convenience type aliases with default threshold (32 bytes)
using SmartBuffer8 = SmartBuffer<8>;
using SmartBuffer16 = SmartBuffer<16>;
using SmartBuffer32 = SmartBuffer<32>;
using SmartBuffer64 = SmartBuffer<64>;
using SmartBuffer128 = SmartBuffer<128>;
using SmartBuffer256 = SmartBuffer<256>;
using SmartBuffer512 = SmartBuffer<512>;
using SmartBuffer1K = SmartBuffer<1024>;
using SmartBuffer2K = SmartBuffer<2048>;
using SmartBuffer4K = SmartBuffer<4096>;

// Convenience type aliases with custom thresholds
template<std::size_t Size>
using SmartBufferStatic64 = SmartBuffer<Size, 64>;  // 64-byte threshold

template<std::size_t Size>
using SmartBufferStatic128 = SmartBuffer<Size, 128>; // 128-byte threshold

template<std::size_t Size>
using SmartBufferStatic256 = SmartBuffer<Size, 256>; // 256-byte threshold

template<std::size_t Size>
using SmartBufferAlwaysDynamic = SmartBuffer<Size, 0>; // Always dynamic

template<std::size_t Size>
using SmartBufferAlwaysStatic = SmartBuffer<Size, SIZE_MAX>; // Always static
