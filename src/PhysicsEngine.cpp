/**
 * @file PhysicsEngine.cpp
 * @brief Implementation of the PhysicsEngine class
 */

#include "PhysicsEngine.hpp"
#include "BlackHole.hpp"
#include "Particle.hpp"
#include "LightRay.hpp"
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
    : m_integrationMethod(IntegrationMethod::RK4)
    , m_errorTolerance(1e-8)
    , m_minStepSize(1e-12)
    , m_maxStepSize(1e-2)
    , m_maxIterations(10000)
    , m_conservationTolerance(1e-6)
    , m_isInitialized(false)
{
}

PhysicsEngine::~PhysicsEngine() {
    Shutdown();
}

bool PhysicsEngine::Initialize() {
    if (m_isInitialized) {
        return true;
    }
    
    std::cout << "Initializing PhysicsEngine...\n";
    
    // Initialize any required resources
    // (Currently no special initialization needed)
    
    m_isInitialized = true;
    std::cout << "PhysicsEngine initialized successfully\n";
    return true;
}

void PhysicsEngine::Shutdown() {
    if (!m_isInitialized) {
        return;
    }
    
    // Clean up any resources
    m_isInitialized = false;
    std::cout << "PhysicsEngine shutdown complete\n";
}

void PhysicsEngine::UpdateParticle(Particle& particle, const BlackHole& blackHole, double timeStep) {
    if (!m_isInitialized) {
        throw std::runtime_error("PhysicsEngine not initialized");
    }
    
    if (!particle.IsActive()) {
        return;
    }
    
    // Get current state
    Vector3 position = particle.GetPosition();
    Vector3 velocity = particle.GetVelocity();
    
    // Convert to spherical coordinates relative to black hole
    Vector3 relativePos = position - blackHole.GetPosition();
    SphericalCoords spherical = CartesianToSpherical(relativePos);
    
    // Check if particle has crossed event horizon
    if (spherical.r <= blackHole.GetSchwarzschildRadius()) {
        particle.SetActive(false);
        return;
    }
    
    // Convert velocity to spherical coordinates
    SphericalVelocity sphericalVel = CartesianToSphericalVelocity(velocity, spherical);
    
    // Create geodesic state vector
    GeodesicState state;
    state.t = 0.0; // We'll track coordinate time separately
    state.r = spherical.r;
    state.theta = spherical.theta;
    state.phi = spherical.phi;
    state.dt_dtau = 1.0; // Initial guess, will be corrected
    state.dr_dtau = sphericalVel.dr_dt;
    state.dtheta_dtau = sphericalVel.dtheta_dt;
    state.dphi_dtau = sphericalVel.dphi_dt;
    
    // Normalize the 4-velocity for massive particles
    NormalizeMassiveParticle4Velocity(state, blackHole);
    
    // Store initial conserved quantities for validation
    double initialEnergy = CalculateEnergy(state, blackHole);
    double initialAngularMomentum = CalculateAngularMomentum(state);
    
    // Integrate geodesic
    GeodesicState newState = state;
    bool success = false;
    
    switch (m_integrationMethod) {
        case IntegrationMethod::EULER:
            success = IntegrateEuler(newState, blackHole, timeStep, true);
            break;
        case IntegrationMethod::RK4:
            success = IntegrateRK4(newState, blackHole, timeStep, true);
            break;
        case IntegrationMethod::ADAPTIVE_RK45:
            success = IntegrateAdaptiveRK45(newState, blackHole, timeStep, true);
            break;
    }
    
    if (!success) {
        std::cerr << "Warning: Geodesic integration failed for particle\n";
        return;
    }
    
    // Validate conservation laws
    double finalEnergy = CalculateEnergy(newState, blackHole);
    double finalAngularMomentum = CalculateAngularMomentum(newState);
    
    double energyError = std::abs((finalEnergy - initialEnergy) / initialEnergy);
    double angularMomentumError = std::abs((finalAngularMomentum - initialAngularMomentum) / initialAngularMomentum);
    
    if (energyError > m_conservationTolerance || angularMomentumError > m_conservationTolerance) {
        std::cerr << "Warning: Conservation violation - Energy error: " << energyError 
                  << ", Angular momentum error: " << angularMomentumError << "\n";
    }
    
    // Convert back to Cartesian coordinates
    SphericalCoords newSpherical = {newState.r, newState.theta, newState.phi};
    Vector3 newRelativePos = SphericalToCartesian(newSpherical);
    Vector3 newPosition = newRelativePos + blackHole.GetPosition();
    
    // Convert velocity back to Cartesian
    SphericalVelocity newSphericalVel = {
        newState.dr_dtau / newState.dt_dtau,
        newState.dtheta_dtau / newState.dt_dtau,
        newState.dphi_dtau / newState.dt_dtau
    };
    Vector3 newVelocity = SphericalToCartesianVelocity(newSphericalVel, newSpherical);
    
    // Update particle state
    particle.SetPosition(newPosition);
    particle.SetVelocity(newVelocity);
    
    // Add to trajectory history
    particle.AddToTrajectory(newPosition);
    
    // Check for escape condition
    if (newState.r > 1000.0 * blackHole.GetSchwarzschildRadius()) {
        particle.SetActive(false);
    }
}

void PhysicsEngine::UpdateLightRay(LightRay& lightRay, const BlackHole& blackHole, double timeStep) {
    if (!m_isInitialized) {
        throw std::runtime_error("PhysicsEngine not initialized");
    }
    
    if (!lightRay.IsActive()) {
        return;
    }
    
    // Get current state
    Vector3 position = lightRay.GetPosition();
    Vector3 direction = lightRay.GetDirection();
    
    // Convert to spherical coordinates relative to black hole
    Vector3 relativePos = position - blackHole.GetPosition();
    SphericalCoords spherical = CartesianToSpherical(relativePos);
    
    // Check if light ray has crossed event horizon
    if (spherical.r <= blackHole.GetSchwarzschildRadius()) {
        lightRay.SetStatus(LightRay::Status::ABSORBED);
        return;
    }
    
    // Convert direction to spherical coordinates
    SphericalVelocity sphericalDir = CartesianToSphericalVelocity(direction, spherical);
    
    // Create null geodesic state vector
    GeodesicState state;
    state.t = 0.0;
    state.r = spherical.r;
    state.theta = spherical.theta;
    state.phi = spherical.phi;
    state.dt_dtau = 1.0; // For null geodesics, we parameterize by coordinate time
    state.dr_dtau = sphericalDir.dr_dt;
    state.dtheta_dtau = sphericalDir.dtheta_dt;
    state.dphi_dtau = sphericalDir.dphi_dt;
    
    // Normalize for null geodesics (ds² = 0)
    NormalizeNullGeodesic4Velocity(state, blackHole);
    
    // Store initial conserved quantities
    double initialEnergy = CalculatePhotonEnergy(state, blackHole);
    double initialAngularMomentum = CalculateAngularMomentum(state);
    
    // Integrate null geodesic
    GeodesicState newState = state;
    bool success = false;
    
    switch (m_integrationMethod) {
        case IntegrationMethod::EULER:
            success = IntegrateEuler(newState, blackHole, timeStep, false);
            break;
        case IntegrationMethod::RK4:
            success = IntegrateRK4(newState, blackHole, timeStep, false);
            break;
        case IntegrationMethod::ADAPTIVE_RK45:
            success = IntegrateAdaptiveRK45(newState, blackHole, timeStep, false);
            break;
    }
    
    if (!success) {
        std::cerr << "Warning: Null geodesic integration failed for light ray\n";
        return;
    }
    
    // Convert back to Cartesian coordinates
    SphericalCoords newSpherical = {newState.r, newState.theta, newState.phi};
    Vector3 newRelativePos = SphericalToCartesian(newSpherical);
    Vector3 newPosition = newRelativePos + blackHole.GetPosition();
    
    // Convert direction back to Cartesian
    SphericalVelocity newSphericalDir = {
        newState.dr_dtau,
        newState.dtheta_dtau,
        newState.dphi_dtau
    };
    Vector3 newDirection = SphericalToCartesianVelocity(newSphericalDir, newSpherical);
    newDirection = newDirection.Normalized();
    
    // Calculate gravitational redshift
    double redshiftFactor = blackHole.GetGravitationalRedshift(spherical.r, newState.r);
    double newFrequency = lightRay.GetFrequency() * redshiftFactor;
    
    // Update light ray state
    lightRay.SetPosition(newPosition);
    lightRay.SetDirection(newDirection);
    lightRay.SetFrequency(newFrequency);
    
    // Add to path history
    lightRay.AddToPath(newPosition);
    
    // Check for escape condition
    if (newState.r > 1000.0 * blackHole.GetSchwarzschildRadius()) {
        lightRay.SetStatus(LightRay::Status::ESCAPED);
    }
}

GeodesicState PhysicsEngine::CalculateGeodesicDerivatives(const GeodesicState& state, 
                                                         const BlackHole& blackHole, 
                                                         bool isMassive) const {
    GeodesicState derivatives;
    
    const double rs = blackHole.GetSchwarzschildRadius();
    const double r = state.r;
    const double theta = state.theta;
    const double sin_theta = std::sin(theta);
    const double cos_theta = std::cos(theta);
    
    // Avoid singularities
    if (r <= rs || sin_theta == 0.0) {
        return derivatives; // Return zero derivatives
    }
    
    const double factor = 1.0 - rs / r;
    const double inv_factor = 1.0 / factor;
    
    // Coordinate derivatives (dx^μ/dτ)
    derivatives.t = state.dt_dtau;
    derivatives.r = state.dr_dtau;
    derivatives.theta = state.dtheta_dtau;
    derivatives.phi = state.dphi_dtau;
    
    // 4-velocity derivatives (d²x^μ/dτ²) from geodesic equation
    // d²t/dτ² = -Γ^t_μν (dx^μ/dτ)(dx^ν/dτ)
    derivatives.dt_dtau = -(rs / (r * r * factor)) * state.dt_dtau * state.dr_dtau;
    
    // d²r/dτ² = -Γ^r_μν (dx^μ/dτ)(dx^ν/dτ)
    double dr_dt2 = (rs * factor / (2.0 * r * r * r)) * state.dt_dtau * state.dt_dtau;
    double dr_dr2 = -(rs / (2.0 * r * factor * (r - rs))) * state.dr_dtau * state.dr_dtau;
    double dr_dtheta2 = -(r - rs) * state.dtheta_dtau * state.dtheta_dtau;
    double dr_dphi2 = -(r - rs) * sin_theta * sin_theta * state.dphi_dtau * state.dphi_dtau;
    
    derivatives.dr_dtau = dr_dt2 + dr_dr2 + dr_dtheta2 + dr_dphi2;
    
    // d²θ/dτ² = -Γ^θ_μν (dx^μ/dτ)(dx^ν/dτ)
    double dtheta_dr2 = (2.0 / r) * state.dr_dtau * state.dtheta_dtau;
    double dtheta_dphi2 = -sin_theta * cos_theta * state.dphi_dtau * state.dphi_dtau;
    
    derivatives.dtheta_dtau = dtheta_dr2 + dtheta_dphi2;
    
    // d²φ/dτ² = -Γ^φ_μν (dx^μ/dτ)(dx^ν/dτ)
    double dphi_dr2 = (2.0 / r) * state.dr_dtau * state.dphi_dtau;
    double dphi_dtheta2 = (2.0 * cos_theta / sin_theta) * state.dtheta_dtau * state.dphi_dtau;
    
    derivatives.dphi_dtau = dphi_dr2 + dphi_dtheta2;
    
    return derivatives;
}

bool PhysicsEngine::IntegrateEuler(GeodesicState& state, const BlackHole& blackHole, 
                                  double timeStep, bool isMassive) const {
    GeodesicState derivatives = CalculateGeodesicDerivatives(state, blackHole, isMassive);
    
    // Simple Euler integration: y_{n+1} = y_n + h * f(y_n)
    state.t += timeStep * derivatives.t;
    state.r += timeStep * derivatives.r;
    state.theta += timeStep * derivatives.theta;
    state.phi += timeStep * derivatives.phi;
    state.dt_dtau += timeStep * derivatives.dt_dtau;
    state.dr_dtau += timeStep * derivatives.dr_dtau;
    state.dtheta_dtau += timeStep * derivatives.dtheta_dtau;
    state.dphi_dtau += timeStep * derivatives.dphi_dtau;
    
    return ValidateGeodesicState(state, blackHole, isMassive);
}

bool PhysicsEngine::IntegrateRK4(GeodesicState& state, const BlackHole& blackHole, 
                                double timeStep, bool isMassive) const {
    // Runge-Kutta 4th order integration
    GeodesicState k1, k2, k3, k4;
    GeodesicState temp_state;
    
    // k1 = f(y_n)
    k1 = CalculateGeodesicDerivatives(state, blackHole, isMassive);
    
    // k2 = f(y_n + h/2 * k1)
    temp_state = AddGeodesicStates(state, ScaleGeodesicState(k1, timeStep / 2.0));
    k2 = CalculateGeodesicDerivatives(temp_state, blackHole, isMassive);
    
    // k3 = f(y_n + h/2 * k2)
    temp_state = AddGeodesicStates(state, ScaleGeodesicState(k2, timeStep / 2.0));
    k3 = CalculateGeodesicDerivatives(temp_state, blackHole, isMassive);
    
    // k4 = f(y_n + h * k3)
    temp_state = AddGeodesicStates(state, ScaleGeodesicState(k3, timeStep));
    k4 = CalculateGeodesicDerivatives(temp_state, blackHole, isMassive);
    
    // y_{n+1} = y_n + h/6 * (k1 + 2*k2 + 2*k3 + k4)
    GeodesicState weighted_sum = AddGeodesicStates(
        AddGeodesicStates(k1, ScaleGeodesicState(k2, 2.0)),
        AddGeodesicStates(ScaleGeodesicState(k3, 2.0), k4)
    );
    
    state = AddGeodesicStates(state, ScaleGeodesicState(weighted_sum, timeStep / 6.0));
    
    return ValidateGeodesicState(state, blackHole, isMassive);
}

bool PhysicsEngine::IntegrateAdaptiveRK45(GeodesicState& state, const BlackHole& blackHole, 
                                         double timeStep, bool isMassive) const {
    // Simplified adaptive RK45 - would need full Dormand-Prince coefficients for production
    double h = timeStep;
    double h_min = m_minStepSize;
    double h_max = m_maxStepSize;
    
    int iterations = 0;
    while (iterations < m_maxIterations) {
        // Try integration with current step size
        GeodesicState state_rk4 = state;
        GeodesicState state_rk5 = state;
        
        bool success_rk4 = IntegrateRK4(state_rk4, blackHole, h, isMassive);
        if (!success_rk4) {
            h *= 0.5;
            if (h < h_min) return false;
            iterations++;
            continue;
        }
        
        // Estimate error (simplified - would use proper RK5 in production)
        double error = EstimateIntegrationError(state, state_rk4, blackHole, isMassive);
        
        if (error < m_errorTolerance) {
            // Accept step
            state = state_rk4;
            return true;
        } else {
            // Reduce step size
            h *= 0.8 * std::pow(m_errorTolerance / error, 0.2);
            h = std::max(h, h_min);
        }
        
        iterations++;
    }
    
    return false; // Failed to converge
}

double PhysicsEngine::EstimateIntegrationError(const GeodesicState& state, 
                                              const GeodesicState& integrated_state,
                                              const BlackHole& blackHole, 
                                              bool isMassive) const {
    // Simple error estimate based on conservation violations
    double initial_energy = CalculateEnergy(state, blackHole);
    double final_energy = CalculateEnergy(integrated_state, blackHole);
    
    double initial_L = CalculateAngularMomentum(state);
    double final_L = CalculateAngularMomentum(integrated_state);
    
    double energy_error = std::abs((final_energy - initial_energy) / initial_energy);
    double L_error = std::abs((final_L - initial_L) / initial_L);
    
    return std::max(energy_error, L_error);
}

bool PhysicsEngine::ValidateGeodesicState(const GeodesicState& state, 
                                         const BlackHole& blackHole, 
                                         bool isMassive) const {
    // Check for NaN or infinite values
    if (!std::isfinite(state.r) || !std::isfinite(state.theta) || !std::isfinite(state.phi) ||
        !std::isfinite(state.dt_dtau) || !std::isfinite(state.dr_dtau) || 
        !std::isfinite(state.dtheta_dtau) || !std::isfinite(state.dphi_dtau)) {
        return false;
    }
    
    // Check physical bounds
    if (state.r <= 0.0 || state.theta < 0.0 || state.theta > M_PI) {
        return false;
    }
    
    // Check if inside event horizon
    if (state.r <= blackHole.GetSchwarzschildRadius()) {
        return false;
    }
    
    return true;
}

void PhysicsEngine::NormalizeMassiveParticle4Velocity(GeodesicState& state, 
                                                     const BlackHole& blackHole) const {
    // For massive particles: g_μν u^μ u^ν = -c²
    const double rs = blackHole.GetSchwarzschildRadius();
    const double r = state.r;
    const double theta = state.theta;
    const double sin_theta = std::sin(theta);
    
    if (r <= rs) return;
    
    const double factor = 1.0 - rs / r;
    
    // Calculate spatial part of 4-velocity norm
    double spatial_norm = (1.0 / factor) * state.dr_dtau * state.dr_dtau +
                         r * r * state.dtheta_dtau * state.dtheta_dtau +
                         r * r * sin_theta * sin_theta * state.dphi_dtau * state.dphi_dtau;
    
    // Solve for dt/dτ from normalization condition
    state.dt_dtau = std::sqrt((c2 + spatial_norm) / factor);
}

void PhysicsEngine::NormalizeNullGeodesic4Velocity(GeodesicState& state, 
                                                  const BlackHole& blackHole) const {
    // For photons: g_μν u^μ u^ν = 0
    const double rs = blackHole.GetSchwarzschildRadius();
    const double r = state.r;
    const double theta = state.theta;
    const double sin_theta = std::sin(theta);
    
    if (r <= rs) return;
    
    const double factor = 1.0 - rs / r;
    
    // Calculate spatial part of 4-velocity norm
    double spatial_norm = (1.0 / factor) * state.dr_dtau * state.dr_dtau +
                         r * r * state.dtheta_dtau * state.dtheta_dtau +
                         r * r * sin_theta * sin_theta * state.dphi_dtau * state.dphi_dtau;
    
    // Solve for dt/dτ from null condition
    state.dt_dtau = std::sqrt(spatial_norm / factor);
}

double PhysicsEngine::CalculateEnergy(const GeodesicState& state, const BlackHole& blackHole) const {
    const double rs = blackHole.GetSchwarzschildRadius();
    const double r = state.r;
    
    if (r <= rs) return 0.0;
    
    // Energy per unit mass: E = (1 - rs/r) * dt/dτ
    return (1.0 - rs / r) * state.dt_dtau;
}

double PhysicsEngine::CalculatePhotonEnergy(const GeodesicState& state, const BlackHole& blackHole) const {
    // For photons, energy is proportional to frequency and redshift factor
    const double rs = blackHole.GetSchwarzschildRadius();
    const double r = state.r;
    
    if (r <= rs) return 0.0;
    
    return (1.0 - rs / r) * state.dt_dtau;
}

double PhysicsEngine::CalculateAngularMomentum(const GeodesicState& state) const {
    // Angular momentum per unit mass: L = r² sin²θ * dφ/dτ
    const double r = state.r;
    const double theta = state.theta;
    const double sin_theta = std::sin(theta);
    
    return r * r * sin_theta * sin_theta * state.dphi_dtau;
}

PhysicsEngine::SphericalCoords PhysicsEngine::CartesianToSpherical(const Vector3& cartesian) const {
    SphericalCoords spherical;
    
    spherical.r = cartesian.Magnitude();
    
    if (spherical.r > 0.0) {
        spherical.theta = std::acos(std::clamp(cartesian.z / spherical.r, -1.0, 1.0));
        spherical.phi = std::atan2(cartesian.y, cartesian.x);
    } else {
        spherical.theta = 0.0;
        spherical.phi = 0.0;
    }
    
    return spherical;
}

Vector3 PhysicsEngine::SphericalToCartesian(const SphericalCoords& spherical) const {
    const double sin_theta = std::sin(spherical.theta);
    const double cos_theta = std::cos(spherical.theta);
    const double sin_phi = std::sin(spherical.phi);
    const double cos_phi = std::cos(spherical.phi);
    
    return Vector3(
        spherical.r * sin_theta * cos_phi,
        spherical.r * sin_theta * sin_phi,
        spherical.r * cos_theta
    );
}

PhysicsEngine::SphericalVelocity PhysicsEngine::CartesianToSphericalVelocity(
    const Vector3& cartesian_vel, const SphericalCoords& position) const {
    
    const double r = position.r;
    const double theta = position.theta;
    const double phi = position.phi;
    
    if (r == 0.0) {
        return {0.0, 0.0, 0.0};
    }
    
    const double sin_theta = std::sin(theta);
    const double cos_theta = std::cos(theta);
    const double sin_phi = std::sin(phi);
    const double cos_phi = std::cos(phi);
    
    SphericalVelocity spherical_vel;
    
    // Transform velocity components
    spherical_vel.dr_dt = sin_theta * cos_phi * cartesian_vel.x +
                         sin_theta * sin_phi * cartesian_vel.y +
                         cos_theta * cartesian_vel.z;
    
    spherical_vel.dtheta_dt = (cos_theta * cos_phi * cartesian_vel.x +
                              cos_theta * sin_phi * cartesian_vel.y -
                              sin_theta * cartesian_vel.z) / r;
    
    if (sin_theta != 0.0) {
        spherical_vel.dphi_dt = (-sin_phi * cartesian_vel.x + cos_phi * cartesian_vel.y) / (r * sin_theta);
    } else {
        spherical_vel.dphi_dt = 0.0;
    }
    
    return spherical_vel;
}

Vector3 PhysicsEngine::SphericalToCartesianVelocity(
    const SphericalVelocity& spherical_vel, const SphericalCoords& position) const {
    
    const double r = position.r;
    const double theta = position.theta;
    const double phi = position.phi;
    
    const double sin_theta = std::sin(theta);
    const double cos_theta = std::cos(theta);
    const double sin_phi = std::sin(phi);
    const double cos_phi = std::cos(phi);
    
    Vector3 cartesian_vel;
    
    cartesian_vel.x = sin_theta * cos_phi * spherical_vel.dr_dt +
                     r * cos_theta * cos_phi * spherical_vel.dtheta_dt -
                     r * sin_theta * sin_phi * spherical_vel.dphi_dt;
    
    cartesian_vel.y = sin_theta * sin_phi * spherical_vel.dr_dt +
                     r * cos_theta * sin_phi * spherical_vel.dtheta_dt +
                     r * sin_theta * cos_phi * spherical_vel.dphi_dt;
    
    cartesian_vel.z = cos_theta * spherical_vel.dr_dt -
                     r * sin_theta * spherical_vel.dtheta_dt;
    
    return cartesian_vel;
}

PhysicsEngine::GeodesicState PhysicsEngine::AddGeodesicStates(const GeodesicState& a, const GeodesicState& b) const {
    GeodesicState result;
    result.t = a.t + b.t;
    result.r = a.r + b.r;
    result.theta = a.theta + b.theta;
    result.phi = a.phi + b.phi;
    result.dt_dtau = a.dt_dtau + b.dt_dtau;
    result.dr_dtau = a.dr_dtau + b.dr_dtau;
    result.dtheta_dtau = a.dtheta_dtau + b.dtheta_dtau;
    result.dphi_dtau = a.dphi_dtau + b.dphi_dtau;
    return result;
}

PhysicsEngine::GeodesicState PhysicsEngine::ScaleGeodesicState(const GeodesicState& state, double scale) const {
    GeodesicState result;
    result.t = state.t * scale;
    result.r = state.r * scale;
    result.theta = state.theta * scale;
    result.phi = state.phi * scale;
    result.dt_dtau = state.dt_dtau * scale;
    result.dr_dtau = state.dr_dtau * scale;
    result.dtheta_dtau = state.dtheta_dtau * scale;
    result.dphi_dtau = state.dphi_dtau * scale;
    return result;
}

} // namespace BlackHoleSim