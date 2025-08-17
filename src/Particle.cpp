/**
 * @file Particle.cpp
 * @brief Implementation of the Particle class
 */

#include "Particle.hpp"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace BlackHoleSim {

// Physical constants
const double G = 6.67430e-11;  // Gravitational constant (m³/kg⋅s²)
const double c = 299792458.0;  // Speed of light (m/s)

Particle::Particle()
    : m_mass(1.0)
    , m_position(0.0, 0.0, 0.0)
    , m_velocity(0.0, 0.0, 0.0)
    , m_type(Type::TEST_PARTICLE)
    , m_isActive(true)
    , m_color(1.0, 1.0, 1.0, 1.0)
    , m_size(1.0)
    , m_maxTrajectoryPoints(1000)
{
    m_trajectory.reserve(m_maxTrajectoryPoints);
}

Particle::Particle(double mass, const Vector3& position, const Vector3& velocity, Type type)
    : m_mass(mass)
    , m_position(position)
    , m_velocity(velocity)
    , m_type(type)
    , m_isActive(true)
    , m_color(1.0, 1.0, 1.0, 1.0)
    , m_size(1.0)
    , m_maxTrajectoryPoints(1000)
{
    m_trajectory.reserve(m_maxTrajectoryPoints);
    
    // Set default colors based on particle type
    switch (type) {
        case Type::TEST_PARTICLE:
            m_color = Vector4(1.0, 1.0, 1.0, 1.0); // White
            m_size = 1.0;
            break;
        case Type::STAR:
            m_color = Vector4(1.0, 1.0, 0.0, 1.0); // Yellow
            m_size = 2.0;
            break;
        case Type::PLANET:
            m_color = Vector4(0.0, 0.5, 1.0, 1.0); // Blue
            m_size = 1.5;
            break;
        case Type::ASTEROID:
            m_color = Vector4(0.7, 0.7, 0.7, 1.0); // Gray
            m_size = 0.5;
            break;
        case Type::COMET:
            m_color = Vector4(0.8, 0.9, 1.0, 1.0); // Light blue
            m_size = 0.8;
            break;
    }
    
    // Add initial position to trajectory
    AddToTrajectory(m_position);
}

Particle::~Particle() {
    // Destructor - nothing special needed
}

// Getters
double Particle::GetMass() const {
    return m_mass;
}

Vector3 Particle::GetPosition() const {
    return m_position;
}

Vector3 Particle::GetVelocity() const {
    return m_velocity;
}

Particle::Type Particle::GetType() const {
    return m_type;
}

bool Particle::IsActive() const {
    return m_isActive;
}

Vector4 Particle::GetColor() const {
    return m_color;
}

double Particle::GetSize() const {
    return m_size;
}

const std::vector<Vector3>& Particle::GetTrajectory() const {
    return m_trajectory;
}

size_t Particle::GetMaxTrajectoryPoints() const {
    return m_maxTrajectoryPoints;
}

// Setters
void Particle::SetMass(double mass) {
    m_mass = std::max(0.0, mass); // Ensure non-negative mass
}

void Particle::SetPosition(const Vector3& position) {
    m_position = position;
}

void Particle::SetVelocity(const Vector3& velocity) {
    m_velocity = velocity;
}

void Particle::SetType(Type type) {
    m_type = type;
    
    // Update default rendering properties based on new type
    switch (type) {
        case Type::TEST_PARTICLE:
            if (m_color.x() == 1.0 && m_color.y() == 1.0 && m_color.z() == 0.0) { // Only if still default yellow
                m_color = Vector4(1.0, 1.0, 1.0, 1.0); // White
            }
            break;
        case Type::STAR:
            m_color = Vector4(1.0, 1.0, 0.0, 1.0); // Yellow
            m_size = 2.0;
            break;
        case Type::PLANET:
            m_color = Vector4(0.0, 0.5, 1.0, 1.0); // Blue
            m_size = 1.5;
            break;
        case Type::ASTEROID:
            m_color = Vector4(0.7, 0.7, 0.7, 1.0); // Gray
            m_size = 0.5;
            break;
        case Type::COMET:
            m_color = Vector4(0.8, 0.9, 1.0, 1.0); // Light blue
            m_size = 0.8;
            break;
    }
}

void Particle::SetActive(bool active) {
    m_isActive = active;
}

void Particle::SetColor(const Vector4& color) {
    m_color = color;
}

void Particle::SetSize(double size) {
    m_size = std::max(0.1, size); // Ensure minimum size for visibility
}

void Particle::SetMaxTrajectoryPoints(size_t maxPoints) {
    m_maxTrajectoryPoints = maxPoints;
    
    // Resize trajectory if necessary
    if (m_trajectory.size() > maxPoints) {
        // Keep the most recent points
        m_trajectory.erase(m_trajectory.begin(), 
                          m_trajectory.begin() + (m_trajectory.size() - maxPoints));
    }
    
    m_trajectory.reserve(maxPoints);
}

// Physics calculations
double Particle::GetRadialDistance(const Vector3& center) const {
    return (m_position - center).Magnitude();
}

double Particle::GetSpeed() const {
    return m_velocity.Magnitude();
}

double Particle::GetKineticEnergy() const {
    double speed = GetSpeed();
    return 0.5 * m_mass * speed * speed;
}

double Particle::GetAngularMomentum(const Vector3& center) const {
    Vector3 r = m_position - center;
    Vector3 p = m_velocity * m_mass; // momentum
    Vector3 L = r.Cross(p); // angular momentum vector
    return L.Magnitude();
}

Vector3 Particle::GetAngularMomentumVector(const Vector3& center) const {
    Vector3 r = m_position - center;
    Vector3 p = m_velocity * m_mass; // momentum
    return r.Cross(p); // angular momentum vector
}

double Particle::GetOrbitalEnergy(const Vector3& center, double centralMass) const {
    double kineticEnergy = GetKineticEnergy();
    double r = GetRadialDistance(center);
    
    if (r > 0.0) {
        double potentialEnergy = -G * centralMass * m_mass / r;
        return kineticEnergy + potentialEnergy;
    }
    
    return kineticEnergy;
}

double Particle::GetEscapeVelocity(const Vector3& center, double centralMass) const {
    double r = GetRadialDistance(center);
    
    if (r > 0.0) {
        return std::sqrt(2.0 * G * centralMass / r);
    }
    
    return 0.0;
}

bool Particle::IsGravitationallyBound(const Vector3& center, double centralMass) const {
    double totalEnergy = GetOrbitalEnergy(center, centralMass);
    return totalEnergy < 0.0;
}

double Particle::GetOrbitalPeriod(const Vector3& center, double centralMass) const {
    double r = GetRadialDistance(center);
    
    if (r > 0.0 && IsGravitationallyBound(center, centralMass)) {
        // For circular orbits: T = 2π√(r³/GM)
        // This is an approximation - actual orbits may be elliptical
        return 2.0 * M_PI * std::sqrt(r * r * r / (G * centralMass));
    }
    
    return 0.0; // Unbound or invalid
}

double Particle::GetCircularOrbitVelocity(const Vector3& center, double centralMass) const {
    double r = GetRadialDistance(center);
    
    if (r > 0.0) {
        return std::sqrt(G * centralMass / r);
    }
    
    return 0.0;
}

// Trajectory management
void Particle::AddToTrajectory(const Vector3& position) {
    m_trajectory.push_back(position);
    
    // Remove oldest points if we exceed the maximum
    if (m_trajectory.size() > m_maxTrajectoryPoints) {
        m_trajectory.erase(m_trajectory.begin());
    }
}

void Particle::ClearTrajectory() {
    m_trajectory.clear();
    // Add current position as the starting point
    m_trajectory.push_back(m_position);
}

void Particle::ReserveTrajectoryCapacity(size_t capacity) {
    m_trajectory.reserve(capacity);
}

// Coordinate conversion utilities (static methods)
SphericalCoords Particle::CartesianToSpherical(const Vector3& cartesian) {
    SphericalCoords spherical;
    
    spherical.r = cartesian.Magnitude();
    
    if (spherical.r > 1e-10) {
        double cosTheta = cartesian.z() / spherical.r;
        // Manual clamp for C++11 compatibility
        cosTheta = (cosTheta < -1.0) ? -1.0 : (cosTheta > 1.0) ? 1.0 : cosTheta;
        spherical.theta = std::acos(cosTheta);
        spherical.phi = std::atan2(cartesian.y(), cartesian.x());
    } else {
        spherical.theta = 0.0;
        spherical.phi = 0.0;
    }
    
    // Ensure phi is in [0, 2π)
    if (spherical.phi < 0.0) {
        spherical.phi += 2.0 * M_PI;
    }
    
    return spherical;
}

Vector3 Particle::SphericalToCartesian(const SphericalCoords& spherical) {
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

Vector3 Particle::SphericalToCartesian(double r, double theta, double phi) {
    const double sin_theta = std::sin(theta);
    const double cos_theta = std::cos(theta);
    const double sin_phi = std::sin(phi);
    const double cos_phi = std::cos(phi);
    
    return Vector3(
        r * sin_theta * cos_phi,
        r * sin_theta * sin_phi,
        r * cos_theta
    );
}

// Utility methods
std::string Particle::GetTypeString() const {
    switch (m_type) {
        case Type::TEST_PARTICLE: return "Test Particle";
        case Type::STAR: return "Star";
        case Type::PLANET: return "Planet";
        case Type::ASTEROID: return "Asteroid";
        case Type::COMET: return "Comet";
        default: return "Unknown";
    }
}

void Particle::Reset(const Vector3& position, const Vector3& velocity) {
    m_position = position;
    m_velocity = velocity;
    m_isActive = true;
    ClearTrajectory();
}

void Particle::ApplyForce(const Vector3& force, double deltaTime) {
    if (m_mass > 0.0 && m_isActive) {
        Vector3 acceleration = force / m_mass;
        m_velocity = m_velocity + acceleration * deltaTime;
    }
}

void Particle::ApplyImpulse(const Vector3& impulse) {
    if (m_mass > 0.0 && m_isActive) {
        Vector3 deltaVelocity = impulse / m_mass;
        m_velocity = m_velocity + deltaVelocity;
    }
}

// Utility methods removed - not declared in header
// GetDistanceTo, GetDirectionTo, IsColliding, GetLorentzFactor methods removed

} // namespace BlackHoleSim