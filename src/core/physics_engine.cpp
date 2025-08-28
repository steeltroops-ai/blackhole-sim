/**
 * @file PhysicsEngine.cpp
 * @brief Implementation of the PhysicsEngine class
 */

#include "blackhole_sim/physics_engine.hpp"
#include "blackhole_sim/black_hole.hpp"
#include "blackhole_sim/particle.hpp"
#include "blackhole_sim/light_ray.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace BlackHoleSim {

// Physical constants
const double G = 6.67430e-11;  // Gravitational constant (m³/kg⋅s²)
const double c = 299792458.0;  // Speed of light (m/s)
const double c2 = c * c;       // Speed of light squared

PhysicsEngine::PhysicsEngine()
    : m_integrationMethod(IntegrationMethod::RUNGE_KUTTA_4)
    , m_tolerance(1e-8)
    , m_maxStepSize(1e-2)
    , m_minStepSize(1e-12)
{
}

bool PhysicsEngine::Initialize(std::shared_ptr<BlackHole> blackHole) {
    m_blackHole = blackHole;
    // Allow initialization without black hole - it can be set later
    return true;
}

void PhysicsEngine::Update(double deltaTime, 
                          std::vector<std::unique_ptr<Particle>>& particles,
                          std::vector<std::unique_ptr<LightRay>>& lightRays) {
    // Update particles
    for (auto& particle : particles) {
        if (particle) {
            // Simple position update - would use geodesic integration in full implementation
            // This is a placeholder for the complex geodesic calculations
        }
    }
    
    // Update light rays
    for (auto& lightRay : lightRays) {
        if (lightRay) {
            // Simple light ray update - would use null geodesic integration
            // This is a placeholder for the complex light ray calculations
        }
    }
}

void PhysicsEngine::CalculateMassiveGeodesicDerivatives(const std::array<double, 8>& state,
                                                       std::array<double, 8>& derivatives) const {
    if (!m_blackHole) return;
    
    double r = state[1];
    double theta = state[2];
    double rs = 2.0 * G * m_blackHole->GetMass() / c2; // Schwarzschild radius
    
    // Simplified geodesic derivatives for Schwarzschild metric
    // This is a basic implementation - full version would include all Christoffel symbols
    derivatives[0] = state[4]; // dt/dtau
    derivatives[1] = state[5]; // dr/dtau
    derivatives[2] = state[6]; // dtheta/dtau
    derivatives[3] = state[7]; // dphi/dtau
    
    // Second derivatives (simplified)
    derivatives[4] = 0.0; // d²t/dtau²
    derivatives[5] = -rs / (2.0 * r * r) * state[4] * state[4]; // d²r/dtau²
    derivatives[6] = 0.0; // d²theta/dtau²
    derivatives[7] = 0.0; // d²phi/dtau²
}

void PhysicsEngine::CalculateNullGeodesicDerivatives(const std::array<double, 8>& state,
                                                    std::array<double, 8>& derivatives) const {
    if (!m_blackHole) return;
    
    double r = state[1];
    double rs = 2.0 * G * m_blackHole->GetMass() / c2;
    
    // Simplified null geodesic derivatives
    derivatives[0] = state[4]; // dt/dlambda
    derivatives[1] = state[5]; // dr/dlambda
    derivatives[2] = state[6]; // dtheta/dlambda
    derivatives[3] = state[7]; // dphi/dlambda
    
    // Second derivatives (simplified)
    derivatives[4] = 0.0;
    derivatives[5] = -rs / (2.0 * r * r) * state[4] * state[4];
    derivatives[6] = 0.0;
    derivatives[7] = 0.0;
}

double PhysicsEngine::CalculateLensingDeflection(double impactParameter, double sourceDistance) const {
    if (!m_blackHole) return 0.0;
    
    double rs = 2.0 * G * m_blackHole->GetMass() / c2;
    // Simplified lensing calculation
    return 4.0 * rs / impactParameter;
}

double PhysicsEngine::CalculateGravitationalRedshift(double emissionRadius, double observerRadius) const {
    if (!m_blackHole) return 0.0;
    
    double rs = 2.0 * G * m_blackHole->GetMass() / c2;
    double factor_emission = std::sqrt(1.0 - rs / emissionRadius);
    double factor_observer = std::sqrt(1.0 - rs / observerRadius);
    
    return factor_observer / factor_emission - 1.0;
}

double PhysicsEngine::CalculateDopplerRedshift(double velocity, double angle) const {
    double beta = velocity / c;
    double gamma = 1.0 / std::sqrt(1.0 - beta * beta);
    return gamma * (1.0 + beta * std::cos(angle)) - 1.0;
}

double PhysicsEngine::CalculateEffectivePotential(double r, double angularMomentum) const {
    if (!m_blackHole) return 0.0;
    
    double rs = 2.0 * G * m_blackHole->GetMass() / c2;
    double L2 = angularMomentum * angularMomentum;
    
    return -rs / (2.0 * r) + L2 / (2.0 * r * r) - rs * L2 / (r * r * r);
}

double PhysicsEngine::FindCircularOrbitRadius(double angularMomentum) const {
    if (!m_blackHole) return 0.0;
    
    double rs = 2.0 * G * m_blackHole->GetMass() / c2;
    double L2 = angularMomentum * angularMomentum;
    
    // Simplified calculation - would need numerical solver for exact result
    return L2 / rs;
}

bool PhysicsEngine::IsOrbitStable(double r, double angularMomentum) const {
    if (!m_blackHole) return false;
    
    double rs = 2.0 * G * m_blackHole->GetMass() / c2;
    return r > 3.0 * rs; // Innermost stable circular orbit approximation
}

std::array<double, 3> PhysicsEngine::CalculateConservedQuantities(const std::array<double, 8>& state) const {
    if (!m_blackHole) return {0.0, 0.0, 0.0};
    
    double r = state[1];
    double rs = 2.0 * G * m_blackHole->GetMass() / c2;
    
    // Energy (simplified)
    double energy = (1.0 - rs / r) * state[4];
    
    // Angular momentum
    double angularMomentum = r * r * state[7];
    
    // Carter constant (0 for Schwarzschild)
    double carter = 0.0;
    
    return {energy, angularMomentum, carter};
}

void PhysicsEngine::IntegrateRK4(std::array<double, 8>& state,
                                std::function<void(const std::array<double, 8>&, std::array<double, 8>&)> derivatives,
                                double stepSize) const {
    std::array<double, 8> k1, k2, k3, k4, temp;
    
    // k1 = f(t, y)
    derivatives(state, k1);
    
    // k2 = f(t + h/2, y + h*k1/2)
    for (int i = 0; i < 8; ++i) {
        temp[i] = state[i] + stepSize * k1[i] / 2.0;
    }
    derivatives(temp, k2);
    
    // k3 = f(t + h/2, y + h*k2/2)
    for (int i = 0; i < 8; ++i) {
        temp[i] = state[i] + stepSize * k2[i] / 2.0;
    }
    derivatives(temp, k3);
    
    // k4 = f(t + h, y + h*k3)
    for (int i = 0; i < 8; ++i) {
        temp[i] = state[i] + stepSize * k3[i];
    }
    derivatives(temp, k4);
    
    // y_{n+1} = y_n + h/6 * (k1 + 2*k2 + 2*k3 + k4)
    for (int i = 0; i < 8; ++i) {
        state[i] += stepSize / 6.0 * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
    }
}

bool PhysicsEngine::IntegrateAdaptiveRK45(std::array<double, 8>& state,
                                         std::function<void(const std::array<double, 8>&, std::array<double, 8>&)> derivatives,
                                         double& stepSize) const {
    // Simplified adaptive step - would use Dormand-Prince coefficients in full implementation
    std::array<double, 8> originalState = state;
    
    // Try full step
    IntegrateRK4(state, derivatives, stepSize);
    
    // For now, just return success - would implement error estimation in full version
    return true;
}

PhysicsEngine::ChristoffelSymbols PhysicsEngine::CalculateChristoffelSymbols(double r, double theta) const {
    if (!m_blackHole) return {};
    
    double rs = 2.0 * G * m_blackHole->GetMass() / c2;
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);
    
    ChristoffelSymbols symbols = {};
    
    // Simplified Christoffel symbols for Schwarzschild metric
    symbols.Gamma_t_tr = rs / (2.0 * r * (r - rs));
    symbols.Gamma_t_rt = symbols.Gamma_t_tr;
    
    symbols.Gamma_r_tt = rs * (r - rs) / (2.0 * r * r * r);
    symbols.Gamma_r_rr = -rs / (2.0 * r * (r - rs));
    symbols.Gamma_r_theta_theta = -(r - rs);
    symbols.Gamma_r_phi_phi = -(r - rs) * sin_theta * sin_theta;
    
    symbols.Gamma_theta_r_theta = 1.0 / r;
    symbols.Gamma_theta_theta_r = symbols.Gamma_theta_r_theta;
    symbols.Gamma_theta_phi_phi = -sin_theta * cos_theta;
    
    symbols.Gamma_phi_r_phi = 1.0 / r;
    symbols.Gamma_phi_phi_r = symbols.Gamma_phi_r_phi;
    symbols.Gamma_phi_theta_phi = cos_theta / sin_theta;
    symbols.Gamma_phi_phi_theta = symbols.Gamma_phi_theta_phi;
    
    return symbols;
}

bool PhysicsEngine::ValidateGeodesicState(const std::array<double, 8>& state) const {
    // Check for NaN or infinite values
    for (double value : state) {
        if (std::isnan(value) || std::isinf(value)) {
            return false;
        }
    }
    
    // Check if radial coordinate is positive and outside event horizon
    if (m_blackHole) {
        double rs = 2.0 * G * m_blackHole->GetMass() / c2;
        if (state[1] <= rs) {
            return false;
        }
    }
    
    return true;
}

} // namespace BlackHoleSim