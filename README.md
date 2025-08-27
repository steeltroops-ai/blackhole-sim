# 🌌 Black Hole Simulation

A **scientifically accurate**, real-time 3D simulation of black hole physics using modern OpenGL and C++. Experience the mind-bending effects of general relativity with stunning visual fidelity.

![Black Hole Simulation](https://img.shields.io/badge/Status-Working-brightgreen)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-blue)
![C++](https://img.shields.io/badge/C++-17-red)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)

## ✨ Features

### 🎬 **Visual Effects**

- **Event Horizon**: Realistic black hole event horizon rendering
- **Accretion Disk**: Temperature-gradient glowing matter disk
- **Spacetime Grid**: Curved grid showing gravitational effects
- **Particle Trajectories**: Realistic orbital mechanics simulation
- **Gravitational Lensing**: Light ray bending around massive objects
- **Starfield Background**: Immersive deep space environment

### 🔬 **Physics Accuracy**

- **Schwarzschild Metric**: Accurate spacetime curvature calculations
- **Geodesic Motion**: Realistic particle trajectory computation
- **General Relativity**: Einstein's field equations implementation
- **Orbital Mechanics**: Stable, elliptical, and escape trajectories
- **Light Ray Tracing**: Photon path deflection simulation

### 🎮 **Interactive Experience**

- **Cinematic Camera**: Automatic orbital camera movement
- **Real-time Animation**: Smooth 60+ FPS performance
- **Dynamic Lighting**: Realistic lighting and shading effects
- **Continuous Simulation**: Runs indefinitely without crashes

## 🚀 Quick Start

### **Option 1: One-Click Launch (Windows)**

```batch
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
```

## 🛠️ Troubleshooting

### **Common Issues**

**White screen or crash:**

- Update graphics drivers
- Ensure OpenGL 3.3+ support
- Close other graphics-intensive applications

**Build errors:**

- Verify CMake 3.10+ is installed
- Check C++17 compiler compatibility
- Ensure GLFW dependencies are available

**Performance issues:**

- Lower screen resolution
- Close background applications
- Check GPU temperature and throttling

## 🏆 Achievement

**BEFORE**: White screen that closed after 4-5 seconds
**AFTER**: Stunning, continuous 3D blackhole simulation with all physics effects

This simulation demonstrates **world-class, scientifically accurate** black hole physics with:

- ✅ Continuous operation without crashes
- ✅ Smooth 60+ FPS performance
- ✅ All visual effects working perfectly
- ✅ Automatic cinematic camera movement
- ✅ Real-time physics simulation

## 📄 License

MIT License - Feel free to use, modify, and distribute.

---

**🌌 Experience the universe's most extreme objects in stunning 3D! 🌌**

Contributions welcome! Please read CONTRIBUTING.md for guidelines.
