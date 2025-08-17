#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "BlackHole.hpp"
#include "Particle.hpp"
#include "LightRay.hpp"
#include "AccretionDisk.hpp"
#include "PhysicsEngine.hpp"

// Test tolerance for floating point comparisons
const double TOLERANCE = 1e-10;
const double PHYSICS_TOLERANCE = 1e-6;

// Helper function for floating point comparison
bool IsNear(double a, double b, double tolerance = TOLERANCE) {
    return std::abs(a - b) < tolerance;
}

class BlackHolePhysicsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Standard test black hole (10 solar masses)
        blackHole = std::make_unique<BlackHole>(10.0 * 1.98847e30, 0.0, 0.0, 0.0);
    }
    
    std::unique_ptr<BlackHole> blackHole;
};

// Test BlackHole basic properties
TEST_F(BlackHolePhysicsTest, BasicProperties) {
    double mass = 10.0 * 1.98847e30; // 10 solar masses
    
    // Test Schwarzschild radius
    double expectedRs = 2.0 * 6.67430e-11 * mass / (2.99792458e8 * 2.99792458e8);
    EXPECT_NEAR(blackHole->GetSchwarzschildRadius(), expectedRs, expectedRs * 1e-10);
    
    // Test photon sphere radius (1.5 * Rs)
    EXPECT_NEAR(blackHole->GetPhotonSphereRadius(), 1.5 * expectedRs, expectedRs * 1e-10);
    
    // Test ISCO radius (3 * Rs)
    EXPECT_NEAR(blackHole->GetISCORadius(), 3.0 * expectedRs, expectedRs * 1e-10);
}

TEST_F(BlackHolePhysicsTest, MetricComponents) {
    double r = 100000.0; // Far from black hole
    double theta = M_PI / 4.0;
    
    // Test metric components
    double gtt = blackHole->GetMetricComponent_tt(r);
    double grr = blackHole->GetMetricComponent_rr(r);
    double gtheta = blackHole->GetMetricComponent_thetatheta(r);
    double gphi = blackHole->GetMetricComponent_phiphi(r, theta);
    
    // At large distances, should approach Minkowski metric
    EXPECT_NEAR(gtt, -1.0, 1e-6);
    EXPECT_NEAR(grr, 1.0, 1e-6);
    EXPECT_NEAR(gtheta, r * r, r * r * 1e-10);
    EXPECT_NEAR(gphi, r * r * sin(theta) * sin(theta), r * r * 1e-10);
}

TEST_F(BlackHolePhysicsTest, GravitationalRedshift) {
    double rs = blackHole->GetSchwarzschildRadius();
    
    // Test redshift at various distances
    double r1 = 10.0 * rs; // Observer at 10 Rs
    double r2 = 2.1 * rs;  // Source just outside horizon
    
    double redshift = blackHole->GetGravitationalRedshift(r1, r2);
    
    // Should be positive (redshifted)
    EXPECT_GT(redshift, 0.0);
    
    // Test limiting case: same radius should give zero redshift
    EXPECT_NEAR(blackHole->GetGravitationalRedshift(r1, r1), 0.0, TOLERANCE);
}

TEST_F(BlackHolePhysicsTest, EscapeVelocity) {
    double rs = blackHole->GetSchwarzschildRadius();
    
    // Test escape velocity at various distances
    double r = 10.0 * rs;
    double vEsc = blackHole->GetEscapeVelocity(r);
    
    // Should be less than speed of light
    EXPECT_LT(vEsc, 2.99792458e8);
    
    // Should approach c as r approaches Rs
    double rNear = 1.01 * rs;
    double vEscNear = blackHole->GetEscapeVelocity(rNear);
    EXPECT_GT(vEscNear, vEsc);
}

TEST_F(BlackHolePhysicsTest, CircularOrbitProperties) {
    double rs = blackHole->GetSchwarzschildRadius();
    double isco = blackHole->GetISCORadius();
    
    // Test circular orbit velocity at ISCO
    double vISCO = blackHole->GetCircularOrbitVelocity(isco);
    EXPECT_GT(vISCO, 0.0);
    EXPECT_LT(vISCO, 2.99792458e8);
    
    // Test orbital period
    double period = blackHole->GetOrbitalPeriod(isco);
    EXPECT_GT(period, 0.0);
    
    // Test orbit stability
    EXPECT_TRUE(blackHole->IsOrbitStable(2.0 * isco));
    EXPECT_FALSE(blackHole->IsOrbitStable(0.5 * isco));
}

class ParticlePhysicsTest : public ::testing::Test {
protected:
    void SetUp() override {
        blackHole = std::make_unique<BlackHole>(1.98847e30, 0.0, 0.0, 0.0); // 1 solar mass
        
        // Create test particle
        particle = std::make_unique<Particle>();
        particle->SetMass(1000.0); // 1000 kg
        particle->SetPosition(100000.0, 0.0, 0.0); // 100 km from center
        particle->SetVelocity(0.0, 7000.0, 0.0); // 7 km/s tangential
    }
    
    std::unique_ptr<BlackHole> blackHole;
    std::unique_ptr<Particle> particle;
};

TEST_F(ParticlePhysicsTest, BasicKinematics) {
    // Test position and velocity getters
    auto pos = particle->GetPosition();
    auto vel = particle->GetVelocity();
    
    EXPECT_NEAR(pos.x, 100000.0, TOLERANCE);
    EXPECT_NEAR(pos.y, 0.0, TOLERANCE);
    EXPECT_NEAR(pos.z, 0.0, TOLERANCE);
    
    EXPECT_NEAR(vel.x, 0.0, TOLERANCE);
    EXPECT_NEAR(vel.y, 7000.0, TOLERANCE);
    EXPECT_NEAR(vel.z, 0.0, TOLERANCE);
}

TEST_F(ParticlePhysicsTest, EnergyCalculations) {
    // Test kinetic energy
    double ke = particle->GetKineticEnergy();
    double expectedKE = 0.5 * particle->GetMass() * particle->GetSpeed() * particle->GetSpeed();
    EXPECT_NEAR(ke, expectedKE, expectedKE * 1e-10);
    
    // Test total energy in gravitational field
    double totalEnergy = particle->GetOrbitalEnergy(*blackHole);
    EXPECT_LT(totalEnergy, 0.0); // Should be negative for bound orbit
}

TEST_F(ParticlePhysicsTest, AngularMomentum) {
    double L = particle->GetAngularMomentum(*blackHole);
    
    // For circular motion: L = mvr
    double expectedL = particle->GetMass() * particle->GetSpeed() * particle->GetRadialDistance();
    EXPECT_NEAR(L, expectedL, expectedL * 1e-10);
}

TEST_F(ParticlePhysicsTest, RelativisticEffects) {
    // Set high velocity to test relativistic effects
    particle->SetVelocity(0.0, 0.9 * 2.99792458e8, 0.0); // 0.9c
    
    double gamma = particle->GetLorentzFactor();
    double expectedGamma = 1.0 / sqrt(1.0 - 0.9 * 0.9);
    EXPECT_NEAR(gamma, expectedGamma, expectedGamma * 1e-10);
    
    // Test relativistic mass
    double relMass = particle->GetRelativisticMass();
    double expectedRelMass = particle->GetMass() * gamma;
    EXPECT_NEAR(relMass, expectedRelMass, expectedRelMass * 1e-10);
}

class LightRayPhysicsTest : public ::testing::Test {
protected:
    void SetUp() override {
        blackHole = std::make_unique<BlackHole>(1.98847e30, 0.0, 0.0, 0.0);
        
        // Create test light ray
        lightRay = std::make_unique<LightRay>();
        lightRay->SetPosition(100000.0, 0.0, 0.0);
        lightRay->SetDirection(-1.0, 0.1, 0.0); // Slightly off-radial
        lightRay->SetWavelength(550e-9); // Green light
    }
    
    std::unique_ptr<BlackHole> blackHole;
    std::unique_ptr<LightRay> lightRay;
};

TEST_F(LightRayPhysicsTest, BasicProperties) {
    // Test wavelength and frequency
    double wavelength = lightRay->GetWavelength();
    double frequency = lightRay->GetFrequency();
    
    double expectedFreq = 2.99792458e8 / wavelength;
    EXPECT_NEAR(frequency, expectedFreq, expectedFreq * 1e-10);
    
    // Test photon energy
    double energy = lightRay->GetPhotonEnergy();
    double expectedEnergy = 6.62607015e-34 * frequency;
    EXPECT_NEAR(energy, expectedEnergy, expectedEnergy * 1e-10);
}

TEST_F(LightRayPhysicsTest, ConservedQuantities) {
    // Test impact parameter
    double b = lightRay->GetImpactParameter(*blackHole);
    EXPECT_GT(b, 0.0);
    
    // Test conserved energy and angular momentum
    double E = lightRay->GetConservedEnergy(*blackHole);
    double L = lightRay->GetConservedAngularMomentum(*blackHole);
    
    EXPECT_GT(E, 0.0);
    EXPECT_GT(L, 0.0);
    
    // Impact parameter should equal L/E
    EXPECT_NEAR(b, L/E, b * 1e-10);
}

TEST_F(LightRayPhysicsTest, RedshiftCalculations) {
    double rs = blackHole->GetSchwarzschildRadius();
    
    // Test gravitational redshift
    double r1 = 10.0 * rs;
    double r2 = 2.1 * rs;
    
    double redshift = lightRay->GetRedshift(r1, r2, 0.0, 0.0); // No Doppler
    EXPECT_GT(redshift, 0.0); // Should be redshifted
    
    // Test blueshift (opposite direction)
    double blueshift = lightRay->GetRedshift(r2, r1, 0.0, 0.0);
    EXPECT_LT(blueshift, 0.0); // Should be blueshifted
}

class AccretionDiskPhysicsTest : public ::testing::Test {
protected:
    void SetUp() override {
        blackHole = std::make_unique<BlackHole>(10.0 * 1.98847e30, 0.0, 0.0, 0.0);
        
        // Create test accretion disk
        double innerRadius = blackHole->GetISCORadius();
        double outerRadius = 100.0 * innerRadius;
        double accretionRate = 1e-8 * 1.98847e30; // 10^-8 solar masses per year
        
        disk = std::make_unique<AccretionDisk>(*blackHole, innerRadius, outerRadius, accretionRate);
    }
    
    std::unique_ptr<BlackHole> blackHole;
    std::unique_ptr<AccretionDisk> disk;
};

TEST_F(AccretionDiskPhysicsTest, TemperatureProfile) {
    double innerR = disk->GetInnerRadius();
    double outerR = disk->GetOuterRadius();
    
    // Test temperature at inner and outer edges
    double tempInner = disk->GetTemperature(innerR);
    double tempOuter = disk->GetTemperature(outerR);
    
    // Inner disk should be hotter
    EXPECT_GT(tempInner, tempOuter);
    EXPECT_GT(tempInner, 0.0);
    EXPECT_GT(tempOuter, 0.0);
    
    // Test temperature scaling (should roughly follow r^-3/4)
    double midR = sqrt(innerR * outerR);
    double tempMid = disk->GetTemperature(midR);
    
    // Rough check of power law scaling
    double ratio1 = tempInner / tempMid;
    double ratio2 = tempMid / tempOuter;
    double rRatio1 = midR / innerR;
    double rRatio2 = outerR / midR;
    
    // Should approximately follow T ∝ r^-3/4
    double expectedRatio1 = pow(rRatio1, 0.75);
    double expectedRatio2 = pow(rRatio2, 0.75);
    
    EXPECT_NEAR(ratio1, expectedRatio1, expectedRatio1 * 0.1); // 10% tolerance
    EXPECT_NEAR(ratio2, expectedRatio2, expectedRatio2 * 0.1);
}

TEST_F(AccretionDiskPhysicsTest, DensityProfile) {
    double innerR = disk->GetInnerRadius();
    double outerR = disk->GetOuterRadius();
    
    double densityInner = disk->GetDensity(innerR);
    double densityOuter = disk->GetDensity(outerR);
    
    // Inner disk should be denser
    EXPECT_GT(densityInner, densityOuter);
    EXPECT_GT(densityInner, 0.0);
    EXPECT_GT(densityOuter, 0.0);
}

TEST_F(AccretionDiskPhysicsTest, LuminosityCalculations) {
    double totalLuminosity = disk->GetTotalLuminosity();
    EXPECT_GT(totalLuminosity, 0.0);
    
    // Test efficiency
    double efficiency = disk->GetEfficiency();
    EXPECT_GT(efficiency, 0.0);
    EXPECT_LT(efficiency, 1.0); // Should be less than 100%
    
    // For Schwarzschild black hole, efficiency should be ~5.7%
    EXPECT_NEAR(efficiency, 0.057, 0.01);
}

class PhysicsEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine = std::make_unique<PhysicsEngine>();
        engine->Initialize();
        
        blackHole = std::make_unique<BlackHole>(1.98847e30, 0.0, 0.0, 0.0);
    }
    
    void TearDown() override {
        engine->Shutdown();
    }
    
    std::unique_ptr<PhysicsEngine> engine;
    std::unique_ptr<BlackHole> blackHole;
};

TEST_F(PhysicsEngineTest, CoordinateTransformations) {
    // Test Cartesian to Spherical conversion
    double x = 3.0, y = 4.0, z = 0.0;
    double r, theta, phi;
    
    engine->CartesianToSpherical(x, y, z, r, theta, phi);
    
    EXPECT_NEAR(r, 5.0, TOLERANCE); // sqrt(3^2 + 4^2)
    EXPECT_NEAR(theta, M_PI/2, TOLERANCE); // z = 0 -> theta = pi/2
    EXPECT_NEAR(phi, atan2(4.0, 3.0), TOLERANCE);
    
    // Test round-trip conversion
    double x2, y2, z2;
    engine->SphericalToCartesian(r, theta, phi, x2, y2, z2);
    
    EXPECT_NEAR(x, x2, TOLERANCE);
    EXPECT_NEAR(y, y2, TOLERANCE);
    EXPECT_NEAR(z, z2, TOLERANCE);
}

TEST_F(PhysicsEngineTest, GeodesicIntegration) {
    // Create a test particle in circular orbit
    Particle particle;
    particle.SetMass(1000.0);
    
    double rs = blackHole->GetSchwarzschildRadius();
    double r = 10.0 * rs; // Stable circular orbit
    double vCirc = blackHole->GetCircularOrbitVelocity(r);
    
    particle.SetPosition(r, 0.0, 0.0);
    particle.SetVelocity(0.0, vCirc, 0.0);
    
    // Store initial energy and angular momentum
    double initialEnergy = particle.GetOrbitalEnergy(*blackHole);
    double initialAngularMomentum = particle.GetAngularMomentum(*blackHole);
    
    // Integrate for one orbital period
    double period = blackHole->GetOrbitalPeriod(r);
    double dt = period / 1000.0; // 1000 steps per orbit
    
    for (int i = 0; i < 1000; ++i) {
        engine->UpdateParticle(particle, *blackHole, dt);
    }
    
    // Check conservation of energy and angular momentum
    double finalEnergy = particle.GetOrbitalEnergy(*blackHole);
    double finalAngularMomentum = particle.GetAngularMomentum(*blackHole);
    
    double energyError = abs((finalEnergy - initialEnergy) / initialEnergy);
    double angularMomentumError = abs((finalAngularMomentum - initialAngularMomentum) / initialAngularMomentum);
    
    EXPECT_LT(energyError, PHYSICS_TOLERANCE);
    EXPECT_LT(angularMomentumError, PHYSICS_TOLERANCE);
    
    // Check that particle is still roughly at the same radius
    double finalRadius = particle.GetRadialDistance();
    EXPECT_NEAR(finalRadius, r, r * 0.01); // 1% tolerance
}

TEST_F(PhysicsEngineTest, LightRayIntegration) {
    // Test light ray propagation
    LightRay lightRay;
    lightRay.SetPosition(100000.0, 0.0, 0.0);
    lightRay.SetDirection(-1.0, 0.0, 0.0); // Radial infall
    lightRay.SetWavelength(550e-9);
    
    // Store initial conserved quantities
    double initialEnergy = lightRay.GetConservedEnergy(*blackHole);
    double initialAngularMomentum = lightRay.GetConservedAngularMomentum(*blackHole);
    
    // Integrate until close to black hole
    double dt = 1e-6; // Small time step
    double rs = blackHole->GetSchwarzschildRadius();
    
    while (lightRay.GetRadialDistance() > 2.1 * rs && lightRay.GetStatus() == LightRay::Status::ACTIVE) {
        engine->UpdateLightRay(lightRay, *blackHole, dt);
    }
    
    // Check conservation (should be exact for light rays)
    double finalEnergy = lightRay.GetConservedEnergy(*blackHole);
    double finalAngularMomentum = lightRay.GetConservedAngularMomentum(*blackHole);
    
    EXPECT_NEAR(finalEnergy, initialEnergy, initialEnergy * 1e-12);
    EXPECT_NEAR(finalAngularMomentum, initialAngularMomentum, abs(initialAngularMomentum) * 1e-12 + 1e-20);
}

// Integration test for the complete physics system
TEST(PhysicsIntegrationTest, CompleteSystemTest) {
    // Create physics engine
    PhysicsEngine engine;
    engine.Initialize();
    
    // Create black hole
    BlackHole blackHole(10.0 * 1.98847e30, 0.0, 0.0, 0.0);
    
    // Create accretion disk
    double innerRadius = blackHole.GetISCORadius();
    double outerRadius = 100.0 * innerRadius;
    double accretionRate = 1e-8 * 1.98847e30;
    AccretionDisk disk(blackHole, innerRadius, outerRadius, accretionRate);
    
    // Create test particles
    std::vector<Particle> particles;
    for (int i = 0; i < 10; ++i) {
        Particle p;
        p.SetMass(1000.0);
        
        double r = (5.0 + i) * blackHole.GetSchwarzschildRadius();
        double v = blackHole.GetCircularOrbitVelocity(r) * 0.9; // Slightly elliptical
        
        p.SetPosition(r, 0.0, 0.0);
        p.SetVelocity(0.0, v, 0.0);
        
        particles.push_back(p);
    }
    
    // Create test light rays
    std::vector<LightRay> lightRays;
    for (int i = 0; i < 5; ++i) {
        LightRay lr;
        lr.SetPosition(50000.0, i * 1000.0, 0.0);
        lr.SetDirection(-1.0, -0.02 * i, 0.0);
        lr.SetWavelength(400e-9 + i * 50e-9); // Different colors
        
        lightRays.push_back(lr);
    }
    
    // Run simulation for a short time
    double dt = 0.01;
    int steps = 1000;
    
    for (int step = 0; step < steps; ++step) {
        // Update particles
        for (auto& particle : particles) {
            if (particle.GetRadialDistance() > blackHole.GetSchwarzschildRadius()) {
                engine.UpdateParticle(particle, blackHole, dt);
            }
        }
        
        // Update light rays
        for (auto& lightRay : lightRays) {
            if (lightRay.GetStatus() == LightRay::Status::ACTIVE) {
                engine.UpdateLightRay(lightRay, blackHole, dt);
            }
        }
        
        // Update disk
        disk.Update(dt);
    }
    
    // Verify that simulation ran without crashes
    // and that objects are in reasonable states
    int activeParticles = 0;
    int activeLightRays = 0;
    
    for (const auto& particle : particles) {
        if (particle.GetRadialDistance() > blackHole.GetSchwarzschildRadius()) {
            activeParticles++;
        }
    }
    
    for (const auto& lightRay : lightRays) {
        if (lightRay.GetStatus() == LightRay::Status::ACTIVE) {
            activeLightRays++;
        }
    }
    
    // Should have some active objects
    EXPECT_GT(activeParticles, 0);
    
    engine.Shutdown();
}

// Main function for running tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}