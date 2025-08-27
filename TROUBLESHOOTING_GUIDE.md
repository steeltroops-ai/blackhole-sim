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
```bash
# Verify GLFW path exists
ls external/glfw-3.4.bin.WIN64/

# If missing, restore from backup or re-download
# Check CMakeLists.txt GLFW_ROOT path
```

#### **Error: OpenGL not found**
```
CMake Error: Could not find OpenGL
```
**Solution:**
```bash
# Linux
sudo apt install libgl1-mesa-dev libglu1-mesa-dev

# Windows - Update graphics drivers
# macOS - Install Xcode command line tools
xcode-select --install
```

### **Compilation Errors**

#### **Error: C++17 features not supported**
```
error: 'std::optional' is not a member of 'std'
```
**Solution:**
```bash
# Verify compiler version
g++ --version  # Should be 7.0+
clang++ --version  # Should be 5.0+

# Update compiler if needed
sudo apt install gcc-9 g++-9  # Ubuntu
```

#### **Error: Missing header files**
```
fatal error: 'blackhole_sim/physics_engine.hpp' file not found
```
**Solution:**
```bash
# Verify include path structure
ls include/blackhole_sim/

# Check CMakeLists.txt include directories
# Ensure all header files are in correct locations
```

#### **Error: Undefined reference to OpenGL functions**
```
undefined reference to 'glClear'
```
**Solution:**
```bash
# Linux - Install OpenGL development libraries
sudo apt install libgl1-mesa-dev

# Windows - Verify OpenGL32.lib linking
# Check CMakeLists.txt OpenGL linking
```

## Runtime Issues

### **Application Startup Problems**

#### **Error: Failed to initialize GLFW**
```
ERROR: Failed to initialize GLFW
```
**Solution:**
```bash
# Check display environment (Linux)
echo $DISPLAY

# For headless systems, use virtual display
Xvfb :1 -screen 0 1024x768x24 &
export DISPLAY=:1

# Windows - Update graphics drivers
# Verify hardware acceleration is enabled
```

#### **Error: OpenGL context creation failed**
```
ERROR: Failed to create OpenGL context
```
**Solution:**
```bash
# Check OpenGL version support
glxinfo | grep "OpenGL version"  # Linux

# Update graphics drivers
# Try software rendering (testing only)
export LIBGL_ALWAYS_SOFTWARE=1

# Reduce OpenGL requirements in code if needed
```

#### **Error: Shader compilation failed**
```
ERROR: Fragment shader compilation failed
```
**Solution:**
```bash
# Check OpenGL version compatibility
# Verify shader source code syntax
# Enable shader debugging in code

# Run with verbose logging
./BlackHoleSimulation --debug --verbose-shaders
```

### **Performance Issues**

#### **Low Frame Rate (< 30 FPS)**
**Diagnosis:**
```bash
# Check GPU utilization
nvidia-smi  # NVIDIA
radeontop   # AMD

# Profile CPU usage
top -p $(pgrep BlackHoleSimulation)
```

**Solutions:**
```bash
# Reduce simulation complexity
./BlackHoleSimulation --particles=100 --quality=low

# Enable GPU acceleration
./BlackHoleSimulation --use-gpu

# Build with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3"
```

#### **High Memory Usage**
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
