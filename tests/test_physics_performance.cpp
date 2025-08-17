#include <gtest/gtest.h>
#include <chrono>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include "BlackHole.hpp"
#include "Particle.hpp"
#include "LightRay.hpp"
#include "AccretionDisk.hpp"
#include "PhysicsEngine.hpp"

// Performance test utilities
class PerformanceTimer {
public:
    void Start() {
        startTime = std::chrono::high_resolution_clock::now();
    }
    
    double Stop() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        return duration.count() / 1000.0; // Return milliseconds
    }
    
private:
    std::chrono::high_resolution_clock::time_point startTime;
};

class PhysicsPerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create standard test objects
        blackHole = std::make_unique<BlackHole>(10.0 * 1.98847e30, 0.0, 0.0, 0.0);
        engine = std::make_unique<PhysicsEngine>();
        engine->Initialize();
        
        // Setup random number generator
        rng.seed(12345); // Fixed seed for reproducible results
    }
    
    void TearDown() override {
        engine->Shutdown();
    }
    
    // Helper function to create random particles
    std::vector<Particle> CreateRandomParticles(int count) {
        std::vector<Particle> particles;
        std::uniform_real_distribution<double> massDist(100.0, 10000.0);
        std::uniform_real_distribution<double> radiusDist(5.0, 100.0);
        std::uniform_real_distribution<double> angleDist(0.0, 2.0 * M_PI);
        std::uniform_real_distribution<double> velocityDist(1000.0, 50000.0);
        
        double rs = blackHole->GetSchwarzschildRadius();
        
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.SetMass(massDist(rng));
            
            double r = radiusDist(rng) * rs;
            double theta = M_PI / 2.0; // Equatorial plane
            double phi = angleDist(rng);
            
            double x = r * cos(phi);
            double y = r * sin(phi);
            double z = 0.0;
            
            p.SetPosition(x, y, z);
            
            // Set tangential velocity for roughly circular orbits
            double vCirc = blackHole->GetCircularOrbitVelocity(r);
            double vMag = vCirc * (0.8 + 0.4 * std::uniform_real_distribution<double>(0.0, 1.0)(rng));
            
            double vx = -vMag * sin(phi);
            double vy = vMag * cos(phi);
            double vz = 0.0;
            
            p.SetVelocity(vx, vy, vz);
            particles.push_back(p);
        }
        
        return particles;
    }
    
    // Helper function to create random light rays
    std::vector<LightRay> CreateRandomLightRays(int count) {
        std::vector<LightRay> lightRays;
        std::uniform_real_distribution<double> radiusDist(10.0, 1000.0);
        std::uniform_real_distribution<double> angleDist(0.0, 2.0 * M_PI);
        std::uniform_real_distribution<double> impactDist(0.1, 10.0);
        std::uniform_real_distribution<double> wavelengthDist(400e-9, 700e-9);
        
        double rs = blackHole->GetSchwarzschildRadius();
        
        for (int i = 0; i < count; ++i) {
            LightRay lr;
            
            double r = radiusDist(rng) * rs;
            double phi = angleDist(rng);
            
            double x = r * cos(phi);
            double y = r * sin(phi);
            double z = 0.0;
            
            lr.SetPosition(x, y, z);
            
            // Set direction roughly toward black hole with some impact parameter
            double impactParam = impactDist(rng) * rs;
            double dx = -cos(phi) + impactParam * sin(phi) / r;
            double dy = -sin(phi) - impactParam * cos(phi) / r;
            double dz = 0.0;
            
            // Normalize direction
            double norm = sqrt(dx*dx + dy*dy + dz*dz);
            lr.SetDirection(dx/norm, dy/norm, dz/norm);
            
            lr.SetWavelength(wavelengthDist(rng));
            lightRays.push_back(lr);
        }
        
        return lightRays;
    }
    
    std::unique_ptr<BlackHole> blackHole;
    std::unique_ptr<PhysicsEngine> engine;
    std::mt19937 rng;
};

// Test particle update performance
TEST_F(PhysicsPerformanceTest, ParticleUpdatePerformance) {
    const int numParticles = 1000;
    const int numSteps = 1000;
    const double dt = 0.01;
    
    auto particles = CreateRandomParticles(numParticles);
    
    PerformanceTimer timer;
    timer.Start();
    
    for (int step = 0; step < numSteps; ++step) {
        for (auto& particle : particles) {
            if (particle.GetRadialDistance() > blackHole->GetSchwarzschildRadius()) {
                engine->UpdateParticle(particle, *blackHole, dt);
            }
        }
    }
    
    double elapsed = timer.Stop();
    double updatesPerSecond = (numParticles * numSteps) / (elapsed / 1000.0);
    
    std::cout << "\nParticle Update Performance:" << std::endl;
    std::cout << "  Particles: " << numParticles << std::endl;
    std::cout << "  Steps: " << numSteps << std::endl;
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms" << std::endl;
    std::cout << "  Updates/second: " << std::fixed << std::setprecision(0) << updatesPerSecond << std::endl;
    std::cout << "  Time per update: " << std::fixed << std::setprecision(3) 
              << (elapsed * 1000.0) / (numParticles * numSteps) << " μs" << std::endl;
    
    // Performance expectation: should handle at least 10,000 updates per second
    EXPECT_GT(updatesPerSecond, 10000.0) << "Particle updates too slow";
}

// Test light ray update performance
TEST_F(PhysicsPerformanceTest, LightRayUpdatePerformance) {
    const int numLightRays = 500;
    const int numSteps = 2000;
    const double dt = 1e-6;
    
    auto lightRays = CreateRandomLightRays(numLightRays);
    
    PerformanceTimer timer;
    timer.Start();
    
    for (int step = 0; step < numSteps; ++step) {
        for (auto& lightRay : lightRays) {
            if (lightRay.GetStatus() == LightRay::Status::ACTIVE) {
                engine->UpdateLightRay(lightRay, *blackHole, dt);
            }
        }
    }
    
    double elapsed = timer.Stop();
    double updatesPerSecond = (numLightRays * numSteps) / (elapsed / 1000.0);
    
    std::cout << "\nLight Ray Update Performance:" << std::endl;
    std::cout << "  Light rays: " << numLightRays << std::endl;
    std::cout << "  Steps: " << numSteps << std::endl;
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms" << std::endl;
    std::cout << "  Updates/second: " << std::fixed << std::setprecision(0) << updatesPerSecond << std::endl;
    std::cout << "  Time per update: " << std::fixed << std::setprecision(3) 
              << (elapsed * 1000.0) / (numLightRays * numSteps) << " μs" << std::endl;
    
    // Performance expectation: should handle at least 50,000 updates per second
    EXPECT_GT(updatesPerSecond, 50000.0) << "Light ray updates too slow";
}

// Test metric calculations performance
TEST_F(PhysicsPerformanceTest, MetricCalculationPerformance) {
    const int numCalculations = 1000000;
    
    std::uniform_real_distribution<double> radiusDist(2.1, 1000.0);
    std::uniform_real_distribution<double> thetaDist(0.1, M_PI - 0.1);
    
    double rs = blackHole->GetSchwarzschildRadius();
    
    PerformanceTimer timer;
    timer.Start();
    
    double sum = 0.0; // Prevent optimization
    for (int i = 0; i < numCalculations; ++i) {
        double r = radiusDist(rng) * rs;
        double theta = thetaDist(rng);
        
        sum += blackHole->GetMetricComponent_tt(r);
        sum += blackHole->GetMetricComponent_rr(r);
        sum += blackHole->GetMetricComponent_thetatheta(r);
        sum += blackHole->GetMetricComponent_phiphi(r, theta);
    }
    
    double elapsed = timer.Stop();
    double calculationsPerSecond = (numCalculations * 4) / (elapsed / 1000.0);
    
    std::cout << "\nMetric Calculation Performance:" << std::endl;
    std::cout << "  Calculations: " << numCalculations * 4 << std::endl;
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms" << std::endl;
    std::cout << "  Calculations/second: " << std::fixed << std::setprecision(0) << calculationsPerSecond << std::endl;
    std::cout << "  Time per calculation: " << std::fixed << std::setprecision(3) 
              << (elapsed * 1000.0) / (numCalculations * 4) << " μs" << std::endl;
    std::cout << "  Sum (anti-optimization): " << sum << std::endl;
    
    // Performance expectation: should handle at least 1,000,000 calculations per second
    EXPECT_GT(calculationsPerSecond, 1000000.0) << "Metric calculations too slow";
}

// Test coordinate transformation performance
TEST_F(PhysicsPerformanceTest, CoordinateTransformationPerformance) {
    const int numTransformations = 1000000;
    
    std::uniform_real_distribution<double> coordDist(-1000.0, 1000.0);
    
    PerformanceTimer timer;
    timer.Start();
    
    double sum = 0.0; // Prevent optimization
    for (int i = 0; i < numTransformations; ++i) {
        double x = coordDist(rng);
        double y = coordDist(rng);
        double z = coordDist(rng);
        
        double r, theta, phi;
        engine->CartesianToSpherical(x, y, z, r, theta, phi);
        
        double x2, y2, z2;
        engine->SphericalToCartesian(r, theta, phi, x2, y2, z2);
        
        sum += x2 + y2 + z2;
    }
    
    double elapsed = timer.Stop();
    double transformationsPerSecond = (numTransformations * 2) / (elapsed / 1000.0);
    
    std::cout << "\nCoordinate Transformation Performance:" << std::endl;
    std::cout << "  Transformations: " << numTransformations * 2 << std::endl;
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms" << std::endl;
    std::cout << "  Transformations/second: " << std::fixed << std::setprecision(0) << transformationsPerSecond << std::endl;
    std::cout << "  Time per transformation: " << std::fixed << std::setprecision(3) 
              << (elapsed * 1000.0) / (numTransformations * 2) << " μs" << std::endl;
    std::cout << "  Sum (anti-optimization): " << sum << std::endl;
    
    // Performance expectation: should handle at least 500,000 transformations per second
    EXPECT_GT(transformationsPerSecond, 500000.0) << "Coordinate transformations too slow";
}

// Test accretion disk calculations performance
TEST_F(PhysicsPerformanceTest, AccretionDiskPerformance) {
    const int numCalculations = 100000;
    
    double innerRadius = blackHole->GetISCORadius();
    double outerRadius = 100.0 * innerRadius;
    double accretionRate = 1e-8 * 1.98847e30;
    
    AccretionDisk disk(*blackHole, innerRadius, outerRadius, accretionRate);
    
    std::uniform_real_distribution<double> radiusDist(innerRadius, outerRadius);
    
    PerformanceTimer timer;
    timer.Start();
    
    double sum = 0.0; // Prevent optimization
    for (int i = 0; i < numCalculations; ++i) {
        double r = radiusDist(rng);
        
        sum += disk.GetTemperature(r);
        sum += disk.GetDensity(r);
        sum += disk.GetSurfaceDensity(r);
        sum += disk.GetLuminosity(r);
    }
    
    double elapsed = timer.Stop();
    double calculationsPerSecond = (numCalculations * 4) / (elapsed / 1000.0);
    
    std::cout << "\nAccretion Disk Calculation Performance:" << std::endl;
    std::cout << "  Calculations: " << numCalculations * 4 << std::endl;
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms" << std::endl;
    std::cout << "  Calculations/second: " << std::fixed << std::setprecision(0) << calculationsPerSecond << std::endl;
    std::cout << "  Time per calculation: " << std::fixed << std::setprecision(3) 
              << (elapsed * 1000.0) / (numCalculations * 4) << " μs" << std::endl;
    std::cout << "  Sum (anti-optimization): " << sum << std::endl;
    
    // Performance expectation: should handle at least 100,000 calculations per second
    EXPECT_GT(calculationsPerSecond, 100000.0) << "Accretion disk calculations too slow";
}

// Test integration method comparison
TEST_F(PhysicsPerformanceTest, IntegrationMethodComparison) {
    const int numParticles = 100;
    const int numSteps = 1000;
    const double dt = 0.01;
    
    auto particles1 = CreateRandomParticles(numParticles);
    auto particles2 = particles1; // Copy for second test
    auto particles3 = particles1; // Copy for third test
    
    // Test Euler method
    engine->SetIntegrationMethod(PhysicsEngine::IntegrationMethod::EULER);
    PerformanceTimer timer1;
    timer1.Start();
    
    for (int step = 0; step < numSteps; ++step) {
        for (auto& particle : particles1) {
            if (particle.GetRadialDistance() > blackHole->GetSchwarzschildRadius()) {
                engine->UpdateParticle(particle, *blackHole, dt);
            }
        }
    }
    
    double elapsed1 = timer1.Stop();
    
    // Test RK4 method
    engine->SetIntegrationMethod(PhysicsEngine::IntegrationMethod::RK4);
    PerformanceTimer timer2;
    timer2.Start();
    
    for (int step = 0; step < numSteps; ++step) {
        for (auto& particle : particles2) {
            if (particle.GetRadialDistance() > blackHole->GetSchwarzschildRadius()) {
                engine->UpdateParticle(particle, *blackHole, dt);
            }
        }
    }
    
    double elapsed2 = timer2.Stop();
    
    // Test Adaptive RK45 method
    engine->SetIntegrationMethod(PhysicsEngine::IntegrationMethod::ADAPTIVE_RK45);
    PerformanceTimer timer3;
    timer3.Start();
    
    for (int step = 0; step < numSteps; ++step) {
        for (auto& particle : particles3) {
            if (particle.GetRadialDistance() > blackHole->GetSchwarzschildRadius()) {
                engine->UpdateParticle(particle, *blackHole, dt);
            }
        }
    }
    
    double elapsed3 = timer3.Stop();
    
    std::cout << "\nIntegration Method Performance Comparison:" << std::endl;
    std::cout << "  Euler method: " << std::fixed << std::setprecision(2) << elapsed1 << " ms" << std::endl;
    std::cout << "  RK4 method: " << std::fixed << std::setprecision(2) << elapsed2 << " ms" << std::endl;
    std::cout << "  Adaptive RK45: " << std::fixed << std::setprecision(2) << elapsed3 << " ms" << std::endl;
    std::cout << "  RK4/Euler ratio: " << std::fixed << std::setprecision(2) << elapsed2/elapsed1 << std::endl;
    std::cout << "  RK45/Euler ratio: " << std::fixed << std::setprecision(2) << elapsed3/elapsed1 << std::endl;
    
    // RK4 should be slower than Euler but not more than 10x
    EXPECT_LT(elapsed2/elapsed1, 10.0) << "RK4 method too slow compared to Euler";
    
    // Adaptive method might be faster or slower depending on step size adaptation
    EXPECT_LT(elapsed3/elapsed1, 20.0) << "Adaptive RK45 method too slow compared to Euler";
}

// Memory usage test
TEST_F(PhysicsPerformanceTest, MemoryUsageTest) {
    const int numParticles = 10000;
    const int numLightRays = 5000;
    
    std::cout << "\nMemory Usage Test:" << std::endl;
    std::cout << "  Creating " << numParticles << " particles and " << numLightRays << " light rays..." << std::endl;
    
    auto particles = CreateRandomParticles(numParticles);
    auto lightRays = CreateRandomLightRays(numLightRays);
    
    // Calculate approximate memory usage
    size_t particleSize = sizeof(Particle);
    size_t lightRaySize = sizeof(LightRay);
    size_t totalMemory = numParticles * particleSize + numLightRays * lightRaySize;
    
    std::cout << "  Particle size: " << particleSize << " bytes" << std::endl;
    std::cout << "  Light ray size: " << lightRaySize << " bytes" << std::endl;
    std::cout << "  Total memory (approx): " << totalMemory / 1024.0 << " KB" << std::endl;
    
    // Test that we can actually use all the objects
    PerformanceTimer timer;
    timer.Start();
    
    double sum = 0.0;
    for (const auto& particle : particles) {
        sum += particle.GetKineticEnergy();
    }
    
    for (const auto& lightRay : lightRays) {
        sum += lightRay.GetPhotonEnergy();
    }
    
    double elapsed = timer.Stop();
    
    std::cout << "  Access time: " << std::fixed << std::setprecision(2) << elapsed << " ms" << std::endl;
    std::cout << "  Sum (anti-optimization): " << sum << std::endl;
    
    // Should be able to access all objects quickly
    EXPECT_LT(elapsed, 100.0) << "Memory access too slow";
}

// Stress test with many objects
TEST_F(PhysicsPerformanceTest, StressTest) {
    const int numParticles = 1000;
    const int numLightRays = 500;
    const int numSteps = 100;
    const double dt = 0.01;
    
    auto particles = CreateRandomParticles(numParticles);
    auto lightRays = CreateRandomLightRays(numLightRays);
    
    // Create accretion disk
    double innerRadius = blackHole->GetISCORadius();
    double outerRadius = 100.0 * innerRadius;
    double accretionRate = 1e-8 * 1.98847e30;
    AccretionDisk disk(*blackHole, innerRadius, outerRadius, accretionRate);
    
    std::cout << "\nStress Test:" << std::endl;
    std::cout << "  Particles: " << numParticles << std::endl;
    std::cout << "  Light rays: " << numLightRays << std::endl;
    std::cout << "  Steps: " << numSteps << std::endl;
    
    PerformanceTimer timer;
    timer.Start();
    
    for (int step = 0; step < numSteps; ++step) {
        // Update particles
        for (auto& particle : particles) {
            if (particle.GetRadialDistance() > blackHole->GetSchwarzschildRadius()) {
                engine->UpdateParticle(particle, *blackHole, dt);
            }
        }
        
        // Update light rays
        for (auto& lightRay : lightRays) {
            if (lightRay.GetStatus() == LightRay::Status::ACTIVE) {
                engine->UpdateLightRay(lightRay, *blackHole, dt * 1e-3); // Smaller dt for light
            }
        }
        
        // Update disk
        disk.Update(dt);
    }
    
    double elapsed = timer.Stop();
    double totalUpdates = numSteps * (numParticles + numLightRays + 1); // +1 for disk
    double updatesPerSecond = totalUpdates / (elapsed / 1000.0);
    
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms" << std::endl;
    std::cout << "  Total updates: " << totalUpdates << std::endl;
    std::cout << "  Updates/second: " << std::fixed << std::setprecision(0) << updatesPerSecond << std::endl;
    std::cout << "  Average time per step: " << std::fixed << std::setprecision(2) << elapsed/numSteps << " ms" << std::endl;
    
    // Should handle the stress test in reasonable time
    EXPECT_LT(elapsed, 10000.0) << "Stress test took too long (>10 seconds)";
    EXPECT_GT(updatesPerSecond, 1000.0) << "Stress test updates too slow";
}

// Main function for running performance tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "\n=== Black Hole Simulation Performance Tests ===" << std::endl;
    std::cout << "These tests measure the performance of physics calculations." << std::endl;
    std::cout << "Results may vary depending on hardware and system load." << std::endl;
    
    int result = RUN_ALL_TESTS();
    
    std::cout << "\n=== Performance Test Summary ===" << std::endl;
    std::cout << "All performance tests completed." << std::endl;
    std::cout << "Check individual test results for performance metrics." << std::endl;
    
    return result;
}