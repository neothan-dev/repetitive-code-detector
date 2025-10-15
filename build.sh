#!/bin/bash

echo "Building RepetitiveCodeDetector..."

# Default behavior: build both platforms
BUILD_LINUX=true
BUILD_WINDOWS=true

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --linux-only)
            BUILD_LINUX=true
            BUILD_WINDOWS=false
            shift
            ;;
        --windows-only)
            BUILD_LINUX=false
            BUILD_WINDOWS=true
            shift
            ;;
        --help|-h)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  --linux-only     Build only for Linux"
            echo "  --windows-only   Build only for Windows"
            echo "  --help, -h       Show this help message"
            echo ""
            echo "Default: Build for both Linux and Windows"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to build for Linux
build_linux() {
    echo "Building for Linux..."
    
    # Create build directory for Linux
    mkdir -p build/linux
    cd build/linux
    
    # Configure with CMake
    echo "Configuring project for Linux..."
    cmake ../.. -DCMAKE_BUILD_TYPE=Release
    if [ $? -ne 0 ]; then
        echo "CMake configuration failed for Linux!"
        exit 1
    fi
    
    # Build the project
    echo "Building project for Linux..."
    make -j$(nproc)
    if [ $? -ne 0 ]; then
        echo "Build failed for Linux!"
        exit 1
    fi
    
    echo "Linux build completed successfully!"
    echo "Linux executable location: build/linux/bin/RepetitiveCodeDetector"
    cd ../..
}

# Function to build for Windows
build_windows() {
    echo "Building for Windows..."
    
    # Check if mingw-w64 is available
    if ! command_exists x86_64-w64-mingw32-g++ && ! command_exists i686-w64-mingw32-g++; then
        echo "Warning: MinGW-w64 not found. Installing..."
        echo "Please install mingw-w64:"
        echo "  Ubuntu/Debian: sudo apt-get install mingw-w64"
        echo "  CentOS/RHEL: sudo yum install mingw64-gcc-c++"
        echo "  Arch Linux: sudo pacman -S mingw-w64-gcc"
        echo ""
        echo "Skipping Windows build..."
        return 1
    fi
    
    # Create build directory for Windows
    mkdir -p build/windows
    cd build/windows
    
    # Determine which MinGW compiler to use
    MINGW_COMPILER=""
    if command_exists x86_64-w64-mingw32-g++; then
        MINGW_COMPILER="x86_64-w64-mingw32"
    elif command_exists i686-w64-mingw32-g++; then
        MINGW_COMPILER="i686-w64-mingw32"
    fi
    
    if [ -z "$MINGW_COMPILER" ]; then
        echo "No suitable MinGW compiler found!"
        cd ../..
        return 1
    fi
    
    echo "Using MinGW compiler: $MINGW_COMPILER"
    
    # Configure with CMake for Windows
    echo "Configuring project for Windows..."
    cmake ../.. \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_C_COMPILER=$MINGW_COMPILER-gcc \
        -DCMAKE_CXX_COMPILER=$MINGW_COMPILER-g++ \
        -DCMAKE_RC_COMPILER=$MINGW_COMPILER-windres \
        -DCMAKE_FIND_ROOT_PATH=/usr/$MINGW_COMPILER \
        -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
        -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
        -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
        -DCMAKE_CXX_FLAGS="-static-libgcc -static-libstdc++" \
        -DCMAKE_EXE_LINKER_FLAGS="-static"
    if [ $? -ne 0 ]; then
        echo "CMake configuration failed for Windows!"
        cd ../..
        return 1
    fi
    
    # Build the project
    echo "Building project for Windows..."
    make -j$(nproc)
    if [ $? -ne 0 ]; then
        echo "Build failed for Windows!"
        cd ../..
        return 1
    fi
    
    echo "Windows build completed successfully!"
    echo "Windows executable location: build/windows/bin/RepetitiveCodeDetector.exe"
    cd ../..
}

# Main build process
echo "Starting build process..."

# Build for Linux if requested
if [ "$BUILD_LINUX" = true ]; then
    build_linux
fi

# Build for Windows if requested
if [ "$BUILD_WINDOWS" = true ]; then
    build_windows
fi

echo ""
echo "Build process completed!"
echo "Available executables:"
if [ -f "build/linux/bin/RepetitiveCodeDetector" ]; then
    echo "  Linux: build/linux/bin/RepetitiveCodeDetector"
fi
if [ -f "build/windows/bin/RepetitiveCodeDetector.exe" ]; then
    echo "  Windows: build/windows/bin/RepetitiveCodeDetector.exe"
fi
