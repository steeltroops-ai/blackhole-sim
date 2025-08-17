#!/bin/bash

# Black Hole Simulation - WSL Build Script
# This script compiles the entire project using g++ in WSL

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Black Hole Simulation Build Script${NC}"
echo -e "${BLUE}===================================${NC}"

# Configuration
CXX="g++"
CXXFLAGS="-std=c++11 -Wall -Wextra -O2 -g"
INCLUDE_DIR="include"
SRC_DIR="src"
BUILD_DIR="build"
TARGET="blackhole-sim"

# Create build directory
echo -e "${YELLOW}Creating build directory...${NC}"
mkdir -p "$BUILD_DIR"

# Source files to compile
SOURCE_FILES=(
    "$SRC_DIR/main.cpp"
    "$SRC_DIR/AccretionDisk.cpp"
    "$SRC_DIR/BlackHole.cpp"
    "$SRC_DIR/InputSystem.cpp"
    "$SRC_DIR/LightRay.cpp"
    "$SRC_DIR/Particle.cpp"
    "$SRC_DIR/PhysicsEngine.cpp"
    "$SRC_DIR/RenderingEngine.cpp"
    "$SRC_DIR/SimulationManager.cpp"
    "$SRC_DIR/glad.c"
)

# Check if all source files exist
echo -e "${YELLOW}Checking source files...${NC}"
for file in "${SOURCE_FILES[@]}"; do
    if [ ! -f "$file" ]; then
        echo -e "${RED}Error: Source file $file not found!${NC}"
        exit 1
    fi
    echo -e "${GREEN}✓${NC} Found: $file"
done

# Check if include directory exists
if [ ! -d "$INCLUDE_DIR" ]; then
    echo -e "${RED}Error: Include directory $INCLUDE_DIR not found!${NC}"
    exit 1
fi

# Compile each source file to object file
echo -e "${YELLOW}Compiling source files...${NC}"
OBJECT_FILES=()
for src_file in "${SOURCE_FILES[@]}"; do
    # Get filename without path and extension
    filename=$(basename "$src_file" .cpp)
    obj_file="$BUILD_DIR/$filename.o"
    
    echo -e "${BLUE}Compiling $src_file...${NC}"
    if $CXX $CXXFLAGS -I"$INCLUDE_DIR" -c "$src_file" -o "$obj_file"; then
        echo -e "${GREEN}✓${NC} Compiled: $obj_file"
        OBJECT_FILES+=("$obj_file")
    else
        echo -e "${RED}✗${NC} Failed to compile: $src_file"
        exit 1
    fi
done

# Link object files into executable
echo -e "${YELLOW}Linking executable...${NC}"
if $CXX $CXXFLAGS "${OBJECT_FILES[@]}" -o "$BUILD_DIR/$TARGET"; then
    echo -e "${GREEN}✓${NC} Successfully created executable: $BUILD_DIR/$TARGET"
else
    echo -e "${RED}✗${NC} Failed to link executable"
    exit 1
fi

# Check if executable was created and is executable
if [ -x "$BUILD_DIR/$TARGET" ]; then
    echo -e "${GREEN}Build completed successfully!${NC}"
    echo -e "${BLUE}Executable location: $BUILD_DIR/$TARGET${NC}"
    echo -e "${BLUE}To run: ./$BUILD_DIR/$TARGET${NC}"
else
    echo -e "${RED}Error: Executable was not created or is not executable${NC}"
    exit 1
fi

echo -e "${GREEN}Build process finished.${NC}"