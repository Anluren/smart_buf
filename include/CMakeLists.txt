# SmartBuffer Library CMakeLists.txt
# Header-only C++17 buffer library with automatic allocation strategy

cmake_minimum_required(VERSION 3.12)

# Define the header-only library target
add_library(smart_buffer INTERFACE)
add_library(SmartBuffer::smart_buffer ALIAS smart_buffer)

# Set target properties
target_include_directories(smart_buffer
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(smart_buffer INTERFACE cxx_std_17)

# Set compiler-specific options for better optimization
target_compile_options(smart_buffer INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Installation rules (if this library is installed)
if(SMARTBUFFER_ENABLE_INSTALL)
    include(GNUInstallDirs)
    
    # Install header files
    install(FILES smart_buffer.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    
    # Install targets
    install(TARGETS smart_buffer
        EXPORT SmartBufferTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    
    # Install export
    install(EXPORT SmartBufferTargets
        FILE SmartBufferTargets.cmake
        NAMESPACE SmartBuffer::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SmartBuffer
    )
    
    # Generate and install config files
    include(CMakePackageConfigHelpers)
    
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/SmartBufferConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/SmartBufferConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SmartBuffer
    )
    
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/SmartBufferConfigVersion.cmake"
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion
    )
    
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/SmartBufferConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/SmartBufferConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SmartBuffer
    )
endif()

# Print information about the library target
message(STATUS "SmartBuffer library configured:")
message(STATUS "  - Header-only C++17 library")
message(STATUS "  - Include directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  - Target: SmartBuffer::smart_buffer")
