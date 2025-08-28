# BlackHole Simulation - Troubleshooting Guide

## Quick Diagnostics

### **System Check**
```bash
# Verify system requirements
./BlackHoleSimulation --system-info

# Check OpenGL support
./BlackHoleSimulation --check-opengl

# Validate dependencies
./BlackHoleSimulation --check-deps
```

## Compilation Issues

### **CMake Configuration Problems**

#### **Error: CMake version too old**
```
CMake Error: CMake 3.16 or higher is required
```
**Solution:**
```bash
# Ubuntu/Debian
sudo apt remove cmake
sudo snap install cmake --classic

# Windows - Download from cmake.org
# macOS
brew upgrade cmake
```

#### **Error: GLFW not found**
```
CMake Error: Could not find GLFW library
```
**Solution:**
# Double-click this file for instant simulation (updated path):
scripts\launch_blackhole_simulation.bat
```
### **Option 2: Build from Source**

#### **Windows**
```batch
# Build the project (updated path)
scripts\build_windows.bat

# Run the simulation (updated path)
scripts\launch_blackhole_simulation.bat
```
#### **Linux/macOS/WSL**
```bash
# Build the project (updated path)
scripts/build.sh

# Run the simulation
cd build && ./BlackHoleSimulation
```
### **Option 3: Professional Development Setup**
```bash
# Complete setup with comprehensive documentation
git clone <repository-url>
cd blackhole-sim

# Read technical documentation
cat TECHNICAL_SPECIFICATION.md    # Architecture overview
cat BUILD_INSTRUCTIONS.md         # Detailed build guide
cat TROUBLESHOOTING_GUIDE.md      # Issue resolution

# Modern CMake build process
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
ctest --output-on-failure         # Run tests
```
## 📋 Requirements

### **System Requirements**

- **OS**: Windows 10+, Linux, macOS
- **Graphics**: OpenGL 3.3+ compatible GPU
- **RAM**: 4GB minimum, 8GB recommended
- **CPU**: Multi-core processor recommended

### **Development Requirements**

- **Compiler**: C++17 compatible (GCC 7+, Clang 5+, MSVC 2017+)
- **Build System**: CMake 3.10+
- **Graphics Library**: GLFW 3.3+ (included)
- **OpenGL Loader**: GLAD (included)

## 🎮 Interactive Controls & Features

### **🎬 Cinematic Experience**

The simulation features an automatic orbital camera that smoothly moves around the black hole, providing cinematic views of all physics effects. The camera intelligently adjusts angles and distance to showcase the most dramatic visual elements.

### **🎛️ Real-Time Interactive Controls**

**Visual Layer Toggles:**

- **G**: Toggle spacetime grid (curved spacetime visualization with anime-style glow)
- **A**: Toggle accretion disk (temperature-gradient disk: blue→yellow→orange→red)
- **P**: Toggle particles (three orbiting particles with realistic physics)
- **L**: Toggle gravitational lensing (Einstein's light ray bending effects)
- **B**: Toggle black hole visibility (event horizon with neon blue glowing edge)

**Animation Controls:**

- **SPACE**: Pause/Resume animation (freeze time for detailed observation)
- **H**: Show comprehensive help and controls list
- **ESC**: Exit simulation

### **🔬 Real-Time Physics Visualization**

- **Schwarzschild Metric**: Accurate spacetime curvature effects
- **Temperature Gradients**: Physics-based accretion disk coloring (T ∝ r^(-3/4))
- **Orbital Mechanics**: Stable, elliptical, and escape trajectories
- **Gravitational Redshift**: Color shifts near the event horizon
- **Light Deflection**: Einstein's δφ = 4GM/(c²b) equation visualized

## 🌌 Enhanced Visual Experience

### **🎨 Anime-Style Aesthetics with Scientific Accuracy**

**Visual Elements:**

1. **🌟 Cosmic Background**: Deep space environment with twinkling stars and subtle nebula colors
2. **⚫ Black Hole Event Horizon**: Pure black sphere with dramatic neon blue/cyan glowing edge
3. **🌀 Physics-Based Accretion Disk**:
   - **Outer regions**: Cool blue (lower temperature ~10⁴ K)
   - **Middle regions**: Warm yellow/orange (medium temperature ~10⁵ K)
   - **Inner regions**: Hot white/red (highest temperature ~10⁶ K)
   - **Realistic rotation** with Shakura-Sunyaev temperature profile
4. **📐 Curved Spacetime Grid**: Dramatically warped grid showing Einstein's spacetime curvature
   - **Teal-to-white gradient** with anime-style glow effects
   - **Extreme curvature** near photon sphere (1.5 Schwarzschild radii)
5. **🔴🔵🟡 Interactive Particles**: Three orbiting objects with distinct physics:
   - **Green**: Stable circular orbit with solid trails
   - **Orange**: Elliptical orbit with dynamic trajectories
   - **Electric Blue**: Distant orbit with escape potential
6. **💫 Gravitational Lensing**: Realistic light ray bending with redshift effects
   - **Blue light** farther from black hole
   - **Red light** closer to event horizon (gravitational redshift)
7. **🎥 Cinematic Camera**: Intelligent orbital movement showcasing all effects

### **🎭 Anime-Style Enhancements**

- **Cel-shading effects** for clean, outlined visuals
- **Bloom and glow** effects for high-energy phenomena
- **Vibrant color palette** with enhanced saturation
- **Smooth animations** with dramatic easing functions
- **Pulsing effects** for bright objects and energy sources

## 🔬 Scientific Accuracy

The simulation demonstrates **real physics** including:

- **Schwarzschild Metric Effects**: Accurate spacetime curvature
- **Gravitational Time Dilation**: Relativistic effects near the black hole
- **Light Ray Deflection**: Einstein's prediction of light bending
- **Orbital Mechanics**: Stable, elliptical, and escape trajectories
- **Accretion Disk Dynamics**: Temperature gradients and matter flow

## 📁 Project Structure
```
blackhole-sim/                           # Professional C++ Scientific Computing Structure
├── 📁 src/                              # Implementation files (.cpp)
│   ├── core/                           # Core engine components
│   │   ├── main.cpp                    # Application entry point
│   │   └── physics_engine.cpp          # Central physics coordination
│   ├── physics/                        # Physics simulation models
│   │   ├── black_hole.cpp              # Schwarzschild black hole implementation
│   │   ├── particle.cpp                # Massive particle dynamics
│   │   ├── light_ray.cpp               # Null geodesic calculations
│   │   └── accretion_disk.cpp          # Accretion disk physics
│   ├── rendering/                      # Graphics and visualization
│   │   ├── rendering_engine.cpp        # OpenGL rendering pipeline
│   │   ├── simple_rendering_engine.cpp # Anime-style effects
│   │   └── glad.c                      # OpenGL function loader
│   ├── simulation/                     # Simulation management
│   │   ├── simulation_manager.cpp      # Main simulation loop
│   │   └── input_system.cpp            # User input handling
│   └── utils/                          # Utility functions
├── 📁 include/blackhole_sim/           # Public header files (namespace-organized)
├── 📁 tests/                           # Testing framework
│   ├── unit/                          # Unit tests
│   ├── integration/                   # Integration tests
│   └── benchmarks/                    # Performance benchmarks
├── 📁 docs/                            # Documentation
│   ├── api/                           # API documentation
│   ├── physics/                       # Physics model documentation
│   └── examples/                      # Usage examples
├── 📁 assets/                          # Resources
│   ├── shaders/                       # GLSL shader files
│   ├── textures/                      # Texture assets
│   └── data/                          # Physics data files
├── 📁 scripts/                         # Build and utility scripts
│   ├── build_windows.bat             # Windows build script
│   ├── build.sh                      # Linux/macOS build script
│   └── launch_blackhole_simulation.bat # Windows launcher
├── 📁 external/                        # Third-party dependencies
│   └── glfw-3.4.bin.WIN64/           # GLFW library (Windows)
├── 📁 build/                           # Build artifacts (gitignored)
├── 🔧 CMakeLists.txt                   # Modern CMake configuration
├── 📋 TECHNICAL_SPECIFICATION.md       # Complete technical documentation
├── 🔨 BUILD_INSTRUCTIONS.md            # Comprehensive build guide
├── 🛠️ TROUBLESHOOTING_GUIDE.md         # Issue resolution guide
├── 📖 DEVELOPMENT.md                   # Development procedures
└── 📖 README.md                       # This file
Memory Usage**
**Diagnosis:**
```bash
# Monitor memory usage
valgrind --tool=massif ./BlackHoleSimulation
ms_print massif.out.* | head -30

# Check for memory leaks
valgrind --leak-check=full ./BlackHoleSimulation
```

**Solutions:**
```bash
# Reduce trajectory history
./BlackHoleSimulation --max-trajectory-points=500

# Limit particle count
./BlackHoleSimulation --max-particles=1000

# Enable memory optimizations in build
cmake .. -DCMAKE_BUILD_TYPE=Release -DOPTIMIZE_MEMORY=ON
```

### **Physics Simulation Issues**

#### **Particles Disappearing**
**Symptoms:** Particles vanish near black hole
**Solution:**
```cpp
// Check event horizon detection
if (blackHole.IsInsideEventHorizon(particle.GetPosition())) {
    // Particle crossed event horizon - expected behavior
}

// Verify numerical stability
// Reduce time step if needed
./BlackHoleSimulation --timestep=1e-6
```

#### **Unstable Orbits**
**Symptoms:** Particles spiral inward unexpectedly
**Solution:**
```bash
# Check integration method
./BlackHoleSimulation --integration=rk4

# Verify initial conditions
./BlackHoleSimulation --debug-physics

# Reduce time step for accuracy
./BlackHoleSimulation --timestep=1e-5
```

#### **Incorrect Gravitational Lensing**
**Symptoms:** Light rays don't bend correctly
**Solution:**
```bash
# Verify Schwarzschild radius calculation
# Check null geodesic integration
# Enable physics debugging
./BlackHoleSimulation --debug-geodesics
```

### **Visual Rendering Issues**

#### **Black Screen**
**Diagnosis:**
```bash
# Check OpenGL errors
./BlackHoleSimulation --check-gl-errors

# Verify shader compilation
./BlackHoleSimulation --debug-shaders
```

**Solutions:**
```bash
# Try different rendering mode
./BlackHoleSimulation --renderer=simple

# Disable advanced effects
./BlackHoleSimulation --no-bloom --no-glow

# Check graphics driver compatibility
```

#### **Incorrect Colors/Effects**
**Symptoms:** Missing anime-style effects
**Solution:**
```bash
# Enable anime rendering mode
./BlackHoleSimulation --anime-mode

# Check shader uniform values
./BlackHoleSimulation --debug-uniforms

# Verify color space settings
./BlackHoleSimulation --color-space=srgb
```

## Platform-Specific Issues

### **Windows**

#### **DLL Not Found Errors**
```
The program can't start because glfw3.dll is missing
```
**Solution:**
```batch
# Copy GLFW DLL to executable directory
copy external\glfw-3.4.bin.WIN64\lib-mingw-w64\glfw3.dll build\

# Or use static linking
cmake .. -DGLFW_USE_STATIC_LIBS=ON
```

#### **Visual Studio Build Issues**
```
MSB8020: The build tools for v142 cannot be found
```
**Solution:**
```batch
# Install correct Visual Studio version
# Or specify toolset explicitly
cmake .. -T v142
```

### **Linux**

#### **Missing X11 Libraries**
```
error while loading shared libraries: libX11.so.6
```
**Solution:**
```bash
# Install X11 development libraries
sudo apt install libx11-dev libxrandr-dev libxinerama-dev
sudo apt install libxcursor-dev libxi-dev
```

#### **Wayland Compatibility**
```
GLFW: Wayland: The platform does not support window positioning
```
**Solution:**
```bash
# Force X11 backend
export GDK_BACKEND=x11
./BlackHoleSimulation

# Or build with Wayland support
cmake .. -DGLFW_USE_WAYLAND=ON
```

### **macOS**

#### **Code Signing Issues**
```
"BlackHoleSimulation" cannot be opened because the developer cannot be verified
```
**Solution:**
```bash
# Allow unsigned applications
sudo spctl --master-disable

# Or sign the application
codesign -s - BlackHoleSimulation
```

#### **Metal vs OpenGL**
```
OpenGL is deprecated on macOS
```
**Solution:**
```bash
# Suppress deprecation warnings
export MACOSX_DEPLOYMENT_TARGET=10.14

# Or consider Metal backend (future enhancement)
```

## Debug Mode Features

### **Command Line Options**
```bash
# Enable comprehensive debugging
./BlackHoleSimulation --debug --verbose

# Physics debugging
./BlackHoleSimulation --debug-physics --debug-geodesics

# Rendering debugging  
./BlackHoleSimulation --debug-shaders --debug-uniforms

# Performance profiling
./BlackHoleSimulation --profile --benchmark
```

### **Log File Analysis**
```bash
# Enable file logging
./BlackHoleSimulation --log-file=debug.log

# Analyze common patterns
grep "ERROR" debug.log
grep "WARNING" debug.log
grep "PERFORMANCE" debug.log
```

## Getting Help

### **Information to Include**
1. **System Information**: OS, GPU, driver versions
2. **Build Configuration**: CMake version, compiler, flags
3. **Error Messages**: Complete error output
4. **Steps to Reproduce**: Exact sequence of actions
5. **Expected vs Actual**: What should happen vs what happens

### **Diagnostic Commands**
```bash
# System information
uname -a                    # Linux/macOS
systeminfo                 # Windows

# Graphics information
lspci | grep VGA           # Linux
system_profiler SPDisplaysDataType  # macOS

# Build information
cmake --version
g++ --version
./BlackHoleSimulation --version
```

This troubleshooting guide covers the most common issues encountered during development and deployment of the blackhole simulation, ensuring smooth operation across all supported platforms while maintaining both scientific accuracy and anime-inspired visual quality.
