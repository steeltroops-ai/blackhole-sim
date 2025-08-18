#pragma once

#include "PhysicsEngine.hpp"
#include "RenderingEngine.hpp"
#include "InputSystem.hpp"
#include "BlackHole.hpp"
#include "AccretionDisk.hpp"
#include "Vector3.hpp"
#include <vector>
#include <memory>
#include <chrono>
#include <string>

namespace BlackHoleSim {

/**
 * @brief Performance mode enumeration
 */
enum class PerformanceMode {
    HIGH_QUALITY,
    BALANCED,
    HIGH_PERFORMANCE
};

/**
 * @brief Configuration structure for simulation parameters
 */
struct SimulationConfig {
    // Window settings
    int windowWidth = 1280;
    int windowHeight = 720;
    bool fullscreen = false;
    bool vsync = true;
    int msaaSamples = 4;
    
    // Configuration file
    std::string configFile;
    
    // Default constructor
    SimulationConfig() = default;
};

/**
 * @brief Main simulation manager that coordinates all subsystems
 * 
 * Manages the simulation loop, coordinates between physics and rendering,
 * handles timing, and maintains the overall simulation state.
 */
class SimulationManager {
public:
    SimulationManager();
    ~SimulationManager();

    /**
     * @brief Initialize the simulation with default parameters
     * @return true if initialization successful, false otherwise
     */
    bool Initialize();
    
    /**
     * @brief Initialize the simulation with configuration
     * @param config Simulation configuration parameters
     * @return true if initialization successful, false otherwise
     */
    bool Initialize(const SimulationConfig& config);

    /**
     * @brief Run the main simulation loop
     * @return Exit code (0 for success)
     */
    int Run();

    /**
     * @brief Shutdown and cleanup all resources
     */
    void Shutdown();

    /**
     * @brief Pause/unpause the simulation
     * @param paused True to pause, false to resume
     */
    void SetPaused(bool paused) { m_isPaused = paused; }

    /**
     * @brief Check if simulation is paused
     * @return True if paused, false if running
     */
    bool IsPaused() const { return m_isPaused; }

    /**
     * @brief Reset simulation to initial state
     */
    void Reset();

    /**
     * @brief Set simulation time step
     * @param dt Time step in seconds
     */
    void SetTimeStep(double dt) { m_timeStep = dt; }

    /**
     * @brief Get current simulation time
     * @return Simulation time in seconds
     */
    double GetSimulationTime() const { return m_simulationTime; }

    /**
     * @brief Get frames per second
     * @return Current FPS
     */
    double GetFPS() const { return 1.0 / m_frameTime; }

    /**
     * @brief Spawn a new particle in the simulation
     */
    void SpawnParticle();

    /**
     * @brief Spawn a new light ray in the simulation
     */
    void SpawnLightRay();

    /**
     * @brief Take a screenshot of the current simulation
     */
    void TakeScreenshot();

    /**
     * @brief Load configuration from file
     * @param filename Path to configuration file
     * @return true if successful, false otherwise
     */
    bool LoadConfiguration(const std::string& filename);

    /**
     * @brief Save configuration to file
     * @param filename Path to configuration file
     * @return true if successful, false otherwise
     */
    bool SaveConfiguration(const std::string& filename) const;

    /**
     * @brief Reset camera to default position and orientation
     */
    void ResetCamera();
    void Stop();
    void SetupDefaultScene();
    void UpdatePerformanceStats();
    void LimitFrameRate();
    void TogglePause();
    void Step();
    void RenderUI();
    void UpdateTiming();
    void SetupInputCallbacks();
    void HandleInputAction(InputSystem::Action action, float value);
    void HandleCameraMovement(InputSystem::Action action, float value);
    void HandleCameraRotation(double deltaX, double deltaY);
    void HandleCameraZoom(double yOffset);
    void ToggleCameraMode();
    void AddTestParticles();
    void AddTestLightRays();

private:
    // Configuration
    SimulationConfig m_config;
    
    // Core subsystems
    std::unique_ptr<PhysicsEngine> m_physicsEngine;
    std::unique_ptr<RenderingEngine> m_renderingEngine;
    std::unique_ptr<InputSystem> m_inputSystem;
    
    // Simulation objects
    std::unique_ptr<BlackHole> m_blackHole;
    std::unique_ptr<AccretionDisk> m_accretionDisk;
    std::vector<Particle> m_particles;
    std::vector<LightRay> m_lightRays;
    
    // Simulation state
    bool m_isInitialized;
    bool m_isRunning;
    bool m_isPaused;
    double m_timeStep;
    double m_simulationTime;
    double m_maxTimeStep;
    double m_minTimeStep;
    PerformanceMode m_performanceMode;
    
    // Performance tracking
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    double m_frameTime;
    double m_deltaTime;
    double m_fps;
    int m_frameCount;
    double m_targetFPS;
    bool m_showPerformanceStats;
    
    // Camera state
    Vector3 cameraPos;
    Vector3 cameraDir;

    /**
     * @brief Update simulation by one time step
     * @param deltaTime Time elapsed since last update
     */
    void Update(double deltaTime);
    
    /**
     * @brief Update physics simulation
     */
    void UpdatePhysics();

    /**
     * @brief Render current simulation state
     */
    void Render();

    /**
     * @brief Handle user input
     */
    void HandleInput();

    /**
     * @brief Update performance metrics
     * @param deltaTime Time elapsed since last frame
     */
    void UpdatePerformanceMetrics(double deltaTime);

    /**
     * @brief Initialize default simulation objects
     */
    void InitializeSimulationObjects();
    
    /**
     * @brief Initialize core subsystems (physics, rendering, input)
     * @return true if successful, false otherwise
     */
    bool InitializeSubsystems();
};

} // namespace BlackHoleSim