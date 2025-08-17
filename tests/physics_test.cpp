#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Test macros
#define EXPECT_NEAR(a, b, tolerance) \
    do { \
        if (std::abs((a) - (b)) > (tolerance)) { \
            std::cout << "FAIL: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")" << std::endl; \
            return false; \
        } \
    } while(0)

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cout << "FAIL: " << #condition << " is false" << std::endl; \
            return false; \
        } \
    } while(0)

// Simple BlackHole class for testing
class BlackHole {
public:
    double mass;
    double schwarzschild_radius;
    
    BlackHole(double m) : mass(m) {
        // G = 6.674e-11, c = 299792458
        // Rs = 2GM/c^2
        const double G = 6.674e-11;
        const double c = 299792458.0;
        schwarzschild_radius = 2.0 * G * mass / (c * c);
    }
    
    // Schwarzschild metric component g_tt
    double GetMetricTT(double r) const {
        if (r <= schwarzschild_radius) return 0.0;
        return -(1.0 - schwarzschild_radius / r);
    }
    
    // Schwarzschild metric component g_rr
    double GetMetricRR(double r) const {
        if (r <= schwarzschild_radius) return 1e10; // Very large number instead of infinity
        return 1.0 / (1.0 - schwarzschild_radius / r);
    }
    
    // Gravitational redshift factor
    double GetRedshiftFactor(double r) const {
        if (r <= schwarzschild_radius) return 0.0;
        return sqrt(1.0 - schwarzschild_radius / r);
    }
};

// Test functions
bool TestBlackHoleCreation() {
    std::cout << "Testing BlackHole creation..." << std::endl;
    
    // Solar mass black hole
    double solar_mass = 1.989e30; // kg
    BlackHole bh(solar_mass);
    
    // Expected Schwarzschild radius for solar mass: ~2.95 km
    EXPECT_NEAR(bh.schwarzschild_radius, 2950.0, 100.0);
    EXPECT_TRUE(bh.mass == solar_mass);
    
    std::cout << "✓ BlackHole creation test passed" << std::endl;
    return true;
}

bool TestSchwarzschildMetric() {
    std::cout << "Testing Schwarzschild metric components..." << std::endl;
    
    double solar_mass = 1.989e30;
    BlackHole bh(solar_mass);
    
    double r = 10.0 * bh.schwarzschild_radius; // 10 Rs
    
    // At 10 Rs, g_tt should be -0.9
    double g_tt = bh.GetMetricTT(r);
    EXPECT_NEAR(g_tt, -0.9, 0.01);
    
    // At 10 Rs, g_rr should be 1/0.9 ≈ 1.111
    double g_rr = bh.GetMetricRR(r);
    EXPECT_NEAR(g_rr, 1.111, 0.01);
    
    // At the Schwarzschild radius, g_tt should be 0
    EXPECT_NEAR(bh.GetMetricTT(bh.schwarzschild_radius), 0.0, 1e-10);
    
    std::cout << "✓ Schwarzschild metric test passed" << std::endl;
    return true;
}

bool TestGravitationalRedshift() {
    std::cout << "Testing gravitational redshift..." << std::endl;
    
    double solar_mass = 1.989e30;
    BlackHole bh(solar_mass);
    
    // At infinity, redshift factor should be 1
    double r_inf = 1e10 * bh.schwarzschild_radius;
    EXPECT_NEAR(bh.GetRedshiftFactor(r_inf), 1.0, 1e-6);
    
    // At 10 Rs, redshift factor should be sqrt(0.9) ≈ 0.949
    double r_10rs = 10.0 * bh.schwarzschild_radius;
    EXPECT_NEAR(bh.GetRedshiftFactor(r_10rs), 0.949, 0.01);
    
    // At the Schwarzschild radius, redshift factor should be 0
    EXPECT_NEAR(bh.GetRedshiftFactor(bh.schwarzschild_radius), 0.0, 1e-10);
    
    std::cout << "✓ Gravitational redshift test passed" << std::endl;
    return true;
}

bool TestPhysicalConstants() {
    std::cout << "Testing physical constants and calculations..." << std::endl;
    
    // Test that our calculations are reasonable
    double earth_mass = 5.972e24; // kg
    BlackHole earth_bh(earth_mass);
    
    // Earth's Schwarzschild radius should be about 8.87 mm
    EXPECT_TRUE(earth_bh.schwarzschild_radius < 0.01); // Less than 1 cm
    EXPECT_TRUE(earth_bh.schwarzschild_radius > 0.008); // Greater than 8 mm
    
    std::cout << "✓ Physical constants test passed" << std::endl;
    return true;
}

int main() {
    std::cout << "=== BlackHole Physics Test Suite ===" << std::endl;
    
    int tests_passed = 0;
    int total_tests = 4;
    
    if (TestBlackHoleCreation()) tests_passed++;
    if (TestSchwarzschildMetric()) tests_passed++;
    if (TestGravitationalRedshift()) tests_passed++;
    if (TestPhysicalConstants()) tests_passed++;
    
    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Tests passed: " << tests_passed << "/" << total_tests << std::endl;
    
    if (tests_passed == total_tests) {
        std::cout << "🎉 All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "❌ Some tests failed!" << std::endl;
        return 1;
    }
}