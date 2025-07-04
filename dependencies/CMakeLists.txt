# Dependencies CMakeLists.txt
# Manages external dependencies for SmartBuffer project

cmake_minimum_required(VERSION 3.12)

# Google Test dependency
if(SMARTBUFFER_BUILD_TESTS)
    # Find or fetch Google Test
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        message(STATUS "Google Test not found, fetching from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
        )
        
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        
        # Don't build Google Test's own tests
        set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
        
        FetchContent_MakeAvailable(googletest)
        
        message(STATUS "Google Test fetched successfully")
    else()
        message(STATUS "Using system Google Test")
    endif()
endif()

# Future dependencies can be added here
# Example:
# if(SMARTBUFFER_BUILD_BENCHMARKS)
#     # Add benchmark library dependency
# endif()
