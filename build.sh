#!/bin/bash

# Black Hole Simulation - Cross-Platform Build Script
# This script builds the project using CMake for better cross-platform support

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
BUILD_DIR="build"
BUILD_TYPE="Release"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --clean)
            echo -e "${YELLOW}Cleaning build directory...${NC}"
            rm -rf "$BUILD_DIR"
            shift
            ;;
        --help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  --debug    Build in debug mode"
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

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: CMake not found. Please install CMake.${NC}"
    exit 1
fi

# Create build directory
echo -e "${YELLOW}Creating build directory...${NC}"
mkdir -p "$BUILD_DIR"

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
if cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"; then
    echo -e "${GREEN}✓${NC} CMake configuration successful"
else
    echo -e "${RED}✗${NC} CMake configuration failed"
    exit 1
fi

# Build with CMake
echo -e "${YELLOW}Building with CMake...${NC}"
if cmake --build "$BUILD_DIR" --config "$BUILD_TYPE"; then
    echo -e "${GREEN}✓${NC} Build successful"
else
    echo -e "${RED}✗${NC} Build failed"
    exit 1
fi

# Check if executable was created
if [ -f "$BUILD_DIR/BlackHoleSimulation" ] || [ -f "$BUILD_DIR/BlackHoleSimulation.exe" ]; then
    echo -e "${GREEN}Build completed successfully!${NC}"
    echo -e "${BLUE}Executable location: $BUILD_DIR/BlackHoleSimulation${NC}"
    echo -e "${BLUE}To run: cd $BUILD_DIR && ./BlackHoleSimulation${NC}"
else
    echo -e "${RED}Error: Executable was not created${NC}"
    exit 1
fi

echo -e "${GREEN}Build process finished.${NC}"