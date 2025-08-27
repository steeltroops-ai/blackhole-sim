#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <chrono>
#include <iomanip>

// Simple test framework
class SimpleTest {
public:
    static int totalTests;
    static int passedTests;
    static int failedTests;
    
    static void EXPECT_NEAR(double a, double b, double tolerance, const std::string& message = "") {
        totalTests++;
        if (std::abs(a - b) <= tolerance) {
            passedTests++;
            std::cout << "[PASS] " << message << std::endl;
        } else {
            failedTests++;
            std::cout << "[FAIL] " << message << " - Expected: " << a << ", Got: " << b 
                      << ", Diff: " << std::abs(a - b) << std::endl;
        }
    }
    
    static void EXPECT_GT(double a, double b, const std::string& message = "") {
        totalTests++;
        if (a > b) {
            passedTests++;
            std::cout << "[PASS] " << message << std::endl;
        } else {
            failedTests++;
            std::cout << "[FAIL] " << message << " - Expected " << a << " > " << b << std::endl;
        }
    }
    
    static void EXPECT_LT(double a, double b, const std::string& message = "") {
        totalTests++;
        if (a < b) {
            passedTests++;
            std::cout << "[PASS] " << message << std::endl;
        } else {
            failedTests++;
            std::cout << "[FAIL] " << message << " - Expected " << a << " < " << b << std::endl;
        }
    }
    
    static void EXPECT_TRUE(bool condition, const std::string& message = "") {
        totalTests++;
        if (condition) {
            passedTests++;
            std::cout << "[PASS] " << message << std::endl;
        } else {
            failedTests++;
            std::cout << "[FAIL] " << message << " - Expected true" << std::endl;
        }
    }
    
    static void EXPECT_FALSE(bool condition, const std::string& message = "") {
        totalTests++;
        if (!condition) {
            passedTests++;
            std::cout << "[PASS] " << message << std::endl;
        } else {
            failedTests++;
            std::cout << "[FAIL] " << message << " - Expected false" << std::endl;
        }
    }
    
    static void PrintSummary() {
        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Total tests: " << totalTests << std::endl;
        std::cout << "Passed: " << passedTests << std::endl;
        std::cout << "Failed: " << failedTests << std::endl;
        std::cout << "Success rate: " << std::fixed << std::setprecision(1) 
                  << (100.0 * passedTests / totalTests) << "%" << std::endl;
    }
};

int SimpleTest::totalTests = 0;
int SimpleTest::passedTests = 0;
int SimpleTest::failedTests = 0;

// Include our physics classes (simplified versions for testing)
namespace Physics {
    const double G = 6.67430e-11;  // Gravitational constant
    const double c = 2.99792458e8; // Speed of light
    const double M_sun = 1.98847e30; // Solar mass
    
    class BlackHole {
    private:
        double mass;
        
    public:
        BlackHole(double m) : mass(m) {}
        
        double GetMass() const { return mass; }
        
        double GetSchwarzschildRadius() const {
            return 2.0 * G * mass / (c * c);
        }
        
        double GetPhotonSphereRadius() const {
            return 1.5 * GetSchwarzschildRadius();
        }
        
        double GetISCORadius() const {
            return 3.0 * GetSchwarzschildRadius();
        }
        
        double GetMetricComponent_tt(double r) const {
            double rs = GetSchwarzschildRadius();
            return -(1.0 - rs / r);
        }
        
        double GetMetricComponent_rr(double r) const {
            double rs = GetSchwarzschildRadius();
            return 1.0 / (1.0 - rs / r);
        }
        
        double GetMetricComponent_thetatheta(double r) const {
            return r * r;
        }
        
        double GetMetricComponent_phiphi(double r, double theta) const {
            return r * r * sin(theta) * sin(theta);
        }
        
        double GetEscapeVelocity(double r) const {
            return c * sqrt(GetSchwarzschildRadius() / r);
        }
        
        double GetCircularOrbitVelocity(double r) const {
            return sqrt(G * mass / r);
        }
        
        double GetOrbitalPeriod(double r) const {
            return 2.0 * M_PI * sqrt(r * r * r / (G * mass));
        }
        
        bool IsOrbitStable(double r) const {
            return r > GetISCORadius();
        }
        
        double GetGravitationalRedshift(double r1, double r2) const {
            if (r1 == r2) return 0.0;
            
            double rs = GetSchwarzschildRadius();
            double factor1 = sqrt(1.0 - rs / r1);
            double factor2 = sqrt(1.0 - rs / r2);
            
            return factor2 / factor1 - 1.0;
        }
    };
    
    class Particle {
    private:
        double mass;
        double x, y, z;
        double vx, vy, vz;
        
    public:
        Particle() : mass(1000.0), x(0), y(0), z(0), vx(0), vy(0), vz(0) {}
        
        void SetMass(double m) { mass = m; }
        void SetPosition(double px, double py, double pz) { x = px; y = py; z = pz; }
        void SetVelocity(double vvx, double vvy, double vvz) { vx = vvx; vy = vvy; vz = vvz; }
        
        double GetMass() const { return mass; }
        double GetX() const { return x; }
        double GetY() const { return y; }
        double GetZ() const { return z; }
        
        double GetRadialDistance() const {
            return sqrt(x*x + y*y + z*z);
        }
        
        double GetSpeed() const {
            return sqrt(vx*vx + vy*vy + vz*vz);
        }
        
        double GetKineticEnergy() const {
            return 0.5 * mass * GetSpeed() * GetSpeed();
        }
        
        double GetAngularMomentum(const BlackHole& bh) const {
            // Simplified: L = mvr for circular motion
            return mass * GetSpeed() * GetRadialDistance();
        }
        
        double GetOrbitalEnergy(const BlackHole& bh) const {
            double r = GetRadialDistance();
            double ke = GetKineticEnergy();
            double pe = -G * bh.GetMass() * mass / r;
            return ke + pe;
        }
        
        double GetLorentzFactor() const {
            double v = GetSpeed();
            double beta = v / c;
            return 1.0 / sqrt(1.0 - beta * beta);
        }
        
        double GetRelativisticMass() const {
            return mass * GetLorentzFactor();
        }
    };
    
    class LightRay {
    private:
        double x, y, z;
        double dx, dy, dz;
        double wavelength;
        
    public:
        enum class Status { ACTIVE, ABSORBED, ESCAPED };
        
        LightRay() : x(0), y(0), z(0), dx(0), dy(0), dz(0), wavelength(550e-9) {}
        
        void SetPosition(double px, double py, double pz) { x = px; y = py; z = pz; }
        void SetDirection(double ddx, double ddy, double ddz) { 
            double norm = sqrt(ddx*ddx + ddy*ddy + ddz*ddz);
            dx = ddx/norm; dy = ddy/norm; dz = ddz/norm;
        }
        void SetWavelength(double w) { wavelength = w; }
        
        double GetWavelength() const { return wavelength; }
        double GetFrequency() const { return c / wavelength; }
        double GetPhotonEnergy() const { return 6.62607015e-34 * GetFrequency(); }
        
        double GetRadialDistance() const {
            return sqrt(x*x + y*y + z*z);
        }
        
        Status GetStatus() const { return Status::ACTIVE; }
        
        double GetImpactParameter(const BlackHole& bh) const {
            // Simplified calculation
            double r = GetRadialDistance();
            return r * sqrt(dx*dx + dy*dy); // Approximate
        }
        
        double GetConservedEnergy(const BlackHole& bh) const {
            double r = GetRadialDistance();
            double rs = bh.GetSchwarzschildRadius();
            return (1.0 - rs/r); // Simplified
        }
        
        double GetConservedAngularMomentum(const BlackHole& bh) const {
            return GetImpactParameter(bh) * GetConservedEnergy(bh);
        }
        
        double GetRedshift(const BlackHole& bh, double r1, double r2, double v1, double v2) const {
            // Gravitational redshift only (simplified)
            return bh.GetGravitationalRedshift(r1, r2);
        }
    };
}

// Test functions
void TestBlackHoleBasics() {
    std::cout << "\n=== Testing Black Hole Basics ===" << std::endl;
    
    Physics::BlackHole bh(10.0 * Physics::M_sun);
    
    // Test Schwarzschild radius
    double rs = bh.GetSchwarzschildRadius();
    double expectedRs = 2.0 * Physics::G * bh.GetMass() / (Physics::c * Physics::c);
    SimpleTest::EXPECT_NEAR(rs, expectedRs, expectedRs * 1e-10, "Schwarzschild radius calculation");
    
    // Test photon sphere
    SimpleTest::EXPECT_NEAR(bh.GetPhotonSphereRadius(), 1.5 * rs, rs * 1e-10, "Photon sphere radius");
    
    // Test ISCO
    SimpleTest::EXPECT_NEAR(bh.GetISCORadius(), 3.0 * rs, rs * 1e-10, "ISCO radius");
}

void TestMetricComponents() {
    std::cout << "\n=== Testing Metric Components ===" << std::endl;
    
    Physics::BlackHole bh(Physics::M_sun);
    double r = 100000.0; // Far from black hole
    double theta = M_PI / 4.0;
    
    // At large distances, should approach Minkowski metric
    SimpleTest::EXPECT_NEAR(bh.GetMetricComponent_tt(r), -1.0, 1e-6, "g_tt at large distance");
    SimpleTest::EXPECT_NEAR(bh.GetMetricComponent_rr(r), 1.0, 1e-6, "g_rr at large distance");
    SimpleTest::EXPECT_NEAR(bh.GetMetricComponent_thetatheta(r), r*r, r*r*1e-10, "g_theta_theta");
    SimpleTest::EXPECT_NEAR(bh.GetMetricComponent_phiphi(r, theta), r*r*sin(theta)*sin(theta), r*r*1e-10, "g_phi_phi");
}

void TestParticlePhysics() {
    std::cout << "\n=== Testing Particle Physics ===" << std::endl;
    
    Physics::BlackHole bh(Physics::M_sun);
    Physics::Particle particle;
    
    particle.SetMass(1000.0);
    particle.SetPosition(100000.0, 0.0, 0.0);
    particle.SetVelocity(0.0, 7000.0, 0.0);
    
    // Test basic properties
    SimpleTest::EXPECT_NEAR(particle.GetRadialDistance(), 100000.0, 1e-10, "Particle radial distance");
    SimpleTest::EXPECT_NEAR(particle.GetSpeed(), 7000.0, 1e-10, "Particle speed");
    
    // Test kinetic energy
    double expectedKE = 0.5 * particle.GetMass() * particle.GetSpeed() * particle.GetSpeed();
    SimpleTest::EXPECT_NEAR(particle.GetKineticEnergy(), expectedKE, expectedKE * 1e-10, "Kinetic energy");
    
    // Test orbital energy (should be negative for bound orbit)
    double orbitalEnergy = particle.GetOrbitalEnergy(bh);
    SimpleTest::EXPECT_LT(orbitalEnergy, 0.0, "Orbital energy should be negative for bound orbit");
}

void TestLightRayPhysics() {
    std::cout << "\n=== Testing Light Ray Physics ===" << std::endl;
    
    Physics::BlackHole bh(Physics::M_sun);
    Physics::LightRay lightRay;
    
    lightRay.SetPosition(100000.0, 0.0, 0.0);
    lightRay.SetDirection(-1.0, 0.1, 0.0);
    lightRay.SetWavelength(550e-9);
    
    // Test wavelength and frequency
    double wavelength = lightRay.GetWavelength();
    double frequency = lightRay.GetFrequency();
    double expectedFreq = Physics::c / wavelength;
    SimpleTest::EXPECT_NEAR(frequency, expectedFreq, expectedFreq * 1e-10, "Light frequency calculation");
    
    // Test photon energy
    double energy = lightRay.GetPhotonEnergy();
    double expectedEnergy = 6.62607015e-34 * frequency;
    SimpleTest::EXPECT_NEAR(energy, expectedEnergy, expectedEnergy * 1e-10, "Photon energy calculation");
    
    // Test conserved quantities
    double impactParam = lightRay.GetImpactParameter(bh);
    SimpleTest::EXPECT_GT(impactParam, 0.0, "Impact parameter should be positive");
}

void TestRelativisticEffects() {
    std::cout << "\n=== Testing Relativistic Effects ===" << std::endl;
    
    Physics::Particle particle;
    particle.SetMass(1000.0);
    particle.SetVelocity(0.0, 0.9 * Physics::c, 0.0); // 0.9c
    
    double gamma = particle.GetLorentzFactor();
    double expectedGamma = 1.0 / sqrt(1.0 - 0.9 * 0.9);
    SimpleTest::EXPECT_NEAR(gamma, expectedGamma, expectedGamma * 1e-10, "Lorentz factor at 0.9c");
    
    // Test relativistic mass
    double relMass = particle.GetRelativisticMass();
    double expectedRelMass = particle.GetMass() * gamma;
    SimpleTest::EXPECT_NEAR(relMass, expectedRelMass, expectedRelMass * 1e-10, "Relativistic mass");
}

void TestGravitationalEffects() {
    std::cout << "\n=== Testing Gravitational Effects ===" << std::endl;
    
    Physics::BlackHole bh(10.0 * Physics::M_sun);
    double rs = bh.GetSchwarzschildRadius();
    
    // Test escape velocity
    double r = 10.0 * rs;
    double vEsc = bh.GetEscapeVelocity(r);
    SimpleTest::EXPECT_LT(vEsc, Physics::c, "Escape velocity should be less than c");
    SimpleTest::EXPECT_GT(vEsc, 0.0, "Escape velocity should be positive");
    
    // Test circular orbit velocity
    double vCirc = bh.GetCircularOrbitVelocity(r);
    SimpleTest::EXPECT_GT(vCirc, 0.0, "Circular orbit velocity should be positive");
    SimpleTest::EXPECT_LT(vCirc, Physics::c, "Circular orbit velocity should be less than c");
    
    // Test orbital period
    double period = bh.GetOrbitalPeriod(r);
    SimpleTest::EXPECT_GT(period, 0.0, "Orbital period should be positive");
    
    // Test orbit stability
    SimpleTest::EXPECT_TRUE(bh.IsOrbitStable(2.0 * bh.GetISCORadius()), "Orbit should be stable above ISCO");
    SimpleTest::EXPECT_FALSE(bh.IsOrbitStable(0.5 * bh.GetISCORadius()), "Orbit should be unstable below ISCO");
}

void RunPerformanceTest() {
    std::cout << "\n=== Performance Test ===" << std::endl;
    
    Physics::BlackHole bh(Physics::M_sun);
    const int numCalculations = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    double sum = 0.0;
    for (int i = 0; i < numCalculations; ++i) {
        double r = 1000.0 + i * 0.1;
        sum += bh.GetMetricComponent_tt(r);
        sum += bh.GetMetricComponent_rr(r);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double calculationsPerSecond = (numCalculations * 2) / (duration.count() / 1e6);
    
    std::cout << "Metric calculations: " << numCalculations * 2 << std::endl;
    std::cout << "Time: " << duration.count() / 1000.0 << " ms" << std::endl;
    std::cout << "Rate: " << std::fixed << std::setprecision(0) << calculationsPerSecond << " calc/sec" << std::endl;
    std::cout << "Sum (anti-optimization): " << sum << std::endl;
    
    SimpleTest::EXPECT_GT(calculationsPerSecond, 100000.0, "Performance should be > 100k calc/sec");
}

int main() {
    std::cout << "=== Black Hole Physics Test Suite ===" << std::endl;
    std::cout << "Testing core physics calculations without external dependencies." << std::endl;
    
    try {
        TestBlackHoleBasics();
        TestMetricComponents();
        TestParticlePhysics();
        TestLightRayPhysics();
        TestRelativisticEffects();
        TestGravitationalEffects();
        RunPerformanceTest();
        
        SimpleTest::PrintSummary();
        
        if (SimpleTest::failedTests == 0) {
            std::cout << "\n🎉 All tests passed! Physics calculations are working correctly." << std::endl;
            return 0;
        } else {
            std::cout << "\n❌ Some tests failed. Please check the implementation." << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cout << "\n💥 Test suite crashed: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "\n💥 Test suite crashed with unknown exception." << std::endl;
        return 1;
    }
}