# Black Hole Simulation - Architecture Design

## Overview

This document describes the modular C++ architecture for the black hole simulation project. The design emphasizes modularity, performance, and maintainability through clear separation of concerns and well-defined interfaces.

## Core Design Principles

1. **Modularity**: Each subsystem is self-contained with minimal dependencies
2. **Performance**: Optimized for real-time physics simulation and rendering
3. **Extensibility**: Easy to add new physics models and rendering features
4. **Testability**: Clear interfaces enable comprehensive unit testing
5. **Cross-platform**: Compatible with Windows, Linux, and macOS

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    SimulationManager                        │
│  - Main simulation loop                                     │
│  - Subsystem coordination                                   │
│  - Configuration management                                 │
└─────────────────┬───────────────┬───────────────┬─────────┘
                  │               │               │
┌─────────────────▼─────────────┐ │ ┌─────────────▼─────────────┐
│        PhysicsEngine          │ │ │       RenderingEngine      │
│  - Geodesic integration       │ │ │  - OpenGL rendering        │
│  - Gravitational lensing      │ │ │  - Shader management       │
│  - Redshift calculations      │ │ │  - Camera control          │
│  - Collision detection        │ │ │  - Visual effects          │
└───────────────────────────────┘ │ └───────────────────────────┘
                                  │
                ┌─────────────────▼─────────────┐
                │          InputSystem          │
                │  - Keyboard/mouse handling    │
                │  - Action mapping             │
                │  - Configuration binding      │
                └───────────────────────────────┘
```

## Core Modules

### 1. SimulationManager

**Responsibility**: Central coordinator for the entire simulation

**Key Features**:
- Main simulation loop with fixed timestep physics
- Subsystem initialization and cleanup
- Configuration file management
- Performance monitoring and profiling
- Scene management and object lifecycle

**Interface**:
```cpp
class SimulationManager {
public:
    bool Initialize(const Config& config);
    void Run();
    void Shutdown();
    
    void Pause();
    void Resume();
    void Reset();
    void Stop();
    
    void LoadScene(const std::string& filename);
    void SaveScene(const std::string& filename);
    
    // Object management
    void AddParticle(const Particle& particle);
    void AddLightRay(const LightRay& ray);
    void RemoveObject(ObjectID id);
    
private:
    std::unique_ptr<PhysicsEngine> m_physics;
    std::unique_ptr<RenderingEngine> m_renderer;
    std::unique_ptr<InputSystem> m_input;
    
    bool m_running = false;
    bool m_paused = false;
    double m_timeStep = 1.0/60.0;
};
```

### 2. PhysicsEngine

**Responsibility**: All physics calculations and simulations

**Key Features**:
- Schwarzschild metric calculations
- Geodesic integration (Euler, RK4, RKF45)
- Gravitational lensing and ray tracing
- Redshift and time dilation effects
- Conservation law monitoring
- Collision detection and response

**Interface**:
```cpp
class PhysicsEngine {
public:
    bool Initialize(const BlackHole& blackHole);
    void Shutdown();
    
    void Update(double deltaTime);
    void UpdateParticles(double deltaTime);
    void UpdateLightRays(double deltaTime);
    
    // Integration methods
    void SetIntegrationMethod(IntegrationMethod method);
    void SetTimeStep(double timeStep);
    void SetTolerance(double tolerance);
    
    // Object management
    void AddParticle(std::shared_ptr<Particle> particle);
    void AddLightRay(std::shared_ptr<LightRay> ray);
    void RemoveObject(ObjectID id);
    
    // Physics queries
    double CalculateRedshift(const Vector3& position, const Vector3& velocity);
    double CalculateTimeDilation(double radius);
    bool IsInsideEventHorizon(double radius) const;
    
private:
    BlackHole m_blackHole;
    std::vector<std::shared_ptr<Particle>> m_particles;
    std::vector<std::shared_ptr<LightRay>> m_lightRays;
    std::unique_ptr<AccretionDisk> m_accretionDisk;
    
    IntegrationMethod m_integrationMethod = IntegrationMethod::RK4;
    double m_timeStep = 1e-4;
    double m_tolerance = 1e-8;
};
```

### 3. RenderingEngine

**Responsibility**: All visualization and graphics rendering

**Key Features**:
- OpenGL-based rendering pipeline
- Shader management and compilation
- Multi-layer rendering system
- Camera control (orbital, free-fly, first-person)
- Post-processing effects
- Screenshot and video capture

**Interface**:
```cpp
class RenderingEngine {
public:
    bool Initialize(int width, int height, const std::string& title);
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    void Present();
    
    // Rendering layers
    void SetRenderLayer(RenderLayer layer, bool enabled);
    void SetQualityLevel(QualityLevel level);
    
    // Camera control
    void SetCameraMode(CameraMode mode);
    void UpdateCamera(const CameraState& state);
    
    // Object rendering
    void RenderParticles(const std::vector<Particle>& particles);
    void RenderLightRays(const std::vector<LightRay>& rays);
    void RenderAccretionDisk(const AccretionDisk& disk);
    void RenderBlackHole(const BlackHole& blackHole);
    
    // Utility
    void TakeScreenshot(const std::string& filename);
    bool ShouldClose() const;
    
private:
    GLFWwindow* m_window = nullptr;
    std::unique_ptr<ShaderManager> m_shaderManager;
    std::unique_ptr<Camera> m_camera;
    
    // Render layers
    std::bitset<static_cast<size_t>(RenderLayer::COUNT)> m_enabledLayers;
    QualityLevel m_qualityLevel = QualityLevel::HIGH;
};
```

### 4. InputSystem

**Responsibility**: User input handling and action mapping

**Key Features**:
- Keyboard and mouse input processing
- Configurable key bindings
- Action-based input system
- Mouse sensitivity and acceleration
- Gamepad support (future)

**Interface**:
```cpp
class InputSystem {
public:
    bool Initialize(GLFWwindow* window);
    void Shutdown();
    
    void Update();
    void ProcessInput();
    
    // Action binding
    void BindKey(Key key, Action action);
    void BindMouseButton(MouseButton button, Action action);
    void UnbindAction(Action action);
    
    // State queries
    bool IsActionPressed(Action action) const;
    bool IsActionJustPressed(Action action) const;
    bool IsActionReleased(Action action) const;
    
    // Mouse control
    Vector2 GetMousePosition() const;
    Vector2 GetMouseDelta() const;
    void SetMouseSensitivity(float sensitivity);
    
    // Configuration
    void LoadKeyBindings(const std::string& filename);
    void SaveKeyBindings(const std::string& filename);
    
private:
    GLFWwindow* m_window = nullptr;
    std::unordered_map<Key, Action> m_keyBindings;
    std::unordered_map<MouseButton, Action> m_mouseBindings;
    
    std::bitset<static_cast<size_t>(Action::COUNT)> m_currentActions;
    std::bitset<static_cast<size_t>(Action::COUNT)> m_previousActions;
    
    Vector2 m_mousePosition{0.0f, 0.0f};
    Vector2 m_mouseDelta{0.0f, 0.0f};
    float m_mouseSensitivity = 1.0f;
};
```

## Data Models

### Core Physics Objects

```cpp
// Base interface for all physics objects
class IPhysicsObject {
public:
    virtual ~IPhysicsObject() = default;
    virtual ObjectID GetID() const = 0;
    virtual ObjectType GetType() const = 0;
    virtual Vector3 GetPosition() const = 0;
    virtual void Update(double deltaTime) = 0;
};

// Base interface for renderable objects
class IRenderable {
public:
    virtual ~IRenderable() = default;
    virtual void Render(const RenderContext& context) = 0;
    virtual BoundingBox GetBoundingBox() const = 0;
    virtual bool IsVisible() const = 0;
};
```

### Specialized Classes

**BlackHole**: Central massive object defining the spacetime geometry
- Mass, spin (future), charge (future)
- Metric calculations and derived quantities
- Event horizon and photon sphere properties

**Particle**: Massive objects following timelike geodesics
- Position, velocity, mass, charge
- Trajectory history and orbital parameters
- Rendering properties (color, size, trail)

**LightRay**: Photons following null geodesics
- Position, direction, frequency, intensity
- Lensing calculations and redshift tracking
- Color mapping and visualization

**AccretionDisk**: Matter distribution around the black hole
- Density and temperature profiles
- Emission spectrum and luminosity
- Relativistic effects and Doppler shifts

## Data Flow Architecture

```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│ User Input  │───▶│ InputSystem │───▶│SimulationMgr│
└─────────────┘    └─────────────┘    └─────────────┘
                                              │
                                              ▼
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Display   │◀───│RenderingEng │◀───│PhysicsEngine│
└─────────────┘    └─────────────┘    └─────────────┘
                           ▲                   │
                           │                   ▼
                   ┌─────────────┐    ┌─────────────┐
                   │   Shaders   │    │   Objects   │
                   │   Textures  │    │ (Particles, │
                   │   Meshes    │    │ Light Rays) │
                   └─────────────┘    └─────────────┘
```

## Performance Considerations

### Memory Management
- Object pooling for frequently created/destroyed objects
- Smart pointers for automatic memory management
- Cache-friendly data layouts for physics calculations
- Minimal dynamic allocations in hot paths

### Multithreading
- Physics calculations on dedicated thread
- Rendering on main thread with OpenGL context
- Input processing on separate thread (future)
- Thread-safe communication via message queues

### GPU Acceleration
- Compute shaders for parallel ray tracing
- Vertex buffer objects for efficient geometry rendering
- Texture streaming for large datasets
- Instanced rendering for particle systems

## Configuration System

### Configuration File Structure
```json
{
  "simulation": {
    "timeStep": 1e-4,
    "integrationMethod": "RK4",
    "tolerance": 1e-8,
    "maxParticles": 10000,
    "maxLightRays": 50000
  },
  "blackHole": {
    "mass": 1e6,
    "position": [0, 0, 0],
    "spin": 0.0
  },
  "rendering": {
    "width": 1920,
    "height": 1080,
    "fullscreen": false,
    "vsync": true,
    "msaa": 4,
    "qualityLevel": "HIGH"
  },
  "input": {
    "mouseSensitivity": 1.0,
    "keyBindings": {
      "SPACE": "PAUSE",
      "R": "RESET",
      "ESC": "QUIT"
    }
  }
}
```

## Error Handling Strategy

### Exception Hierarchy
```cpp
class SimulationException : public std::exception {
public:
    explicit SimulationException(const std::string& message);
    const char* what() const noexcept override;
};

class PhysicsException : public SimulationException {
    // Physics-specific errors (integration failures, etc.)
};

class RenderingException : public SimulationException {
    // OpenGL errors, shader compilation failures, etc.
};

class ConfigurationException : public SimulationException {
    // Configuration file parsing errors, invalid parameters, etc.
};
```

### Error Recovery
- Graceful degradation for non-critical failures
- Automatic fallback to lower quality settings
- Comprehensive logging with different severity levels
- User-friendly error messages and suggestions

## Testing Strategy

### Unit Tests
- Physics calculations (geodesic integration accuracy)
- Mathematical functions (metric components, Christoffel symbols)
- Coordinate transformations
- Conservation law verification

### Integration Tests
- Complete simulation scenarios
- Performance benchmarks
- Memory leak detection
- Cross-platform compatibility

### Validation Tests
- Comparison with analytical solutions
- Known physics test cases
- Visual regression testing for rendering

## Build System

### CMake Configuration
- Cross-platform build support
- Dependency management (OpenGL, GLFW, GLM)
- Optional features (CUDA, OpenMP)
- Testing framework integration
- Documentation generation

### Dependencies
- **Required**: OpenGL 3.3+, GLFW 3.3+, GLM 0.9.9+
- **Optional**: CUDA 11.0+ (GPU acceleration), OpenMP (multithreading)
- **Testing**: Google Test, Google Benchmark
- **Documentation**: Doxygen

## Future Extensions

### Planned Features
1. **Kerr Black Holes**: Rotating black hole support
2. **Multiple Black Holes**: Binary systems and mergers
3. **Electromagnetic Fields**: Charged particle dynamics
4. **Advanced Rendering**: Volumetric rendering, HDR, bloom
5. **VR Support**: Virtual reality visualization
6. **Network Simulation**: Distributed computing support

### Extensibility Points
- Plugin system for custom physics models
- Scripting interface (Lua/Python)
- Custom shader support
- External data import/export
- Real-time parameter adjustment

This architecture provides a solid foundation for a high-performance, extensible black hole simulation while maintaining clean separation of concerns and testability.