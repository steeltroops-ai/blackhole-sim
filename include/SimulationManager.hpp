#pragma once

#include "PhysicsEngine.hpp"
#include "RenderingEngine.hpp"
#include "InputSystem.hpp"
#include "BlackHole.hpp"
#include "AccretionDisk.hpp"
#include <vector>
#include <memory>
#include <chrono>

namespace BlackHoleSim {

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
    void SetPaused(bool paused) { m_paused = paused; }

    /**
     * @brief Check if simulation is paused
     * @return True if paused, false if running
     */
    bool IsPaused() const { return m_paused; }

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
    double GetFPS() const { return m_fps; }

private:
    // Core subsystems
    std::unique_ptr<PhysicsEngine> m_physicsEngine;
    std::unique_ptr<RenderingEngine> m_renderingEngine;
    std::unique_ptr<InputSystem> m_inputSystem;

    // Simulation objects
    std::unique_ptr<BlackHole> m_blackHole;
    std::unique_ptr<AccretionDisk> m_accretionDisk;
    std::vector<std::unique_ptr<Particle>> m_particles;
    std::vector<std::unique_ptr<LightRay>> m_lightRays;

    // Simulation state
    bool m_running;
    bool m_paused;
    double m_timeStep;
    double m_simulationTime;
    
    // Performance tracking
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    double m_fps;
    double m_frameTimeAccumulator;
    int m_frameCount;

    /**
     * @brief Update simulation by one time step
     * @param deltaTime Time elapsed since last update
     */
    void Update(double deltaTime);

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
};

} // namespace BlackHoleSim