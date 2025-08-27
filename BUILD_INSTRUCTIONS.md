# BlackHole Simulation - Build Instructions

## Quick Start

### **Windows (Recommended)**
```batch
# Clone and build
git clone <repository-url>
cd blackhole-sim
scripts\build_windows.bat

# Run simulation
scripts\launch_blackhole_simulation.bat
```

### **Linux/macOS**
```bash
# Clone and build
git clone <repository-url>
cd blackhole-sim
chmod +x scripts/build.sh
scripts/build.sh

# Run simulation
./build/BlackHoleSimulation
```

## Detailed Build Process

### **Prerequisites Installation**

#### **Windows**

**Option 1: Visual Studio (Recommended)**
1. Install Visual Studio 2019+ with C++ development tools
2. Install CMake 3.16+ from https://cmake.org/download/
3. Ensure Windows SDK is installed

**Option 2: MinGW-w64**
1. Install MSYS2 from https://www.msys2.org/
2. Install build tools:
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-cmake
   pacman -S mingw-w64-x86_64-make
   ```
3. Add MinGW-w64 to PATH: `C:\msys64\mingw64\bin`

#### **Linux (Ubuntu/Debian)**
```bash
# Essential build tools
sudo apt update
sudo apt install build-essential cmake git

# Graphics development libraries
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
sudo apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

# Optional: Additional development tools
sudo apt install clang-tidy cppcheck valgrind
```

#### **Linux (CentOS/RHEL/Fedora)**
```bash
# Essential build tools
sudo dnf install gcc-c++ cmake git make

# Graphics development libraries  
sudo dnf install mesa-libGL-devel mesa-libGLU-devel
sudo dnf install libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel

# Optional: Additional development tools
sudo dnf install clang-tools-extra cppcheck valgrind
```

#### **macOS**
```bash
# Install Xcode command line tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake
brew install cmake

# Optional: Additional development tools
brew install clang-format cppcheck
```

### **Build Configuration**

#### **Standard Build (Release)**
```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build project
cmake --build . --config Release

# Run tests (optional)
ctest --output-on-failure
```

#### **Debug Build**
```bash
# Configure for debugging
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build with debug symbols
cmake --build . --config Debug

# Run with debugging enabled
./BlackHoleSimulation --debug
```

#### **Cross-Platform Build Options**
```bash
# Windows with Visual Studio
cmake .. -G "Visual Studio 16 2019" -A x64

# Windows with MinGW
cmake .. -G "MinGW Makefiles"

# Linux with Ninja (faster builds)
cmake .. -G Ninja

# macOS with Xcode
cmake .. -G Xcode
```

### **Advanced Build Options**

#### **Performance Optimization**
```bash
# Maximum optimization
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native"

# Link-time optimization
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON

# OpenMP parallel processing (if available)
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_OPENMP=ON
```

#### **Development Build**
```bash
# Enable all warnings and static analysis
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic" \
         -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Enable sanitizers (Linux/macOS)
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined"
```

### **Build Verification**

#### **Test Execution**
```bash
# Run all tests
cd build
ctest --verbose

# Run specific test categories
ctest -R unit_tests
ctest -R integration_tests
ctest -R benchmarks
```

#### **Performance Validation**
```bash
# Run performance benchmarks
./build/benchmarks/test_physics_performance

# Profile with system tools (Linux)
perf record -g ./build/BlackHoleSimulation
perf report

# Memory analysis (Linux)
valgrind --tool=memcheck --leak-check=full ./build/BlackHoleSimulation
```

### **Troubleshooting**

#### **Common Build Issues**

**CMake Configuration Fails**
```bash
# Clear CMake cache
rm -rf build/
mkdir build && cd build

# Specify compiler explicitly
cmake .. -DCMAKE_CXX_COMPILER=g++

# Check CMake version
cmake --version  # Should be 3.16+
```

**GLFW Linking Errors (Windows)**
```bash
# Verify GLFW path in CMakeLists.txt
# Should point to: external/glfw-3.4.bin.WIN64/

# Try different linking approach
cmake .. -DGLFW_USE_STATIC_LIBS=OFF
```

**OpenGL Context Creation Fails**
```bash
# Update graphics drivers
# Verify OpenGL support
glxinfo | grep "OpenGL version"  # Linux
# Windows: Use GPU-Z or similar tool

# Try software rendering (testing only)
export LIBGL_ALWAYS_SOFTWARE=1  # Linux
```

**Missing Dependencies (Linux)**
```bash
# Install missing development packages
sudo apt install libglfw3-dev libglew-dev  # Ubuntu/Debian
sudo dnf install glfw-devel glew-devel     # CentOS/RHEL/Fedora
```

#### **Runtime Issues**

**Simulation Crashes on Startup**
```bash
# Run with debugging
./BlackHoleSimulation --debug --verbose

# Check graphics capabilities
./BlackHoleSimulation --check-opengl

# Verify file permissions
chmod +x BlackHoleSimulation
```

**Poor Performance**
```bash
# Verify release build
file BlackHoleSimulation  # Should show "not stripped" for debug

# Check GPU utilization
nvidia-smi  # NVIDIA GPUs
radeontop   # AMD GPUs

# Reduce simulation complexity
./BlackHoleSimulation --particles=100 --quality=medium
```

### **IDE Integration**

#### **Visual Studio Code**
```json
// .vscode/settings.json
{
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.generator": "Ninja",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

#### **CLion**
1. Open project folder in CLion
2. CLion will automatically detect CMakeLists.txt
3. Configure build profiles in Settings → Build, Execution, Deployment → CMake

#### **Visual Studio**
1. Open folder in Visual Studio
2. Visual Studio will detect CMakeLists.txt automatically
3. Configure CMake settings in Project → CMake Settings

### **Continuous Integration**

#### **GitHub Actions Example**
```yaml
name: Build and Test
on: [push, pull_request]
jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
    steps:
    - uses: actions/checkout@v3
    - name: Configure CMake
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release
    - name: Build
      run: cmake --build build --config Release
    - name: Test
      run: cd build && ctest --output-on-failure
```

This comprehensive build guide ensures successful compilation across all supported platforms while maintaining the project's scientific accuracy and anime-inspired visual quality.
