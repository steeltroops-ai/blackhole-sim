/**
 * @file SimulationManager.cpp
 * @brief Implementation of the SimulationManager class
 */

#include "SimulationManager.hpp"
#include "PhysicsEngine.hpp"
#include "RenderingEngine.hpp"
#include "InputSystem.hpp"
#include "BlackHole.hpp"
#include "Particle.hpp"
#include "LightRay.hpp"
#include "AccretionDisk.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Gravitational constant in m^3 kg^-1 s^-2
static const double G = 6.67430e-11;

#ifdef _WIN32
#include <windows.h>
#endif

namespace BlackHoleSim {

SimulationManager::SimulationManager()
    : m_physicsEngine(nullptr)
    , m_renderingEngine(nullptr)
    , m_inputSystem(nullptr)
    , m_blackHole(nullptr)
    , m_accretionDisk(nullptr)
    , m_isInitialized(false)
    , m_isRunning(false)
    , m_isPaused(false)
    , m_timeStep(0.01)
    , m_simulationTime(0.0)
    , m_frameCount(0)
    , m_lastFrameTime(std::chrono::high_resolution_clock::now())
    , m_deltaTime(0.0)
    , m_fps(0.0)
    , m_targetFPS(60.0)
    , m_maxTimeStep(0.1)
    , m_minTimeStep(1e-6)
    , m_performanceMode(PerformanceMode::BALANCED)
    , m_showPerformanceStats(false)
{
}

SimulationManager::~SimulationManager() {
    Shutdown();
}

bool SimulationManager::Initialize(const SimulationConfig& config) {
    if (m_isInitialized) {
        std::cerr << "Warning: SimulationManager already initialized\n";
        return true;
    }

    std::cout << "Initializing SimulationManager...\n";
    
    // Store configuration
    m_config = config;
    
    try {
        // Initialize subsystems
        if (!InitializeSubsystems()) {
            std::cerr << "Error: Failed to initialize subsystems\n";
            return false;
        }
        
        // Load configuration if specified
        if (!config.configFile.empty()) {
            if (!LoadConfiguration(config.configFile)) {
                std::cerr << "Warning: Failed to load configuration file: " << config.configFile << "\n";
            }
        }
        
        // Setup default scene
        SetupDefaultScene();
        
        // Setup input callbacks
        SetupInputCallbacks();
        
        m_isInitialized = true;
        std::cout << "SimulationManager initialized successfully\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error during initialization: " << e.what() << "\n";
        Shutdown();
        return false;
    }
}

bool SimulationManager::InitializeSubsystems() {
    // Initialize Physics Engine
    std::cout << "  Initializing Physics Engine...\n";
    m_physicsEngine.reset(new PhysicsEngine());
    // Initialize without black hole for now - will be set later in SetupDefaultScene
    if (!m_physicsEngine->Initialize(nullptr)) {
        std::cerr << "    Failed to initialize Physics Engine\n";
        return false;
    }
    
    // Initialize Rendering Engine
    std::cout << "  Initializing Rendering Engine...\n";
    m_renderingEngine.reset(new RenderingEngine());
    if (!m_renderingEngine->Initialize(m_config.windowWidth, m_config.windowHeight, 
                                       "Black Hole Simulation")) {
        std::cerr << "    Failed to initialize Rendering Engine\n";
        return false;
    }
    
    // Configure rendering settings
    // VSync and MSAA methods are now available in RenderingEngine
    
    // Initialize Input System
    std::cout << "  Initializing Input System...\n";
    m_inputSystem.reset(new InputSystem());
    if (!m_inputSystem->Initialize(m_renderingEngine->GetWindow())) {
        std::cerr << "    Failed to initialize Input System\n";
        return false;
    }
    
    return true;
}



void SimulationManager::SetupDefaultScene() {
    std::cout << "  Setting up default scene...\n";
    
    // Create black hole (1 solar mass at origin)
    const double solarMass = 1.989e30; // kg
    m_blackHole.reset(new BlackHole(solarMass, {0.0, 0.0, 0.0}));
    
    // Now initialize physics engine with the black hole
    if (m_physicsEngine && m_blackHole) {
        m_physicsEngine->Initialize(std::shared_ptr<BlackHole>(m_blackHole.get(), [](BlackHole*){}));
    }
    
    // Create accretion disk
    double innerRadius = 3.0 * m_blackHole->GetSchwarzschildRadius();
    double outerRadius = 20.0 * m_blackHole->GetSchwarzschildRadius();
    double accretionRate = 0.1 * solarMass;
    // Create AccretionDisk with proper parameters
    AccretionDisk::ModelType model = AccretionDisk::ModelType::SHAKURA_SUNYAEV;
    double alpha = 0.1;
    m_accretionDisk.reset(new AccretionDisk(innerRadius, outerRadius, accretionRate, model, alpha));
    
    // Add some test particles
    AddTestParticles();
    
    // Add some light rays for lensing demonstration
    AddTestLightRays();
    
    // Set initial camera position
    auto& camera = m_renderingEngine->GetCamera();
    double radius = 50.0 * m_blackHole->GetSchwarzschildRadius();
    camera.position = {0.0f, 0.0f, static_cast<float>(radius)};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
}

void SimulationManager::AddTestParticles() {
    const double rs = m_blackHole->GetSchwarzschildRadius();
    const double testMass = 1e20; // kg (small test mass)
    
    // Add particles in stable circular orbits
    std::vector<double> radii = {6.0 * rs, 10.0 * rs, 15.0 * rs, 25.0 * rs};
    
    for (size_t i = 0; i < radii.size(); ++i) {
        double r = radii[i];
        double angle = (2.0 * M_PI * i) / radii.size();
        
        // Position in circular orbit
        Vector3 position(r * std::cos(angle), r * std::sin(angle), 0.0);
        
        // Circular orbital velocity
        double v_circular = std::sqrt(m_blackHole->GetMass() * G / r);
        Vector3 velocity(-v_circular * std::sin(angle), v_circular * std::cos(angle), 0.0);
        
        std::array<double, 3> pos = {position.x(), position.y(), position.z()};
        std::array<double, 3> vel = {velocity.x(), velocity.y(), velocity.z()};
        Particle particle(testMass, pos, vel);
        particle.SetType(Particle::Type::TEST_PARTICLE);
        particle.SetColor({1.0f, 0.8f, 0.2f, 1.0f}); // Golden color (RGBA)
        particle.SetSize(2.0f);
        // TODO: Implement particle trail length if needed
        
        m_particles.push_back(particle);
    }
    
    std::cout << "    Added " << m_particles.size() << " test particles\n";
}

void SimulationManager::AddTestLightRays() {
    const double rs = m_blackHole->GetSchwarzschildRadius();
    
    // Add light rays from different impact parameters
    std::vector<double> impactParams = {2.5 * rs, 3.0 * rs, 4.0 * rs, 6.0 * rs, 10.0 * rs};
    
    for (size_t i = 0; i < impactParams.size(); ++i) {
        double b = impactParams[i];
        double startDistance = 100.0 * rs;
        
        // Position light ray far from black hole
        Vector3 position(-startDistance, b, 0.0);
        Vector3 direction(1.0, 0.0, 0.0); // Moving toward black hole
        
        std::array<double, 3> pos = {position.x(), position.y(), position.z()};
        std::array<double, 3> dir = {direction.x(), direction.y(), direction.z()};
        LightRay lightRay(pos, dir, 5e14); // Green light frequency
        lightRay.SetType(LightRay::Type::LENSED);
        lightRay.SetFrequency(5e14); // Green light
        lightRay.SetIntensity(1.0);
        lightRay.SetMaxPathLength(2000);
        
        m_lightRays.push_back(lightRay);
    }
    
    std::cout << "    Added " << m_lightRays.size() << " test light rays\n";
}

void SimulationManager::SetupInputCallbacks() {
    // Set input callback for actions
    m_inputSystem->SetInputCallback([this](InputSystem::Action action, float value) {
        HandleInputAction(action, value);
    });
    
    // Set mouse callback for camera control
    m_inputSystem->SetMouseCallback([this](double x, double y, double deltaX, double deltaY) {
        if (!m_inputSystem->IsCursorEnabled()) {
            HandleCameraRotation(deltaX, deltaY);
        }
        (void)x; (void)y; // Suppress unused parameter warnings
    });
    
    // Set scroll callback for zoom
    m_inputSystem->SetScrollCallback([this](double xOffset, double yOffset) {
        HandleCameraZoom(yOffset);
        (void)xOffset; // Suppress unused parameter warning
    });
    
    // Setup default key bindings
    m_inputSystem->ResetToDefaults();
}

void SimulationManager::HandleInputAction(InputSystem::Action action, float value) {
    (void)value; // Suppress unused parameter warning
    switch (action) {
        case InputSystem::Action::TOGGLE_PAUSE:
            TogglePause();
            break;
            
        case InputSystem::Action::RESET_SIMULATION:
            Reset();
            break;
            
        case InputSystem::Action::STEP_SIMULATION:
            if (m_isPaused) {
                Step();
            }
            break;
            
        case InputSystem::Action::INCREASE_TIME_STEP:
            SetTimeStep(m_timeStep * 1.1);
            break;
            
        case InputSystem::Action::DECREASE_TIME_STEP:
            SetTimeStep(m_timeStep * 0.9);
            break;
            
        case InputSystem::Action::CAMERA_FORWARD:
        case InputSystem::Action::CAMERA_BACKWARD:
        case InputSystem::Action::CAMERA_LEFT:
        case InputSystem::Action::CAMERA_RIGHT:
        case InputSystem::Action::CAMERA_UP:
        case InputSystem::Action::CAMERA_DOWN:
            HandleCameraMovement(action, value);
            break;
            
        case InputSystem::Action::CAMERA_RESET:
            ResetCamera();
            break;
            
        case InputSystem::Action::TOGGLE_CAMERA_MODE:
            ToggleCameraMode();
            break;
            
        case InputSystem::Action::TOGGLE_GRID:
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::SPACETIME_GRID);
            break;
            
        case InputSystem::Action::TOGGLE_PARTICLES:
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::PARTICLES);
            break;
            
        case InputSystem::Action::TOGGLE_TRAILS:
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::PARTICLE_TRAILS);
            break;
            
        case InputSystem::Action::TOGGLE_LIGHT_RAYS:
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::LIGHT_RAYS);
            break;
            
        case InputSystem::Action::TOGGLE_ACCRETION_DISK:
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::ACCRETION_DISK);
            break;
            
        case InputSystem::Action::TOGGLE_BLACK_HOLE:
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::BLACK_HOLE);
            break;
            
        case InputSystem::Action::TOGGLE_PHOTON_SPHERE:
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::PHOTON_SPHERE);
            break;
            
        case InputSystem::Action::TOGGLE_UI:
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::UI_OVERLAY);
            break;
            
        case InputSystem::Action::TOGGLE_FULLSCREEN:
            // TODO: Implement fullscreen toggle
            break;
            
        case InputSystem::Action::SPAWN_PARTICLE:
            SpawnParticle();
            break;
            
        case InputSystem::Action::SPAWN_LIGHT_RAY:
            SpawnLightRay();
            break;
            
        case InputSystem::Action::TAKE_SCREENSHOT:
            TakeScreenshot();
            break;
            
        case InputSystem::Action::EXIT_APPLICATION:
            Stop();
            break;
            
        default:
            break;
    }
}

int SimulationManager::Run() {
    if (!m_isInitialized) {
        std::cerr << "Error: SimulationManager not initialized\n";
        return -1;
    }
    
    m_isRunning = true;
    
    std::cout << "Starting simulation loop...\n";
    
    // Main simulation loop
    while (m_isRunning && !m_renderingEngine->ShouldClose()) {
        // Update timing
        UpdateTiming();
        
        // Process input
        m_inputSystem->Update(m_deltaTime);
        
        // Update physics (if not paused)
        if (!m_isPaused) {
            UpdatePhysics();
        }
        
        // Render frame
        Render();
        
        // Update performance statistics
        UpdatePerformanceStats();
        
        // Limit frame rate if needed
        LimitFrameRate();
    }
    
    std::cout << "Simulation loop ended\n";
    return 0;
}

void SimulationManager::UpdateTiming() {
    auto now = std::chrono::high_resolution_clock::now();
    
    if (m_frameCount == 0) {
        // First frame, initialize last frame time
        m_lastFrameTime = now;
        m_deltaTime = 1.0 / 60.0; // Default to 60 FPS
    } else {
        // Calculate delta time
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastFrameTime);
        m_deltaTime = duration.count() / 1000000.0; // Convert to seconds
        m_lastFrameTime = now;
    }
    
    // Clamp delta time to prevent large jumps
    m_deltaTime = std::min(m_deltaTime, 1.0 / 30.0); // Max 30 FPS minimum
    
    m_frameCount++;
}

void SimulationManager::UpdatePhysics() {
    if (!m_physicsEngine) return;
    
    // TODO: PhysicsEngine::Update expects vectors of unique_ptr, but we store objects
    // For now, we'll just update the simulation time and accretion disk
    // The physics integration will need to be implemented when we fix the storage types
    
    // Update accretion disk
    if (m_accretionDisk && m_blackHole) {
        m_accretionDisk->Update(m_timeStep, m_blackHole->GetMass());
    }
    
    // Advance simulation time
    m_simulationTime += m_timeStep;
}

void SimulationManager::Render() {
    if (!m_renderingEngine) return;
    
    // Begin frame
    m_renderingEngine->BeginFrame();
    
    // Convert particle and light ray storage to unique_ptr vectors for rendering
    // Create copies in unique_ptrs to match the rendering interface
    std::vector<std::unique_ptr<Particle>> particlePtrs;
    std::vector<std::unique_ptr<LightRay>> lightRayPtrs;
    
    for (const auto& particle : m_particles) {
        particlePtrs.emplace_back(std::make_unique<Particle>(particle));
    }
    
    for (const auto& lightRay : m_lightRays) {
        lightRayPtrs.emplace_back(std::make_unique<LightRay>(lightRay));
    }
    
    // Render all objects using the main render method
    if (m_blackHole) {
        m_renderingEngine->Render(*m_blackHole, particlePtrs, lightRayPtrs, m_accretionDisk.get());
    }
    
    // Render UI
    RenderUI();
    
    // End frame
    m_renderingEngine->EndFrame();
}

void SimulationManager::RenderUI() {
    // Render performance stats if enabled
    if (m_showPerformanceStats) {
        // TODO: Implement text rendering in RenderingEngine
        // For now, just output to console occasionally
        static int frameCounter = 0;
        if (++frameCounter % 60 == 0) { // Every 60 frames
            std::cout << "FPS: " << static_cast<int>(m_fps) << std::endl;
        }
        // TODO: Add more UI elements when text rendering is implemented
    }
    
    // TODO: Render pause indicator when text rendering is available
    if (m_isPaused) {
        // For now, pause state is shown in console output
    }
}

void SimulationManager::UpdatePerformanceStats() {
    static auto lastStatsUpdate = std::chrono::high_resolution_clock::now();
    static int framesSinceLastUpdate = 0;
    
    framesSinceLastUpdate++;
    
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastStatsUpdate).count() / 1000.0;
    
    if (duration >= 1.0) { // Update every second
        m_fps = framesSinceLastUpdate / duration;
        lastStatsUpdate = now;
        framesSinceLastUpdate = 0;
    }
}

void SimulationManager::LimitFrameRate() {
    if (m_targetFPS > 0.0) {
        double targetFrameTime = 1.0 / m_targetFPS;
        double frameTime = m_deltaTime;
        
        if (frameTime < targetFrameTime) {
            double sleepTime = targetFrameTime - frameTime;
#ifdef _WIN32
            Sleep(static_cast<DWORD>(sleepTime * 1000.0)); // Windows Sleep takes milliseconds
#else
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
#endif
        }
    }
}

void SimulationManager::Shutdown() {
    if (!m_isInitialized) return;
    
    std::cout << "Shutting down SimulationManager...\n";
    
    m_isRunning = false;
    
    // Clear objects
    m_particles.clear();
    m_lightRays.clear();
    m_accretionDisk.reset();
    m_blackHole.reset();
    
    // Shutdown subsystems
    if (m_inputSystem) {
        m_inputSystem->Shutdown();
        m_inputSystem.reset();
    }
    
    if (m_renderingEngine) {
        m_renderingEngine->Shutdown();
        m_renderingEngine.reset();
    }
    
    if (m_physicsEngine) {
        // PhysicsEngine doesn't require explicit shutdown
        m_physicsEngine.reset();
    }
    
    m_isInitialized = false;
    std::cout << "SimulationManager shutdown complete\n";
}

void SimulationManager::TogglePause() {
    m_isPaused = !m_isPaused;
    std::cout << (m_isPaused ? "Simulation paused\n" : "Simulation resumed\n");
}

void SimulationManager::Step() {
    if (m_isPaused) {
        UpdatePhysics();
        std::cout << "Simulation stepped (t = " << m_simulationTime << ")\n";
    }
}

void SimulationManager::Reset() {
    std::cout << "Resetting simulation...\n";
    
    // Reset simulation time
    m_simulationTime = 0.0;
    
    // Clear and recreate objects
    m_particles.clear();
    m_lightRays.clear();
    
    // Setup default scene again
    SetupDefaultScene();
    
    // Reset camera
    ResetCamera();
    
    std::cout << "Simulation reset complete\n";
}

void SimulationManager::Stop() {
    std::cout << "Stopping simulation...\n";
    m_isRunning = false;
}

void SimulationManager::ResetCamera() {
    if (m_renderingEngine && m_blackHole) {
        auto& camera = m_renderingEngine->GetCamera();
        double radius = 50.0 * m_blackHole->GetSchwarzschildRadius();
        camera.position = {0.0f, 0.0f, static_cast<float>(radius)};
        camera.target = {0.0f, 0.0f, 0.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        std::cout << "Camera reset\n";
    }
}

void SimulationManager::HandleCameraMovement(InputSystem::Action action, float value) {
    if (!m_renderingEngine) return;
    
    auto& camera = m_renderingEngine->GetCamera();
    const float moveSpeed = 10.0f * m_deltaTime; // Units per second
    
    // Calculate camera forward, right, and up vectors
    float forward[3] = {
        camera.target[0] - camera.position[0],
        camera.target[1] - camera.position[1],
        camera.target[2] - camera.position[2]
    };
    
    // Normalize forward vector
    float forwardLength = std::sqrt(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    if (forwardLength > 0.0f) {
        forward[0] /= forwardLength;
        forward[1] /= forwardLength;
        forward[2] /= forwardLength;
    }
    
    // Calculate right vector (cross product of forward and up)
    float right[3] = {
        forward[1] * camera.up[2] - forward[2] * camera.up[1],
        forward[2] * camera.up[0] - forward[0] * camera.up[2],
        forward[0] * camera.up[1] - forward[1] * camera.up[0]
    };
    
    switch (action) {
        case InputSystem::Action::CAMERA_FORWARD:
            camera.position[0] += forward[0] * moveSpeed;
            camera.position[1] += forward[1] * moveSpeed;
            camera.position[2] += forward[2] * moveSpeed;
            camera.target[0] += forward[0] * moveSpeed;
            camera.target[1] += forward[1] * moveSpeed;
            camera.target[2] += forward[2] * moveSpeed;
            break;
            
        case InputSystem::Action::CAMERA_BACKWARD:
            camera.position[0] -= forward[0] * moveSpeed;
            camera.position[1] -= forward[1] * moveSpeed;
            camera.position[2] -= forward[2] * moveSpeed;
            camera.target[0] -= forward[0] * moveSpeed;
            camera.target[1] -= forward[1] * moveSpeed;
            camera.target[2] -= forward[2] * moveSpeed;
            break;
            
        case InputSystem::Action::CAMERA_LEFT:
            camera.position[0] -= right[0] * moveSpeed;
            camera.position[1] -= right[1] * moveSpeed;
            camera.position[2] -= right[2] * moveSpeed;
            camera.target[0] -= right[0] * moveSpeed;
            camera.target[1] -= right[1] * moveSpeed;
            camera.target[2] -= right[2] * moveSpeed;
            break;
            
        case InputSystem::Action::CAMERA_RIGHT:
            camera.position[0] += right[0] * moveSpeed;
            camera.position[1] += right[1] * moveSpeed;
            camera.position[2] += right[2] * moveSpeed;
            camera.target[0] += right[0] * moveSpeed;
            camera.target[1] += right[1] * moveSpeed;
            camera.target[2] += right[2] * moveSpeed;
            break;
            
        case InputSystem::Action::CAMERA_UP:
            camera.position[0] += camera.up[0] * moveSpeed;
            camera.position[1] += camera.up[1] * moveSpeed;
            camera.position[2] += camera.up[2] * moveSpeed;
            camera.target[0] += camera.up[0] * moveSpeed;
            camera.target[1] += camera.up[1] * moveSpeed;
            camera.target[2] += camera.up[2] * moveSpeed;
            break;
            
        case InputSystem::Action::CAMERA_DOWN:
            camera.position[0] -= camera.up[0] * moveSpeed;
            camera.position[1] -= camera.up[1] * moveSpeed;
            camera.position[2] -= camera.up[2] * moveSpeed;
            camera.target[0] -= camera.up[0] * moveSpeed;
            camera.target[1] -= camera.up[1] * moveSpeed;
            camera.target[2] -= camera.up[2] * moveSpeed;
            break;
            
        default:
            break;
    }
}

void SimulationManager::HandleCameraRotation(double deltaX, double deltaY) {
    if (!m_renderingEngine) return;
    
    auto& camera = m_renderingEngine->GetCamera();
    const float sensitivity = 0.002f;
    
    // Calculate current direction vector
    float direction[3] = {
        camera.target[0] - camera.position[0],
        camera.target[1] - camera.position[1],
        camera.target[2] - camera.position[2]
    };
    
    // Convert to spherical coordinates for rotation
    float radius = std::sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);
    if (radius < 0.001f) radius = 1.0f;
    
    float yaw = std::atan2(direction[0], direction[2]);
    float pitch = std::asin(direction[1] / radius);
    
    // Apply rotation
    yaw += deltaX * sensitivity;
    pitch -= deltaY * sensitivity;
    
    // Clamp pitch to avoid gimbal lock
    const float maxPitch = M_PI * 0.49f;
    pitch = std::max(-maxPitch, std::min(maxPitch, pitch));
    
    // Convert back to Cartesian coordinates
    direction[0] = radius * std::sin(yaw) * std::cos(pitch);
    direction[1] = radius * std::sin(pitch);
    direction[2] = radius * std::cos(yaw) * std::cos(pitch);
    
    // Update target
    camera.target[0] = camera.position[0] + direction[0];
    camera.target[1] = camera.position[1] + direction[1];
    camera.target[2] = camera.position[2] + direction[2];
}

void SimulationManager::HandleCameraZoom(double yOffset) {
    if (!m_renderingEngine) return;
    
    auto& camera = m_renderingEngine->GetCamera();
    const float zoomSpeed = 5.0f;
    
    // Calculate direction to target
    float direction[3] = {
        camera.target[0] - camera.position[0],
        camera.target[1] - camera.position[1],
        camera.target[2] - camera.position[2]
    };
    
    // Normalize direction
    float length = std::sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);
    if (length > 0.001f) {
        direction[0] /= length;
        direction[1] /= length;
        direction[2] /= length;
        
        // Move camera along direction
        float zoomAmount = yOffset * zoomSpeed;
        camera.position[0] += direction[0] * zoomAmount;
        camera.position[1] += direction[1] * zoomAmount;
        camera.position[2] += direction[2] * zoomAmount;
    }
}

void SimulationManager::ToggleCameraMode() {
    if (!m_renderingEngine) return;
    
    auto& camera = m_renderingEngine->GetCamera();
    
    switch (camera.mode) {
        case RenderingEngine::CameraMode::FREE_LOOK:
            camera.mode = RenderingEngine::CameraMode::ORBIT;
            std::cout << "Camera mode: ORBIT\n";
            break;
            
        case RenderingEngine::CameraMode::ORBIT:
            camera.mode = RenderingEngine::CameraMode::FOLLOW_PARTICLE;
            std::cout << "Camera mode: FOLLOW_PARTICLE\n";
            break;
            
        case RenderingEngine::CameraMode::FOLLOW_PARTICLE:
            camera.mode = RenderingEngine::CameraMode::FIXED;
            std::cout << "Camera mode: FIXED\n";
            break;
            
        case RenderingEngine::CameraMode::FIXED:
            camera.mode = RenderingEngine::CameraMode::FREE_LOOK;
            std::cout << "Camera mode: FREE_LOOK\n";
            break;
    }
}

void SimulationManager::SpawnParticle() {
    // Spawn particle at camera position with random velocity
    if (m_renderingEngine && m_blackHole) {
        const auto& camera = m_renderingEngine->GetCamera();
        std::array<double, 3> cameraPos = {camera.position[0], camera.position[1], camera.position[2]};
        std::array<double, 3> randomVel = {
            0.1 * (rand() / double(RAND_MAX) - 0.5),
            0.1 * (rand() / double(RAND_MAX) - 0.5),
            0.1 * (rand() / double(RAND_MAX) - 0.5)
        };
        
        Particle particle(1e20, cameraPos, randomVel);
        // Note: SetType, SetColor, SetSize, SetTrailLength methods may not exist
        // particle.SetType(Particle::Type::TEST_PARTICLE);
        // particle.SetColor(Vector3(1.0, 0.5, 0.0));
        // particle.SetSize(1.5f);
        // particle.SetTrailLength(500);
        
        m_particles.push_back(particle);
        std::cout << "Spawned particle at camera position\n";
    }
}

void SimulationManager::SpawnLightRay() {
    // Spawn light ray at camera position in camera direction
    if (m_renderingEngine) {
        const auto& camera = m_renderingEngine->GetCamera();
        std::array<double, 3> cameraPos = {camera.position[0], camera.position[1], camera.position[2]};
        // Calculate direction from position to target
        std::array<double, 3> cameraDir = {
            camera.target[0] - camera.position[0],
            camera.target[1] - camera.position[1],
            camera.target[2] - camera.position[2]
        };
        // Normalize direction
        double length = std::sqrt(cameraDir[0]*cameraDir[0] + cameraDir[1]*cameraDir[1] + cameraDir[2]*cameraDir[2]);
        if (length > 0.0) {
            cameraDir[0] /= length;
            cameraDir[1] /= length;
            cameraDir[2] /= length;
        }
        
        LightRay lightRay(cameraPos, cameraDir, 5e14, 1.0);
        // Note: SetType, SetFrequency, SetIntensity, SetTrailLength methods may not exist
        // lightRay.SetType(LightRay::Type::DIRECT);
        // lightRay.SetFrequency(5e14);
        // lightRay.SetIntensity(1.0);
        // lightRay.SetTrailLength(1000);
        
        m_lightRays.push_back(lightRay);
        std::cout << "Spawned light ray from camera\n";
    }
}

void SimulationManager::TakeScreenshot() {
    if (m_renderingEngine) {
        std::string filename = "screenshot_" + std::to_string(m_frameCount) + ".png";
        if (m_renderingEngine->TakeScreenshot(filename)) {
            std::cout << "Screenshot saved: " << filename << "\n";
        } else {
            std::cout << "Failed to save screenshot\n";
        }
    }
}

bool SimulationManager::LoadConfiguration(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Simple configuration loading (could be enhanced with JSON/XML)
    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;
        
        // Parse key=value pairs
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // Apply configuration
            if (key == "timeStep") {
                m_timeStep = std::stod(value);
            } else if (key == "targetFPS") {
                m_targetFPS = std::stod(value);
            } else if (key == "showPerformanceStats") {
                m_showPerformanceStats = (value == "true" || value == "1");
            }
        }
    }
    
    return true;
}

bool SimulationManager::SaveConfiguration(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Black Hole Simulation Configuration\n";
    file << "timeStep=" << m_timeStep << "\n";
    file << "targetFPS=" << m_targetFPS << "\n";
    file << "showPerformanceStats=" << (m_showPerformanceStats ? "true" : "false") << "\n";
    
    return true;
}

} // namespace BlackHoleSim