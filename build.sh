#!/bin/bash

# SmartBuffer Build Script
# Usage: ./build.sh [options]

set -e  # Exit on any error

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
EXAMPLES=ON
TESTS=ON
BENCHMARKS=ON
CLEAN=false
INSTALL=false
RUN_TESTS=false

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Help function
show_help() {
    cat << EOF
SmartBuffer Build Script

Usage: $0 [OPTIONS]

OPTIONS:
    -h, --help              Show this help message
    -t, --type TYPE         Build type: Debug, Release, RelWithDebInfo, MinSizeRel (default: Release)
    -d, --dir DIR          Build directory (default: build)
    --no-examples          Don't build examples
    --no-tests             Don't build tests  
    --no-benchmarks        Don't build benchmarks
    -c, --clean            Clean build directory before building
    -i, --install          Install after building
    -r, --run-tests        Run tests after building
    -j, --jobs N           Number of parallel jobs (default: auto-detect)

EXAMPLES:
    $0                              # Standard release build
    $0 -t Debug -c                  # Clean debug build
    $0 --no-benchmarks -r           # Build without benchmarks and run tests
    $0 -t Release -i                # Release build with installation
    $0 -c -r -i                     # Clean build, run tests, and install

EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -d|--dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        --no-examples)
            EXAMPLES=OFF
            shift
            ;;
        --no-tests)
            TESTS=OFF
            shift
            ;;
        --no-benchmarks)
            BENCHMARKS=OFF
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -i|--install)
            INSTALL=true
            shift
            ;;
        -r|--run-tests)
            RUN_TESTS=true
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        *)
            print_error "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# Auto-detect number of CPU cores if not specified
if [[ -z "${JOBS:-}" ]]; then
    if command -v nproc >/dev/null 2>&1; then
        JOBS=$(nproc)
    elif command -v sysctl >/dev/null 2>&1; then
        JOBS=$(sysctl -n hw.ncpu)
    else
        JOBS=4
    fi
fi

# Validate build type
case $BUILD_TYPE in
    Debug|Release|RelWithDebInfo|MinSizeRel)
        ;;
    *)
        print_error "Invalid build type: $BUILD_TYPE"
        print_error "Valid types: Debug, Release, RelWithDebInfo, MinSizeRel"
        exit 1
        ;;
esac

print_status "SmartBuffer Build Configuration"
echo "  Build Type: $BUILD_TYPE"
echo "  Build Directory: $BUILD_DIR"
echo "  Examples: $EXAMPLES"
echo "  Tests: $TESTS"
echo "  Benchmarks: $BENCHMARKS"
echo "  Parallel Jobs: $JOBS"
echo "  Clean: $CLEAN"
echo "  Install: $INSTALL"
echo "  Run Tests: $RUN_TESTS"
echo

# Clean build directory if requested
if [[ "$CLEAN" == "true" ]]; then
    if [[ -d "$BUILD_DIR" ]]; then
        print_status "Cleaning build directory: $BUILD_DIR"
        rm -rf "$BUILD_DIR"
    fi
fi

# Create build directory
print_status "Creating build directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"

# Configure
print_status "Configuring with CMake..."
cd "$BUILD_DIR"

cmake .. \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DSMARTBUFFER_BUILD_EXAMPLES="$EXAMPLES" \
    -DSMARTBUFFER_BUILD_TESTS="$TESTS" \
    -DSMARTBUFFER_BUILD_BENCHMARKS="$BENCHMARKS"

# Build
print_status "Building with $JOBS parallel jobs..."
cmake --build . --parallel "$JOBS"

print_success "Build completed successfully!"

# Run tests if requested
if [[ "$RUN_TESTS" == "true" && "$TESTS" == "ON" ]]; then
    print_status "Running tests..."
    ctest --output-on-failure
    print_success "Tests completed!"
fi

# Install if requested
if [[ "$INSTALL" == "true" ]]; then
    print_status "Installing..."
    cmake --install .
    print_success "Installation completed!"
fi

# Show available executables
print_status "Available executables in $BUILD_DIR:"
if [[ "$EXAMPLES" == "ON" && -x "smartbuffer_example" ]]; then
    echo "  ./smartbuffer_example - Run examples"
fi
if [[ "$TESTS" == "ON" && -x "smartbuffer_test" ]]; then
    echo "  ./smartbuffer_test - Run unit tests"
fi
if [[ "$BENCHMARKS" == "ON" && -x "smartbuffer_benchmark" ]]; then
    echo "  ./smartbuffer_benchmark - Run performance benchmarks"
fi

print_success "Build script completed successfully!"
