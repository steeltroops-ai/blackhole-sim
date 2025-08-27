# BlackHole Simulation - Development Guide

## 🚀 Quick Start

### Prerequisites

**Required:**
- CMake 3.16+
- C++17 compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
- OpenGL 3.3+ support
- Git

**Platform-Specific:**

**Windows:**
```bash
# Using Chocolatey
choco install cmake mingw git

# Or using Visual Studio 2022 with C++ workload
```

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake ninja-build git
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
sudo apt-get install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

**macOS:**
```bash
brew install cmake ninja git
xcode-select --install
```

### Build Instructions

**1. Clone Repository:**
```bash
git clone https://github.com/steeltroops-ai/blackhole-sim.git
cd blackhole-sim
```

**2. Configure Build:**
```bash
# Debug build (recommended for development)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Release build (optimized)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# With Ninja (faster builds)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

**3. Build:**
```bash
cmake --build build --parallel
```

**4. Run:**
```bash
# From build directory
cd build
./BlackHoleSimulation

# Or directly
./build/BlackHoleSimulation
```

## 🏗️ Project Structure

```
blackhole-sim/
├── src/                    # Source files
│   ├── main.cpp           # Application entry point
│   ├── PhysicsEngine.cpp  # Core physics calculations
│   ├── RenderingEngine.cpp # OpenGL rendering
│   ├── SimulationManager.cpp # Main simulation loop
│   └── ...
├── include/               # Header files
│   ├── PhysicsEngine.hpp
│   ├── RenderingEngine.hpp
│   └── ...
├── tests/                 # Unit and integration tests
├── docs/                  # Documentation
├── glfw-3.4.bin.WIN64/   # GLFW library (Windows)
├── CMakeLists.txt        # Build configuration
└── README.md
```

## 🧪 Testing

### Running Tests
```bash
# All tests
cd build
ctest --output-on-failure

# Specific test
ctest -R physics_test

# Verbose output
ctest --verbose
```

### Test Categories
- **Unit Tests:** Individual component testing
- **Physics Tests:** Numerical accuracy validation
- **Integration Tests:** Component interaction testing
- **Performance Tests:** Benchmarking and profiling

## 🔧 Development Tools

### Static Analysis
```bash
# clang-tidy (if available)
cmake --build build --target clang-tidy

# cppcheck
cppcheck --enable=all --std=c++17 src/ include/
```

### Code Formatting
```bash
# clang-format (if available)
cmake --build build --target format

# Manual formatting
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

### Debugging
```bash
# Debug build with sanitizers
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined"
cmake --build build-debug
./build-debug/BlackHoleSimulation
```

### Memory Analysis
```bash
# Valgrind (Linux/macOS)
valgrind --tool=memcheck --leak-check=full ./build/BlackHoleSimulation

# AddressSanitizer (built-in)
# Already enabled in Debug builds
```

## 🐳 Docker Development

### Build Container
```bash
# Development container
docker build --target development -t blackhole-sim:dev .

# Production container
docker build --target runtime -t blackhole-sim:latest .
```

### Run Container
```bash
# Interactive development
docker run -it --rm -v $(pwd):/workspace blackhole-sim:dev

# Run simulation
docker run --rm -v $(pwd)/data:/data blackhole-sim:latest
```

## 📊 Performance Profiling

### CPU Profiling
```bash
# perf (Linux)
perf record -g ./build/BlackHoleSimulation
perf report

# gprof
cmake -B build-profile -DCMAKE_CXX_FLAGS="-pg"
cmake --build build-profile
./build-profile/BlackHoleSimulation
gprof ./build-profile/BlackHoleSimulation gmon.out > analysis.txt
```

### GPU Profiling
```bash
# NVIDIA Nsight (if available)
nsight-compute ./build/BlackHoleSimulation

# Intel VTune (if available)
vtune -collect hotspots ./build/BlackHoleSimulation
```

## 🔬 Physics Validation

### Analytical Tests
The simulation includes validation against known analytical solutions:

- **Schwarzschild Geodesics:** Particle trajectories around black holes
- **Photon Sphere:** Light ray behavior at r = 1.5 * Rs
- **Event Horizon:** Proper behavior at r = Rs
- **Conservation Laws:** Energy and angular momentum conservation

### Numerical Accuracy
- **Integration Methods:** RK4, adaptive timestep control
- **Precision:** Double precision floating point
- **Stability:** CFL condition enforcement

## 🎨 Rendering System

### Graphics Pipeline
1. **Vertex Processing:** 3D transformations
2. **Geometry Shaders:** Particle system generation
3. **Fragment Shaders:** Anime-style effects, bloom
4. **Post-Processing:** HDR tone mapping, anti-aliasing

### Shader Development
```bash
# Shaders located in src/shaders/ (if separated)
# Hot-reload during development (if implemented)
```

## 🚨 Troubleshooting

### Common Issues

**Build Fails - GLFW Not Found:**
```bash
# Ensure GLFW path is correct in CMakeLists.txt
# Windows: Check glfw-3.4.bin.WIN64/ directory exists
# Linux: Install libglfw3-dev
sudo apt-get install libglfw3-dev
```

**OpenGL Context Creation Fails:**
```bash
# Check graphics drivers
# Ensure OpenGL 3.3+ support
# Try software rendering: LIBGL_ALWAYS_SOFTWARE=1
```

**Simulation Diverges:**
```bash
# Reduce timestep in configuration
# Check initial conditions
# Verify numerical stability
```

**Performance Issues:**
```bash
# Build in Release mode
# Check GPU utilization
# Profile with tools mentioned above
```

### Debug Builds
Debug builds include:
- Address Sanitizer (detects memory errors)
- Undefined Behavior Sanitizer
- Debug symbols for GDB/LLDB
- Assertion checks
- Verbose logging

### Getting Help
1. Check existing issues on GitHub
2. Run with `--verbose` flag for detailed output
3. Include system information and build configuration
4. Provide minimal reproduction case

## 📈 Contributing

### Code Style
- Follow existing code style
- Use clang-format for consistency
- Add unit tests for new features
- Update documentation

### Pull Request Process
1. Fork repository
2. Create feature branch
3. Make changes with tests
4. Run full test suite
5. Submit pull request with description

### Performance Considerations
- Profile before optimizing
- Maintain numerical accuracy
- Consider memory usage
- Test on target hardware

## 📚 Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [OpenGL Reference](https://www.opengl.org/sdk/docs/)
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [General Relativity Physics](https://en.wikipedia.org/wiki/General_relativity)

## 🏷️ Version Information

- **CMake:** 3.16+
- **C++ Standard:** C++17
- **OpenGL:** 3.3+
- **GLFW:** 3.4
- **Compiler Support:** GCC 9+, Clang 10+, MSVC 2019+
