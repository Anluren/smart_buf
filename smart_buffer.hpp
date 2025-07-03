#pragma once

#include <cstdint>
#include <memory>
#include <array>
#include <type_traits>
#include <algorithm>
#include <cstring>

/**
 * @brief A template-based smart buffer that automatically chooses between 
 *        static and dynamic allocation based on buffer size.
 * 
 * For buffers <= 32 bytes: uses static allocation (std::array)
 * For buffers > 32 bytes: uses dynamic allocation (std::unique_ptr)
 * 
 * @tparam Size The size of the buffer in bytes
 */
template<std::size_t Size>
class SmartBuffer {
private:
    static constexpr std::size_t STATIC_THRESHOLD = 32;
    static constexpr bool use_static = Size <= STATIC_THRESHOLD;
    
    // Type alias for cleaner code
    using StaticBuffer = std::array<std::uint8_t, Size>;
    using DynamicBuffer = std::unique_ptr<std::uint8_t[]>;
    
    // Storage - either static array or dynamic pointer
    typename std::conditional<use_static, StaticBuffer, DynamicBuffer>::type buffer_;
    
    // Helper methods for static allocation
    template<bool IsStatic = use_static>
    typename std::enable_if<IsStatic>::type init_buffer() {
        buffer_.fill(0);
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<!IsStatic>::type init_buffer() {
        buffer_ = std::make_unique<std::uint8_t[]>(Size);
        std::fill(buffer_.get(), buffer_.get() + Size, 0);
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<IsStatic>::type copy_from(const SmartBuffer& other) {
        buffer_ = other.buffer_;
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<!IsStatic>::type copy_from(const SmartBuffer& other) {
        buffer_ = std::make_unique<std::uint8_t[]>(Size);
        std::copy(other.buffer_.get(), other.buffer_.get() + Size, buffer_.get());
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<IsStatic>::type move_from(SmartBuffer&& other) {
        buffer_ = std::move(other.buffer_);
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<!IsStatic>::type move_from(SmartBuffer&& other) {
        buffer_ = std::move(other.buffer_);
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<IsStatic>::type assign_from(const SmartBuffer& other) {
        buffer_ = other.buffer_;
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<!IsStatic>::type assign_from(const SmartBuffer& other) {
        if (!buffer_) {
            buffer_ = std::make_unique<std::uint8_t[]>(Size);
        }
        std::copy(other.buffer_.get(), other.buffer_.get() + Size, buffer_.get());
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<IsStatic, std::uint8_t*>::type get_data() {
        return buffer_.data();
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<!IsStatic, std::uint8_t*>::type get_data() {
        return buffer_.get();
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<IsStatic, const std::uint8_t*>::type get_data() const {
        return buffer_.data();
    }
    
    template<bool IsStatic = use_static>
    typename std::enable_if<!IsStatic, const std::uint8_t*>::type get_data() const {
        return buffer_.get();
    }

public:
    /**
     * @brief Default constructor
     * For static buffers: initializes array to zero
     * For dynamic buffers: allocates memory and initializes to zero
     */
    SmartBuffer() {
        init_buffer();
    }
    
    /**
     * @brief Copy constructor
     */
    SmartBuffer(const SmartBuffer& other) {
        copy_from(other);
    }
    
    /**
     * @brief Move constructor
     */
    SmartBuffer(SmartBuffer&& other) noexcept {
        move_from(std::move(other));
    }
    
    /**
     * @brief Copy assignment operator
     */
    SmartBuffer& operator=(const SmartBuffer& other) {
        if (this != &other) {
            assign_from(other);
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
        return get_data();
    }
    
    /**
     * @brief Conversion operator to const uint8_t*
     * @return Const pointer to the buffer data
     */
    operator const std::uint8_t*() const {
        return get_data();
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
     * @brief Get the size of the buffer
     * @return Size of the buffer in bytes
     */
    constexpr std::size_t size() const noexcept {
        return Size;
    }
    
    /**
     * @brief Check if the buffer uses static allocation
     * @return true if static allocation is used, false for dynamic
     */
    constexpr bool is_static() const noexcept {
        return use_static;
    }
    
    /**
     * @brief Array subscript operator
     * @param index Index to access
     * @return Reference to the byte at the given index
     */
    std::uint8_t& operator[](std::size_t index) {
        return data()[index];
    }
    
    /**
     * @brief Array subscript operator (const version)
     * @param index Index to access
     * @return Const reference to the byte at the given index
     */
    const std::uint8_t& operator[](std::size_t index) const {
        return data()[index];
    }
    
    /**
     * @brief Fill the buffer with a specific value
     * @param value Value to fill the buffer with
     */
    void fill(std::uint8_t value) {
        std::fill(data(), data() + Size, value);
    }
    
    /**
     * @brief Clear the buffer (fill with zeros)
     */
    void clear() {
        fill(0);
    }
};

// Convenience type aliases for common buffer sizes
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
