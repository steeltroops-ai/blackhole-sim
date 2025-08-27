#pragma once

#include <array>
#include <cmath>
#include <limits>
#include <utility>
#include "Vector3.hpp"

namespace BlackHoleSim {

/**
 * @brief Metric component identifiers for Schwarzschild metric
 */
enum class MetricComponent {
    G_TT,           ///< Time-time component g_tt
    G_RR,           ///< Radial-radial component g_rr
    G_THETA_THETA,  ///< Theta-theta component g_θθ
    G_PHI_PHI       ///< Phi-phi component g_φφ
};

/**
 * @brief Represents a Schwarzschild (non-rotating) black hole
 * 
 * Implements the Schwarzschild metric and provides methods for calculating
 * spacetime curvature, event horizon properties, and gravitational effects.
 */
class BlackHole {
public:
    /**
     * @brief Construct a black hole with given mass
     * @param mass Black hole mass in solar masses
     * @param position Position in 3D space (x, y, z)
     */
    BlackHole(double mass, const std::array<double, 3>& position = {0.0, 0.0, 0.0});

    /**
     * @brief Get black hole mass in solar masses
     * @return Mass in solar masses
     */
    double GetMass() const { return m_mass; }

    /**
     * @brief Set black hole mass
     * @param mass New mass in solar masses
     */
    void SetMass(double mass);

    /**
     * @brief Destructor
     */
    ~BlackHole();

    /**
     * @brief Get black hole mass in geometric units (GM/c²)
     * @return Mass in geometric units (meters)
     */
    double GetGeometricMass() const { return G * m_mass / C_SQUARED; }

    /**
     * @brief Get Schwarzschild radius (event horizon)
     * @return Schwarzschild radius in meters
     */
    double GetSchwarzschildRadius() const { return m_schwarzschildRadius; }

    /**
     * @brief Get black hole position
     * @return Position array [x, y, z]
     */
    const std::array<double, 3>& GetPosition() const { return m_position; }

    /**
     * @brief Set black hole position
     * @param position New position [x, y, z]
     */
    void SetPosition(const std::array<double, 3>& position) { m_position = position; }

    /**
     * @brief Calculate Schwarzschild metric components at given position
     * @param r Radial distance from black hole center
     * @param theta Polar angle
     * @param phi Azimuthal angle
     * @return Metric tensor components [g_tt, g_rr, g_theta_theta, g_phi_phi]
     */
    std::array<double, 4> GetMetricComponents(double r, double theta, double phi) const;

    /**
     * @brief Get specific metric component
     * @param component Which metric component to calculate
     * @param r Radial distance
     * @param theta Polar angle
     * @return Metric component value
     */
    double GetMetricComponent(MetricComponent component, double r, double theta) const;

    /**
     * @brief Calculate gravitational potential at given distance
     * @param r Radial distance from black hole center
     * @return Gravitational potential (dimensionless)
     */
    double GetGravitationalPotential(double r) const;

    /**
     * @brief Calculate time dilation factor at given distance
     * @param r Radial distance from black hole center
     * @return Time dilation factor (proper time / coordinate time)
     */
    double GetTimeDilationFactor(double r) const;

    /**
     * @brief Calculate gravitational redshift factor
     * @param r Radial distance from black hole center
     * @return Redshift factor (observed frequency / emitted frequency)
     */
    double GetRedshiftFactor(double r) const;

    /**
     * @brief Calculate escape velocity at given distance
     * @param r Radial distance from black hole center
     * @return Escape velocity in m/s
     */
    double GetEscapeVelocity(double r) const;

    /**
     * @brief Check if position is inside event horizon
     * @param position Position vector
     * @return True if inside event horizon, false otherwise
     */
    bool IsInsideEventHorizon(const Vector3& position) const;
    
    /**
     * @brief Check if position is inside photon sphere
     * @param position Position vector
     * @return True if inside photon sphere, false otherwise
     */
    bool IsInsidePhotonSphere(const Vector3& position) const;
    
    /**
     * @brief Check if position is inside ISCO
     * @param position Position vector
     * @return True if inside ISCO, false otherwise
     */
    bool IsInsideISCO(const Vector3& position) const;

    /**
     * @brief Get photon sphere radius (unstable circular orbit for light)
     * @return Photon sphere radius in meters
     */
    double GetPhotonSphereRadius() const {
        return 1.5 * m_schwarzschildRadius;
    }

    /**
     * @brief Get innermost stable circular orbit (ISCO) radius
     * @return ISCO radius in meters
     */
    double GetISCORadius() const {
        return 3.0 * m_schwarzschildRadius;
    }

    /**
     * @brief Calculate tidal acceleration
     * @param position Position vector
     * @param separation Separation vector
     * @return Tidal acceleration vector
     */
    Vector3 GetTidalAcceleration(const Vector3& position, const Vector3& separation) const;

    /**
     * @brief Calculate circular orbit velocity at given radius
     * @param r Orbital radius
     * @return Orbital velocity in m/s
     */
    double GetCircularOrbitVelocity(double r) const;

    /**
     * @brief Calculate orbital period for circular orbit at given radius
     * @param r Orbital radius
     * @return Orbital period in seconds
     */
    double GetOrbitalPeriod(double r) const;
    
    /**
     * @brief Calculate effective potential
     * @param r Radial distance
     * @param L Angular momentum
     * @return Effective potential
     */
    double GetEffectivePotential(double r, double L) const;
    
    /**
     * @brief Calculate photon effective potential
     * @param r Radial distance
     * @param b Impact parameter
     * @return Photon effective potential
     */
    double GetPhotonEffectivePotential(double r, double b) const;
    
    /**
     * @brief Get Christoffel symbol
     * @param mu First index
     * @param nu Second index
     * @param lambda Third index
     * @param r Radial coordinate
     * @param theta Polar coordinate
     * @return Christoffel symbol components
     */
    std::array<double, 4> GetChristoffelSymbol(int mu, int nu, int lambda, double r, double theta) const;
    
    /**
     * @brief Get gravitational field
     * @param position Position vector
     * @return Gravitational field vector
     */
    Vector3 GetGravitationalField(const Vector3& position) const;
    
    /**
     * @brief Get surface gravity
     * @return Surface gravity
     */
    double GetSurfaceGravity() const;
    
    /**
     * @brief Get Hawking temperature
     * @return Hawking temperature in K
     */
    double GetHawkingTemperature() const;
    
    /**
     * @brief Get Bekenstein-Hawking entropy
     * @return Entropy
     */
    double GetBekensteinHawkingEntropy() const;
    
    /**
     * @brief Get luminosity
     * @return Luminosity in W
     */
    double GetLuminosity() const;
    
    /**
     * @brief Check if orbit is stable
     * @param r Radial distance
     * @param L Angular momentum
     * @return True if stable, false otherwise
     */
    bool IsStableOrbit(double r, double L) const;
    
    /**
     * @brief Get turning points
     * @param E Energy
     * @param L Angular momentum
     * @return Pair of turning points
     */
    std::pair<double, double> GetTurningPoints(double E, double L) const;

private:
    double m_mass;                    ///< Mass in kg
    std::array<double, 3> m_position; ///< Position in 3D space
    double m_schwarzschildRadius;     ///< Schwarzschild radius (2GM/c²)
    double m_photonSphereRadius;      ///< Photon sphere radius
    double m_iscoRadius;              ///< ISCO radius

    // Physical constants
    static constexpr double G = 6.67430e-11;     ///< Gravitational constant
    static constexpr double C = 299792458.0;     ///< Speed of light in m/s
    static constexpr double C_SQUARED = C * C;   ///< Speed of light squared
    static constexpr double HBAR = 1.054571817e-34; ///< Reduced Planck constant
    static constexpr double K_B = 1.380649e-23;  ///< Boltzmann constant
    static constexpr double SIGMA_SB = 5.670374419e-8; ///< Stefan-Boltzmann constant

    /**
     * @brief Update derived quantities from mass
     */
    void UpdateDerivedQuantities();

    /**
     * @brief Calculate distance from center to position
     * @param position Position vector
     * @return Distance from black hole center
     */
    double GetDistanceFromCenter(const Vector3& position) const;
};

} // namespace BlackHoleSim