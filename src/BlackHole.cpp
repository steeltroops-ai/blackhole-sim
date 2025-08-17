/**
 * @file BlackHole.cpp
 * @brief Implementation of the BlackHole class
 */

#include "BlackHole.hpp"
#include "Vector3.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <utility>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace BlackHoleSim {

// Physical constants
const double G = 6.67430e-11;  // Gravitational constant (m³/kg⋅s²)
const double c = 299792458.0;  // Speed of light (m/s)
const double c2 = c * c;       // Speed of light squared
const double c4 = c2 * c2;     // Speed of light to the fourth power

BlackHole::BlackHole(double mass, const std::array<double, 3>& position)
    : m_mass(mass)
    , m_position(position)
    , m_schwarzschildRadius(0.0)
    , m_photonSphereRadius(0.0)
    , m_iscoRadius(0.0)
{
    if (mass <= 0.0) {
        throw std::invalid_argument("Black hole mass must be positive");
    }
    
    // Calculate derived quantities
    UpdateDerivedQuantities();
}

BlackHole::~BlackHole() = default;

void BlackHole::SetMass(double mass) {
    if (mass <= 0.0) {
        throw std::invalid_argument("Black hole mass must be positive");
    }
    
    m_mass = mass;
    UpdateDerivedQuantities();
}



void BlackHole::UpdateDerivedQuantities() {
    // Schwarzschild radius: rs = 2GM/c²
    m_schwarzschildRadius = 2.0 * G * m_mass / c2;
    
    // Photon sphere radius: r_ph = 3GM/c² = 1.5 * rs
    m_photonSphereRadius = 1.5 * m_schwarzschildRadius;
    
    // Innermost Stable Circular Orbit (ISCO) radius: r_isco = 6GM/c² = 3 * rs
    m_iscoRadius = 3.0 * m_schwarzschildRadius;
}

double BlackHole::GetMetricComponent(MetricComponent component, double r, double theta) const {
    if (r <= 0.0) {
        throw std::invalid_argument("Radius must be positive");
    }
    
    const double rs = m_schwarzschildRadius;
    
    switch (component) {
        case MetricComponent::G_TT:
            // g_tt = -(1 - rs/r)
            return -(1.0 - rs / r);
            
        case MetricComponent::G_RR:
            // g_rr = 1/(1 - rs/r)
            if (r <= rs) {
                // Inside event horizon, metric becomes singular
                return std::numeric_limits<double>::infinity();
            }
            return 1.0 / (1.0 - rs / r);
            
        case MetricComponent::G_THETA_THETA:
            // g_θθ = r²
            return r * r;
            
        case MetricComponent::G_PHI_PHI:
            // g_φφ = r² sin²θ
            return r * r * std::sin(theta) * std::sin(theta);
            
        default:
            return 0.0;
    }
}

double BlackHole::GetGravitationalPotential(double r) const {
    if (r <= 0.0) {
        return -std::numeric_limits<double>::infinity();
    }
    
    // Newtonian potential: Φ = -GM/r
    return -G * m_mass / r;
}

double BlackHole::GetTimeDilationFactor(double r) const {
    if (r <= m_schwarzschildRadius) {
        return std::numeric_limits<double>::infinity();
    }
    
    // Time dilation factor: √(1 - rs/r)
    return std::sqrt(1.0 - m_schwarzschildRadius / r);
}

double BlackHole::GetRedshiftFactor(double r) const {
    if (r <= m_schwarzschildRadius) {
        return std::numeric_limits<double>::infinity();
    }
    
    // Redshift factor for observer at infinity: √(1 - rs/r)
    double factor = 1.0 - m_schwarzschildRadius / r;
    
    return std::sqrt(factor);
}

double BlackHole::GetEscapeVelocity(double r) const {
    if (r <= m_schwarzschildRadius) {
        return c; // Speed of light at event horizon
    }
    
    // Classical escape velocity: v_esc = √(2GM/r)
    double v_esc_classical = std::sqrt(2.0 * G * m_mass / r);
    
    // Relativistic correction (approximate)
    double gamma = 1.0 / std::sqrt(1.0 - m_schwarzschildRadius / r);
    
    return std::min(v_esc_classical * gamma, c);
}

bool BlackHole::IsInsideEventHorizon(const Vector3& position) const {
    double r = (position - m_position).Magnitude();
    return r <= m_schwarzschildRadius;
}

bool BlackHole::IsInsidePhotonSphere(const Vector3& position) const {
    double r = (position - m_position).Magnitude();
    return r <= m_photonSphereRadius;
}

bool BlackHole::IsInsideISCO(const Vector3& position) const {
    double r = (position - m_position).Magnitude();
    return r <= m_iscoRadius;
}

Vector3 BlackHole::GetTidalAcceleration(const Vector3& position, const Vector3& separation) const {
    // Calculate tidal acceleration due to differential gravitational field
    Vector3 bh_position(m_position);
    Vector3 r_vec = position - bh_position;
    double r = r_vec.Magnitude();
    
    if (r <= m_schwarzschildRadius || r == 0.0) {
        return Vector3(0.0, 0.0, 0.0);
    }
    
    Vector3 r_hat = r_vec / r;
    
    // Tidal acceleration: a_tidal = -GM/r³ * [3(ξ⋅r̂)r̂ - ξ]
    // where ξ is the separation vector
    double dot_product = separation.Dot(r_hat);
    Vector3 radial_component = 3.0 * dot_product * r_hat;
    Vector3 tidal_accel = -G * m_mass / (r * r * r) * (radial_component - separation);
    
    return tidal_accel;
}

double BlackHole::GetCircularOrbitVelocity(double r) const {
    if (r <= m_iscoRadius) {
        // No stable circular orbits inside ISCO
        return 0.0;
    }
    
    // Circular orbital velocity: v = √(GM/r) * √(1 - 3rs/(2r))
    // The second factor is a relativistic correction
    double v_newtonian = std::sqrt(G * m_mass / r);
    double relativistic_factor = std::sqrt(1.0 - 1.5 * m_schwarzschildRadius / r);
    
    return v_newtonian * relativistic_factor;
}

double BlackHole::GetOrbitalPeriod(double r) const {
    if (r <= m_iscoRadius) {
        return std::numeric_limits<double>::infinity();
    }
    
    // Kepler's third law with relativistic corrections
    // T = 2π√(r³/GM) * (1 + 3rs/(4r) + ...)
    double period_newtonian = 2.0 * M_PI * std::sqrt(r * r * r / (G * m_mass));
    double relativistic_correction = 1.0 + 0.75 * m_schwarzschildRadius / r;
    
    return period_newtonian * relativistic_correction;
}

double BlackHole::GetEffectivePotential(double r, double L) const {
    if (r <= 0.0) {
        return -std::numeric_limits<double>::infinity();
    }
    
    // Effective potential for massive particles:
    // V_eff = -GM/r + L²/(2r²) - GML²/(c²r³)
    // where L is the angular momentum per unit mass
    
    double term1 = -G * m_mass / r;                    // Gravitational potential
    double term2 = L * L / (2.0 * r * r);              // Centrifugal barrier
    double term3 = -G * m_mass * L * L / (c2 * r * r * r); // Relativistic correction
    
    return term1 + term2 + term3;
}

double BlackHole::GetPhotonEffectivePotential(double r, double b) const {
    if (r <= 0.0) {
        return -std::numeric_limits<double>::infinity();
    }
    
    // Effective potential for photons:
    // V_eff = (1 - rs/r) * b²/r²
    // where b is the impact parameter
    
    double factor = 1.0 - m_schwarzschildRadius / r;
    return factor * b * b / (r * r);
}

std::array<double, 4> BlackHole::GetChristoffelSymbol(int mu, int nu, int lambda, double r, double theta) const {
    // Calculate Christoffel symbols for Schwarzschild metric
    // This is a simplified implementation - full calculation would be more extensive
    
    const double rs = m_schwarzschildRadius;
    std::array<double, 4> result = {0.0, 0.0, 0.0, 0.0};
    
    if (r <= rs) {
        return result; // Singular at event horizon
    }
    
    // Some key non-zero Christoffel symbols for Schwarzschild metric:
    
    if (mu == 0 && nu == 1 && lambda == 0) {
        // Γ^t_tr = rs/(2r(r-rs))
        result[0] = rs / (2.0 * r * (r - rs));
    }
    else if (mu == 1 && nu == 0 && lambda == 0) {
        // Γ^r_tt = rs(r-rs)/(2r³)
        result[1] = rs * (r - rs) / (2.0 * r * r * r);
    }
    else if (mu == 1 && nu == 1 && lambda == 1) {
        // Γ^r_rr = -rs/(2r(r-rs))
        result[1] = -rs / (2.0 * r * (r - rs));
    }
    else if (mu == 1 && nu == 2 && lambda == 2) {
        // Γ^r_θθ = -(r-rs)
        result[1] = -(r - rs);
    }
    else if (mu == 1 && nu == 3 && lambda == 3) {
        // Γ^r_φφ = -(r-rs)sin²θ
        result[1] = -(r - rs) * std::sin(theta) * std::sin(theta);
    }
    else if (mu == 2 && nu == 1 && lambda == 2) {
        // Γ^θ_rθ = 1/r
        result[2] = 1.0 / r;
    }
    else if (mu == 2 && nu == 3 && lambda == 3) {
        // Γ^θ_φφ = -sinθ cosθ
        result[2] = -std::sin(theta) * std::cos(theta);
    }
    else if (mu == 3 && nu == 1 && lambda == 3) {
        // Γ^φ_rφ = 1/r
        result[3] = 1.0 / r;
    }
    else if (mu == 3 && nu == 2 && lambda == 3) {
        // Γ^φ_θφ = cotθ
        if (std::sin(theta) != 0.0) {
            result[3] = std::cos(theta) / std::sin(theta);
        }
    }
    
    return result;
}

Vector3 BlackHole::GetGravitationalField(const Vector3& position) const {
    Vector3 bh_position(m_position);
    Vector3 r_vec = position - bh_position;
    double r = r_vec.Magnitude();
    
    if (r == 0.0) {
        return Vector3(0.0, 0.0, 0.0);
    }
    
    // Newtonian gravitational field: g = -GM/r² * r̂
    Vector3 r_hat = r_vec / r;
    double field_magnitude = G * m_mass / (r * r);
    
    return -field_magnitude * r_hat;
}

double BlackHole::GetSurfaceGravity() const {
    // Surface gravity at event horizon: κ = c⁴/(4GM) = c²/(2rs)
    return c2 / (2.0 * m_schwarzschildRadius);
}

double BlackHole::GetHawkingTemperature() const {
    // Hawking temperature: T = ℏc³/(8πGMk_B)
    // Using ℏ = 1.054571817e-34 J⋅s, k_B = 1.380649e-23 J/K
    const double hbar = 1.054571817e-34;
    const double k_B = 1.380649e-23;
    
    return hbar * c * c * c / (8.0 * M_PI * G * m_mass * k_B);
}

double BlackHole::GetBekensteinHawkingEntropy() const {
    // Bekenstein-Hawking entropy: S = A/(4ℓ_P²) = πrs²c³/(2Gℏ)
    // where A = 4πrs² is the area of the event horizon
    const double hbar = 1.054571817e-34;
    const double area = 4.0 * M_PI * m_schwarzschildRadius * m_schwarzschildRadius;
    
    return area * c * c * c / (4.0 * G * hbar);
}

double BlackHole::GetLuminosity() const {
    // Hawking radiation luminosity: L = ℏc⁶/(15360πG²M²)
    const double hbar = 1.054571817e-34;
    const double c6 = c2 * c2 * c2;
    
    return hbar * c6 / (15360.0 * M_PI * G * G * m_mass * m_mass);
}

bool BlackHole::IsStableOrbit(double r, double L) const {
    if (r <= m_iscoRadius) {
        return false; // No stable orbits inside ISCO
    }
    
    // Check stability by examining second derivative of effective potential
    const double dr = 1e-6; // Small increment for numerical derivative
    
    double V_minus = GetEffectivePotential(r - dr, L);
    double V_center = GetEffectivePotential(r, L);
    double V_plus = GetEffectivePotential(r + dr, L);
    
    // Second derivative
    double d2V_dr2 = (V_plus - 2.0 * V_center + V_minus) / (dr * dr);
    
    return d2V_dr2 > 0.0; // Stable if second derivative is positive
}

std::pair<double, double> BlackHole::GetTurningPoints(double E, double L) const {
    // Find turning points where E = V_eff(r)
    // This is a simplified implementation - would need numerical root finding for accuracy
    
    double r_min = m_schwarzschildRadius * 1.01; // Start just outside event horizon
    double r_max = 1000.0 * m_schwarzschildRadius; // Large outer radius
    
    double r_inner = r_min;
    double r_outer = r_max;
    
    // Simple search for turning points
    const int num_points = 1000;
    double dr = (r_max - r_min) / num_points;
    
    for (int i = 0; i < num_points; ++i) {
        double r = r_min + i * dr;
        double V_eff = GetEffectivePotential(r, L);
        
        if (std::abs(V_eff - E) < 1e-10) {
            if (r < (r_min + r_max) / 2.0) {
                r_inner = r;
            } else {
                r_outer = r;
            }
        }
    }
    
    return std::make_pair(r_inner, r_outer);
}

} // namespace BlackHoleSim