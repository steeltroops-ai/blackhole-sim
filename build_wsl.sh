#!/bin/bash
# WSL-compatible build script for BlackHole Simulation Project
# This script builds the project using g++ with proper dependency management

set -e  # Exit on any error

echo "Building BlackHole Simulation Project (WSL Version)..."
echo "================================================="

# Configuration
COMPILER="g++"
FLAGS="-std=c++17 -Wall -Wextra -O2"
DEBUG_FLAGS="-std=c++17 -Wall -Wextra -g -O0 -DDEBUG"
INCLUDE_DIRS="-I./include -I./glfw-3.4.bin.WIN64/include -I./glad/include"
SRC_DIR="./src"
TEST_DIR="./tests"
BUILD_DIR="./build"
GLFW_LIB="./glfw-3.4.bin.WIN64/lib-mingw-w64"
LIBS="-L${GLFW_LIB} -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32"

# Source files
SOURCE_FILES=(
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/PhysicsEngine.cpp"
    "${SRC_DIR}/RenderingEngine.cpp"
    "${SRC_DIR}/InputSystem.cpp"
    "${SRC_DIR}/SimulationManager.cpp"
    "${SRC_DIR}/BlackHole.cpp"
    "${SRC_DIR}/Particle.cpp"
    "${SRC_DIR}/LightRay.cpp"
    "${SRC_DIR}/AccretionDisk.cpp"
    "${SRC_DIR}/glad.c"
)

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to compile individual source files
compile_source() {
    local source_file="$1"
    local output_file="$2"
    local compile_flags="$3"
    
    echo "Compiling: $(basename "$source_file")..."
    if $COMPILER $compile_flags $INCLUDE_DIRS -c "$source_file" -o "$output_file"; then
        echo "✓ $(basename "$source_file") compiled successfully"
        return 0
    else
        echo "✗ Failed to compile $(basename "$source_file")"
        return 1
    fi
}

# Function to link object files
link_executable() {
    local output_name="$1"
    shift
    local object_files=("$@")
    
    echo "Linking executable: $output_name..."
    if $COMPILER "${object_files[@]}" $LIBS -o "${BUILD_DIR}/$output_name"; then
        echo "✓ $output_name linked successfully"
        return 0
    else
        echo "✗ Failed to link $output_name"
        return 1
    fi
}

# Check prerequisites
echo "Checking prerequisites..."
if ! command_exists "$COMPILER"; then
    echo "Error: $COMPILER not found. Please install g++ or ensure it's in PATH."
    echo "On WSL Ubuntu: sudo apt update && sudo apt install build-essential"
    exit 1
fi
echo "✓ Compiler found: $(which $COMPILER)"

# Check for required directories and files
if [ ! -d "$SRC_DIR" ]; then
    echo "Error: Source directory '$SRC_DIR' not found"
    exit 1
fi

if [ ! -d "./include" ]; then
    echo "Error: Include directory './include' not found"
    exit 1
fi

if [ ! -d "./glfw-3.4.bin.WIN64" ]; then
    echo "Warning: GLFW directory not found. Some features may not work."
fi

echo "✓ Required directories found"

# Create build directory
echo ""
echo "=== Creating Build Directory ==="
mkdir -p "$BUILD_DIR"
echo "✓ Build directory ready: $BUILD_DIR"

# Parse command line arguments
BUILD_TYPE="release"
RUN_TESTS="false"
CLEAN_BUILD="false"

while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="debug"
            shift
            ;;
        --test)
            RUN_TESTS="true"
            shift
            ;;
        --clean)
            CLEAN_BUILD="true"
            shift
            ;;
        --help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  --debug    Build in debug mode"
            echo "  --test     Run tests after building"
            echo "  --clean    Clean build directory first"
            echo "  --help     Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Clean build if requested
if [ "$CLEAN_BUILD" = "true" ]; then
    echo ""
    echo "=== Cleaning Build Directory ==="
    rm -rf "${BUILD_DIR}"/*
    echo "✓ Build directory cleaned"
fi

# Set compilation flags based on build type
if [ "$BUILD_TYPE" = "debug" ]; then
    COMPILE_FLAGS="$DEBUG_FLAGS"
    echo "Building in DEBUG mode"
else
    COMPILE_FLAGS="$FLAGS"
    echo "Building in RELEASE mode"
fi

echo ""
echo "=== Compiling Source Files ==="

# Compile each source file to object file
OBJECT_FILES=()
for source_file in "${SOURCE_FILES[@]}"; do
    if [ -f "$source_file" ]; then
        # Generate object file name
        basename_file=$(basename "$source_file")
        object_file="${BUILD_DIR}/${basename_file%.*}.o"
        
        # Compile source to object
        if compile_source "$source_file" "$object_file" "$COMPILE_FLAGS"; then
            OBJECT_FILES+=("$object_file")
        else
            echo "Compilation failed. Stopping build."
            exit 1
        fi
    else
        echo "Warning: Source file not found: $source_file"
    fi
done

echo ""
echo "=== Linking Executable ==="

# Link the main executable
if link_executable "blackhole_simulation" "${OBJECT_FILES[@]}"; then
    echo "✓ Main executable built successfully"
else
    echo "✗ Failed to build main executable"
    exit 1
fi

# Build and run tests if requested
if [ "$RUN_TESTS" = "true" ]; then
    echo ""
    echo "=== Building and Running Tests ==="
    
    # Build minimal test
    if [ -f "${TEST_DIR}/minimal_test.cpp" ]; then
        echo "Building minimal test..."
        if $COMPILER $COMPILE_FLAGS $INCLUDE_DIRS "${TEST_DIR}/minimal_test.cpp" -o "${BUILD_DIR}/minimal_test"; then
            echo "✓ Minimal test compiled"
            echo "Running minimal test..."
            if "${BUILD_DIR}/minimal_test"; then
                echo "✓ Minimal test passed"
            else
                echo "✗ Minimal test failed"
            fi
        else
            echo "✗ Failed to compile minimal test"
        fi
    fi
    
    # Build physics test
    if [ -f "${TEST_DIR}/physics_test.cpp" ]; then
        echo "Building physics test..."
        if $COMPILER $COMPILE_FLAGS $INCLUDE_DIRS "${TEST_DIR}/physics_test.cpp" -o "${BUILD_DIR}/physics_test"; then
            echo "✓ Physics test compiled"
            echo "Running physics test..."
            if "${BUILD_DIR}/physics_test"; then
                echo "✓ Physics test passed"
            else
                echo "✗ Physics test failed"
            fi
        else
            echo "✗ Failed to compile physics test"
        fi
    fi
fi

echo ""
echo "=== Build Summary ==="
echo "Build type: $BUILD_TYPE"
echo "Compiler: $COMPILER"
echo "Flags: $COMPILE_FLAGS"
echo "Output directory: $BUILD_DIR"
echo "Executable: ${BUILD_DIR}/blackhole_simulation"

if [ -f "${BUILD_DIR}/blackhole_simulation" ]; then
    echo ""
    echo "✓ Build completed successfully!"
    echo "To run the simulation: ./${BUILD_DIR}/blackhole_simulation"
    
    # Check for GLFW DLL and provide instructions
    if [ -f "./glfw-3.4.bin.WIN64/lib-mingw-w64/glfw3.dll" ]; then
        if [ ! -f "${BUILD_DIR}/glfw3.dll" ]; then
            echo ""
            echo "Note: Copying GLFW DLL for runtime..."
            cp "./glfw-3.4.bin.WIN64/lib-mingw-w64/glfw3.dll" "${BUILD_DIR}/" 2>/dev/null || echo "Warning: Could not copy GLFW DLL"
        fi
    fi
else
    echo ""
    echo "✗ Build failed!"
    exit 1
fi

echo ""
echo "Build script completed."