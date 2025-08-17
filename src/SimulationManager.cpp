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
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>

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
    , m_lastFrameTime(0.0)
    , m_deltaTime(0.0)
    , m_fps(0.0)
    , m_targetFPS(60.0)
    , m_maxTimeStep(0.1)
    , m_minTimeStep(1e-6)
    , m_performanceMode(PerformanceMode::BALANCED)
    , m_showPerformanceStats(false)
{
    // Initialize timing
    auto now = std::chrono::high_resolution_clock::now();
    m_lastFrameTime = std::chrono::duration<double>(now.time_since_epoch()).count();
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
        if (!SetupDefaultScene()) {
            std::cerr << "Error: Failed to setup default scene\n";
            return false;
        }
        
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
    m_physicsEngine = std::make_unique<PhysicsEngine>();
    if (!m_physicsEngine->Initialize()) {
        std::cerr << "    Failed to initialize Physics Engine\n";
        return false;
    }
    
    // Initialize Rendering Engine
    std::cout << "  Initializing Rendering Engine...\n";
    m_renderingEngine = std::make_unique<RenderingEngine>();
    if (!m_renderingEngine->Initialize(m_config.windowWidth, m_config.windowHeight, 
                                       "Black Hole Simulation", m_config.fullscreen)) {
        std::cerr << "    Failed to initialize Rendering Engine\n";
        return false;
    }
    
    // Configure rendering settings
    m_renderingEngine->SetVSync(m_config.vsync);
    m_renderingEngine->SetMSAA(m_config.msaaSamples);
    
    // Initialize Input System
    std::cout << "  Initializing Input System...\n";
    m_inputSystem = std::make_unique<InputSystem>();
    if (!m_inputSystem->Initialize(m_renderingEngine->GetWindow())) {
        std::cerr << "    Failed to initialize Input System\n";
        return false;
    }
    
    return true;
}

bool SimulationManager::SetupDefaultScene() {
    std::cout << "  Setting up default scene...\n";
    
    // Create black hole (1 solar mass at origin)
    const double solarMass = 1.989e30; // kg
    m_blackHole = std::make_unique<BlackHole>(solarMass, Vector3(0.0, 0.0, 0.0));
    
    // Create accretion disk
    m_accretionDisk = std::make_unique<AccretionDisk>(*m_blackHole);
    m_accretionDisk->SetInnerRadius(3.0 * m_blackHole->GetSchwarzschildRadius());
    m_accretionDisk->SetOuterRadius(20.0 * m_blackHole->GetSchwarzschildRadius());
    m_accretionDisk->SetMass(0.1 * solarMass);
    m_accretionDisk->SetModel(AccretionDisk::Model::SHAKURA_SUNYAEV);
    
    // Add some test particles
    AddTestParticles();
    
    // Add some light rays for lensing demonstration
    AddTestLightRays();
    
    // Set initial camera position
    Vector3 cameraPos(0.0, 0.0, 50.0 * m_blackHole->GetSchwarzschildRadius());
    m_renderingEngine->SetCameraPosition(cameraPos);
    m_renderingEngine->SetCameraTarget(Vector3(0.0, 0.0, 0.0));
    
    return true;
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
        
        auto particle = std::make_unique<Particle>(testMass, position, velocity);
        particle->SetType(Particle::Type::TEST_PARTICLE);
        particle->SetColor(Vector3(1.0, 0.8, 0.2)); // Golden color
        particle->SetSize(2.0f);
        particle->SetTrailLength(1000);
        
        m_particles.push_back(std::move(particle));
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
        
        auto lightRay = std::make_unique<LightRay>(position, direction);
        lightRay->SetType(LightRay::Type::LENSED);
        lightRay->SetFrequency(5e14); // Green light
        lightRay->SetIntensity(1.0);
        lightRay->SetTrailLength(2000);
        
        m_lightRays.push_back(std::move(lightRay));
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
            m_renderingEngine->UpdateCameraRotation(static_cast<float>(deltaX), static_cast<float>(deltaY));
        }
    });
    
    // Set scroll callback for zoom
    m_inputSystem->SetScrollCallback([this](double xOffset, double yOffset) {
        m_renderingEngine->UpdateCameraZoom(static_cast<float>(yOffset));
    });
    
    // Setup default key bindings
    m_inputSystem->ResetToDefaults();
}

void SimulationManager::HandleInputAction(InputSystem::Action action, float value) {
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
            m_renderingEngine->UpdateCameraPosition(RenderingEngine::CameraMovement::FORWARD, value);
            break;
            
        case InputSystem::Action::CAMERA_BACKWARD:
            m_renderingEngine->UpdateCameraPosition(RenderingEngine::CameraMovement::BACKWARD, value);
            break;
            
        case InputSystem::Action::CAMERA_LEFT:
            m_renderingEngine->UpdateCameraPosition(RenderingEngine::CameraMovement::LEFT, value);
            break;
            
        case InputSystem::Action::CAMERA_RIGHT:
            m_renderingEngine->UpdateCameraPosition(RenderingEngine::CameraMovement::RIGHT, value);
            break;
            
        case InputSystem::Action::CAMERA_UP:
            m_renderingEngine->UpdateCameraPosition(RenderingEngine::CameraMovement::UP, value);
            break;
            
        case InputSystem::Action::CAMERA_DOWN:
            m_renderingEngine->UpdateCameraPosition(RenderingEngine::CameraMovement::DOWN, value);
            break;
            
        case InputSystem::Action::CAMERA_RESET:
            ResetCamera();
            break;
            
        case InputSystem::Action::TOGGLE_CAMERA_MODE:
            m_renderingEngine->ToggleCameraMode();
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
            m_renderingEngine->ToggleRenderLayer(RenderingEngine::RenderLayer::UI);
            break;
            
        case InputSystem::Action::TOGGLE_FULLSCREEN:
            m_renderingEngine->ToggleFullscreen();
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

void SimulationManager::Run() {
    if (!m_isInitialized) {
        std::cerr << "Error: SimulationManager not initialized\n";
        return;
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
}

void SimulationManager::UpdateTiming() {
    auto now = std::chrono::high_resolution_clock::now();
    double currentTime = std::chrono::duration<double>(now.time_since_epoch()).count();
    
    m_deltaTime = currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;
    
    // Clamp delta time to prevent large jumps
    m_deltaTime = std::min(m_deltaTime, 1.0 / 30.0); // Max 30 FPS minimum
    
    m_frameCount++;
}

void SimulationManager::UpdatePhysics() {
    if (!m_physicsEngine) return;
    
    // Update particles
    for (auto& particle : m_particles) {
        if (particle && particle->IsActive()) {
            m_physicsEngine->UpdateParticle(*particle, *m_blackHole, m_timeStep);
        }
    }
    
    // Update light rays
    for (auto& lightRay : m_lightRays) {
        if (lightRay && lightRay->IsActive()) {
            m_physicsEngine->UpdateLightRay(*lightRay, *m_blackHole, m_timeStep);
        }
    }
    
    // Update accretion disk
    if (m_accretionDisk) {
        m_accretionDisk->Update(m_timeStep);
    }
    
    // Advance simulation time
    m_simulationTime += m_timeStep;
}

void SimulationManager::Render() {
    if (!m_renderingEngine) return;
    
    // Begin frame
    m_renderingEngine->BeginFrame();
    
    // Render black hole
    if (m_blackHole) {
        m_renderingEngine->RenderBlackHole(*m_blackHole);
    }
    
    // Render accretion disk
    if (m_accretionDisk) {
        m_renderingEngine->RenderAccretionDisk(*m_accretionDisk);
    }
    
    // Render particles
    for (const auto& particle : m_particles) {
        if (particle && particle->IsActive()) {
            m_renderingEngine->RenderParticle(*particle);
        }
    }
    
    // Render light rays
    for (const auto& lightRay : m_lightRays) {
        if (lightRay && lightRay->IsActive()) {
            m_renderingEngine->RenderLightRay(*lightRay);
        }
    }
    
    // Render UI
    RenderUI();
    
    // End frame
    m_renderingEngine->EndFrame();
}

void SimulationManager::RenderUI() {
    // Render performance stats if enabled
    if (m_showPerformanceStats) {
        m_renderingEngine->RenderText("FPS: " + std::to_string(static_cast<int>(m_fps)), 10, 10);
        m_renderingEngine->RenderText("Time Step: " + std::to_string(m_timeStep), 10, 30);
        m_renderingEngine->RenderText("Sim Time: " + std::to_string(m_simulationTime), 10, 50);
        m_renderingEngine->RenderText("Particles: " + std::to_string(m_particles.size()), 10, 70);
        m_renderingEngine->RenderText("Light Rays: " + std::to_string(m_lightRays.size()), 10, 90);
    }
    
    // Render pause indicator
    if (m_isPaused) {
        m_renderingEngine->RenderText("PAUSED", m_config.windowWidth / 2 - 30, 50);
    }
}

void SimulationManager::UpdatePerformanceStats() {
    static double lastStatsUpdate = 0.0;
    static int framesSinceLastUpdate = 0;
    
    framesSinceLastUpdate++;
    
    if (m_lastFrameTime - lastStatsUpdate >= 1.0) { // Update every second
        m_fps = framesSinceLastUpdate / (m_lastFrameTime - lastStatsUpdate);
        lastStatsUpdate = m_lastFrameTime;
        framesSinceLastUpdate = 0;
    }
}

void SimulationManager::LimitFrameRate() {
    if (m_targetFPS > 0.0) {
        double targetFrameTime = 1.0 / m_targetFPS;
        double frameTime = m_deltaTime;
        
        if (frameTime < targetFrameTime) {
            double sleepTime = targetFrameTime - frameTime;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
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
        m_physicsEngine->Shutdown();
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

void SimulationManager::SetTimeStep(double timeStep) {
    m_timeStep = std::clamp(timeStep, m_minTimeStep, m_maxTimeStep);
    std::cout << "Time step set to: " << m_timeStep << "\n";
}

void SimulationManager::ResetCamera() {
    if (m_renderingEngine && m_blackHole) {
        Vector3 cameraPos(0.0, 0.0, 50.0 * m_blackHole->GetSchwarzschildRadius());
        m_renderingEngine->SetCameraPosition(cameraPos);
        m_renderingEngine->SetCameraTarget(Vector3(0.0, 0.0, 0.0));
        std::cout << "Camera reset\n";
    }
}

void SimulationManager::SpawnParticle() {
    // Spawn particle at camera position with random velocity
    if (m_renderingEngine && m_blackHole) {
        Vector3 cameraPos = m_renderingEngine->GetCameraPosition();
        Vector3 randomVel(0.1 * (rand() / double(RAND_MAX) - 0.5),
                         0.1 * (rand() / double(RAND_MAX) - 0.5),
                         0.1 * (rand() / double(RAND_MAX) - 0.5));
        
        auto particle = std::make_unique<Particle>(1e20, cameraPos, randomVel);
        particle->SetType(Particle::Type::TEST_PARTICLE);
        particle->SetColor(Vector3(1.0, 0.5, 0.0));
        particle->SetSize(1.5f);
        particle->SetTrailLength(500);
        
        m_particles.push_back(std::move(particle));
        std::cout << "Spawned particle at camera position\n";
    }
}

void SimulationManager::SpawnLightRay() {
    // Spawn light ray at camera position in camera direction
    if (m_renderingEngine) {
        Vector3 cameraPos = m_renderingEngine->GetCameraPosition();
        Vector3 cameraDir = m_renderingEngine->GetCameraDirection();
        
        auto lightRay = std::make_unique<LightRay>(cameraPos, cameraDir);
        lightRay->SetType(LightRay::Type::DIRECT);
        lightRay->SetFrequency(5e14);
        lightRay->SetIntensity(1.0);
        lightRay->SetTrailLength(1000);
        
        m_lightRays.push_back(std::move(lightRay));
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