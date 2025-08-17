# Black Hole Simulation

A high-performance C++ simulation of black hole physics featuring gravitational lensing, geodesic calculations, and real-time visualization.

## Features

- **Accurate Physics**: Implements Schwarzschild metric for non-rotating black holes
- **Geodesic Integration**: Runge-Kutta 4th order integration for particle and light ray trajectories
- **Gravitational Lensing**: Real-time ray-tracing with redshift calculations
- **Accretion Disk**: Physically-based emission modeling
- **Interactive Visualization**: OpenGL-based rendering with user controls
- **Performance Optimized**: Multi-threaded physics with optional GPU acceleration

## Project Structure

```
black_hole_sim/
├── include/                 # Header files
│   ├── PhysicsEngine.hpp    # Physics calculations (geodesics, lensing)
│   ├── RenderingEngine.hpp  # OpenGL rendering
│   ├── InputSystem.hpp      # User input handling
│   ├── SimulationManager.hpp# Main simulation loop
│   ├── BlackHole.hpp        # Black hole properties
│   ├── Particle.hpp         # Massive particles/stars
│   ├── LightRay.hpp         # Light paths for lensing
│   └── AccretionDisk.hpp    # Accretion disk model
├── src/                     # Source files
├── tests/                   # Unit and integration tests
├── docs/                    # Documentation
└── CMakeLists.txt           # Build system
```

## Dependencies

- **CMake** (>= 3.16)
- **C++17** compatible compiler
- **OpenGL** (>= 3.3)
- **GLFW3** for windowing
- **OpenMP** (optional, for parallelization)
- **CUDA** (optional, for GPU acceleration)

## Building

### Linux/macOS
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake build-essential libglfw3-dev libgl1-mesa-dev

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Windows
```powershell
# Using vcpkg for dependencies
vcpkg install glfw3 opengl

# Build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

## Usage

```bash
./BlackHoleSimulation
```

### Controls
- **Space**: Toggle simulation pause
- **R**: Reset simulation
- **1-5**: Toggle rendering layers (grid, trajectories, accretion disk, etc.)
- **Mouse**: Camera control
- **WASD**: Navigation

## Physics Model

The simulation implements:

1. **Schwarzschild Metric**: Describes spacetime curvature around a non-rotating black hole
2. **Geodesic Equations**: Calculates trajectories of massive particles and photons
3. **Gravitational Lensing**: Ray-tracing through curved spacetime
4. **Redshift Effects**: Gravitational and Doppler redshift calculations
5. **Accretion Disk**: Shakura-Sunyaev model for disk physics

## Testing

```bash
# Run unit tests
make test
# or
./RunTests
```

## Documentation

Detailed documentation is available in the `docs/` directory:
- `physics_model.md`: Mathematical foundations and equations
- `architecture.md`: Code structure and design patterns
- `usage.md`: User guide and examples

## Performance

- **Multi-threading**: OpenMP parallelization for physics calculations
- **GPU Acceleration**: Optional CUDA support for ray-tracing
- **Optimized Integration**: Adaptive step-size Runge-Kutta methods
- **Memory Efficient**: Object pooling for particles and rays

## References

- Misner, Thorne, Wheeler: "Gravitation" (1973)
- Shakura & Sunyaev: "Black holes in binary systems" (1973)
- Chandrasekhar: "The Mathematical Theory of Black Holes" (1983)

## License

MIT License - see LICENSE file for details.

## Contributing

Contributions welcome! Please read CONTRIBUTING.md for guidelines.