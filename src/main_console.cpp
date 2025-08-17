/**
 * @file main_console.cpp
 * @brief Console-only version of the Black Hole Simulation
 * 
 * This version runs without graphics to test the physics engine.
 */

#include "BlackHole.hpp"
#include "Particle.hpp"
#include "PhysicsEngine.hpp"
#include "LightRay.hpp"
#include "Vector3.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

using namespace BlackHoleSim;

int main() {
    std::cout << "\n========================================\n";
    std::cout << "    Black Hole Simulation (Console)\n";
    std::cout << "========================================\n\n";
    
    try {
        // Create physics engine
        PhysicsEngine physics;
        
        // Create a black hole at origin with mass of 1e30 kg
        std::array<double, 3> position = {0.0, 0.0, 0.0};
        BlackHole blackHole(1e30, position);
        
        // Create a test particle
          Vector3 initialPos(100.0, 0.0, 0.0);  // 100 units from black hole
          Vector3 initialVel(0.0, 50.0, 0.0);   // Tangential velocity
          std::unique_ptr<Particle> particle(new Particle(1e20, initialPos, initialVel));
         
         // Create vectors for physics engine
         std::vector<std::unique_ptr<Particle>> particles;
         std::vector<std::unique_ptr<LightRay>> lightRays;
         particles.push_back(std::move(particle));
        
        std::cout << "Initial particle position: (" << initialPos.x() << ", " 
                  << initialPos.y() << ", " << initialPos.z() << ")\n";
        std::cout << "Initial particle velocity: (" << initialVel.x() << ", " 
                  << initialVel.y() << ", " << initialVel.z() << ")\n\n";
        
        // Simulation parameters
        const double timeStep = 0.01;
        const int maxSteps = 1000;
        const int printInterval = 100;
        
        std::cout << "Running simulation for " << maxSteps << " steps...\n\n";
        
        // Run simulation loop
        for (int step = 0; step < maxSteps; ++step) {
            // Update particle physics
            physics.Update(timeStep, particles, lightRays);
            
            // Print status every printInterval steps
            if (step % printInterval == 0) {
                Vector3 pos = particles[0]->GetPosition();
                Vector3 vel = particles[0]->GetVelocity();
                double distance = pos.Magnitude();
                double speed = vel.Magnitude();
                
                std::cout << "Step " << step << ":\n";
                std::cout << "  Position: (" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")\n";
                std::cout << "  Distance from black hole: " << distance << "\n";
                std::cout << "  Speed: " << speed << "\n\n";
                
                // Check if particle fell into black hole
                if (distance < blackHole.GetSchwarzschildRadius()) {
                    std::cout << "Particle crossed event horizon!\n";
                    break;
                }
            }
            
            // Small delay to make output readable (removed for C++11 compatibility)
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        std::cout << "Simulation completed successfully!\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}