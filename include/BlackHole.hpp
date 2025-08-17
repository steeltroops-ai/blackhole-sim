#pragma once

#include <array>
#include <cmath>

namespace BlackHoleSim {

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
     * @brief Get black hole mass in geometric units (GM/c²)
     * @return Mass in geometric units (meters)
     */
    double GetGeometricMass() const { return m_geometricMass; }

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
     * @param r Radial distance from black hole center
     * @return True if inside event horizon, false otherwise
     */
    bool IsInsideEventHorizon(double r) const {
        return r < m_schwarzschildRadius;
    }

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
     * @brief Calculate tidal acceleration at given position
     * @param r Radial distance from black hole center
     * @param height Height above/below radial line
     * @return Tidal acceleration in m/s²
     */
    double GetTidalAcceleration(double r, double height) const;

    /**
     * @brief Calculate orbital velocity for circular orbit at given radius
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

private:
    double m_mass;                    ///< Mass in solar masses
    double m_geometricMass;           ///< Mass in geometric units (GM/c²)
    double m_schwarzschildRadius;     ///< Schwarzschild radius (2GM/c²)
    std::array<double, 3> m_position; ///< Position in 3D space

    // Physical constants
    static constexpr double SOLAR_MASS = 1.98847e30;     ///< Solar mass in kg
    static constexpr double G = 6.67430e-11;             ///< Gravitational constant
    static constexpr double C = 299792458.0;             ///< Speed of light in m/s
    static constexpr double C_SQUARED = C * C;           ///< Speed of light squared

    /**
     * @brief Initialize derived quantities from mass
     */
    void InitializeDerivedQuantities();
};

} // namespace BlackHoleSim