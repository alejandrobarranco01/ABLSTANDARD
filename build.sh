#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

# Parse command line arguments
CLEAN=false
BUILD_TYPE="Debug"

for arg in "$@"; do
    case $arg in
        --clean)
            CLEAN=true
            shift
            ;;
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --release)
            BUILD_TYPE="Release"
            shift
            ;;
        *)
            echo "Unknown option: $arg"
            echo "Usage: $0 [--clean] [--debug] [--release]"
            exit 1
            ;;
    esac
done

echo ">>> STM32 Build Script"
echo ">>> Build Type: $BUILD_TYPE"

# Handle clean option - only clean, don't exit
if [ "$CLEAN" = true ]; then
    echo ">>> Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        echo ">>> Build directory removed."
    else
        echo ">>> Build directory already clean (does not exist)."
    fi
    echo ">>> Clean completed."
    # Don't exit - continue with build
fi

# Check if build directory exists, create if not
if [ -d "$BUILD_DIR" ]; then
    echo ">>> Build directory already exists — will reuse and rebuild..."
else
    echo ">>> Creating build directory..."
    mkdir "$BUILD_DIR"
fi

# Enter build directory
cd "$BUILD_DIR"

# Run CMake configure
echo ">>> Running cmake configure with $BUILD_TYPE mode..."
cmake .. -DCMAKE_TOOLCHAIN_FILE="$SCRIPT_DIR/cmake/gcc-arm-none-eabi.cmake" \
         -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# Run CMake build
echo ">>> Building project..."
cmake --build . --config "$BUILD_TYPE"

echo ">>> Build complete!"
echo ">>> Binary location: $BUILD_DIR/ABL2.elf (or .bin/.hex)"
