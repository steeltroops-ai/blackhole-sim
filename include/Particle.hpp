#pragma once

#include <array>
#include <vector>
#include <string>

namespace BlackHoleSim {

/**
 * @brief Represents a massive particle or star in the simulation
 * 
 * Stores position, velocity, and physical properties of massive objects
 * that follow timelike geodesics in the black hole's spacetime.
 */
class Particle {
public:
    /**
     * @brief Particle type enumeration
     */
    enum class Type {
        TEST_PARTICLE,   ///< Massless test particle
        STAR,           ///< Star with significant mass
        PLANET,         ///< Planetary object
        DEBRIS,         ///< Small debris or dust
        CUSTOM          ///< User-defined type
    };

    /**
     * @brief Construct a particle with given properties
     * @param mass Particle mass in kg
     * @param position Initial position [x, y, z] in meters
     * @param velocity Initial velocity [vx, vy, vz] in m/s
     * @param type Particle type
     */
    Particle(double mass, 
             const std::array<double, 3>& position,
             const std::array<double, 3>& velocity,
             Type type = Type::TEST_PARTICLE);

    /**
     * @brief Get particle mass
     * @return Mass in kg
     */
    double GetMass() const { return m_mass; }

    /**
     * @brief Set particle mass
     * @param mass New mass in kg
     */
    void SetMass(double mass) { m_mass = mass; }

    /**
     * @brief Get current position
     * @return Position array [x, y, z] in meters
     */
    const std::array<double, 3>& GetPosition() const { return m_position; }

    /**
     * @brief Set position
     * @param position New position [x, y, z] in meters
     */
    void SetPosition(const std::array<double, 3>& position) { m_position = position; }

    /**
     * @brief Get current velocity
     * @return Velocity array [vx, vy, vz] in m/s
     */
    const std::array<double, 3>& GetVelocity() const { return m_velocity; }

    /**
     * @brief Set velocity
     * @param velocity New velocity [vx, vy, vz] in m/s
     */
    void SetVelocity(const std::array<double, 3>& velocity) { m_velocity = velocity; }

    /**
     * @brief Get particle type
     * @return Particle type
     */
    Type GetType() const { return m_type; }

    /**
     * @brief Set particle type
     * @param type New particle type
     */
    void SetType(Type type) { m_type = type; }

    /**
     * @brief Get geodesic state for physics integration
     * @return State array [t, r, theta, phi, dt/dtau, dr/dtau, dtheta/dtau, dphi/dtau]
     */
    std::array<double, 8> GetGeodesicState() const;

    /**
     * @brief Set geodesic state from physics integration
     * @param state State array [t, r, theta, phi, dt/dtau, dr/dtau, dtheta/dtau, dphi/dtau]
     */
    void SetGeodesicState(const std::array<double, 8>& state);

    /**
     * @brief Get radial distance from origin
     * @return Radial distance in meters
     */
    double GetRadialDistance() const;

    /**
     * @brief Get speed (magnitude of velocity)
     * @return Speed in m/s
     */
    double GetSpeed() const;

    /**
     * @brief Get kinetic energy
     * @return Kinetic energy in Joules
     */
    double GetKineticEnergy() const;

    /**
     * @brief Get angular momentum magnitude
     * @return Angular momentum in kg⋅m²/s
     */
    double GetAngularMomentum() const;

    /**
     * @brief Get angular momentum vector
     * @return Angular momentum vector [Lx, Ly, Lz] in kg⋅m²/s
     */
    std::array<double, 3> GetAngularMomentumVector() const;

    /**
     * @brief Check if particle is active (not fallen into black hole)
     * @return True if active, false if inactive
     */
    bool IsActive() const { return m_active; }

    /**
     * @brief Set particle active state
     * @param active New active state
     */
    void SetActive(bool active) { m_active = active; }

    /**
     * @brief Get trajectory history
     * @return Vector of historical positions
     */
    const std::vector<std::array<double, 3>>& GetTrajectory() const { return m_trajectory; }

    /**
     * @brief Add current position to trajectory history
     */
    void RecordTrajectoryPoint();

    /**
     * @brief Clear trajectory history
     */
    void ClearTrajectory() { m_trajectory.clear(); }

    /**
     * @brief Set maximum trajectory length
     * @param maxLength Maximum number of trajectory points to store
     */
    void SetMaxTrajectoryLength(size_t maxLength) { m_maxTrajectoryLength = maxLength; }

    /**
     * @brief Get particle color for rendering
     * @return Color array [r, g, b, a] (0-1 range)
     */
    const std::array<float, 4>& GetColor() const { return m_color; }

    /**
     * @brief Set particle color for rendering
     * @param color Color array [r, g, b, a] (0-1 range)
     */
    void SetColor(const std::array<float, 4>& color) { m_color = color; }

    /**
     * @brief Get particle size for rendering
     * @return Size in pixels
     */
    float GetSize() const { return m_size; }

    /**
     * @brief Set particle size for rendering
     * @param size Size in pixels
     */
    void SetSize(float size) { m_size = size; }

    /**
     * @brief Get particle name/label
     * @return Particle name
     */
    const std::string& GetName() const { return m_name; }

    /**
     * @brief Set particle name/label
     * @param name New particle name
     */
    void SetName(const std::string& name) { m_name = name; }

    /**
     * @brief Convert Cartesian coordinates to spherical
     * @param cartesian Cartesian coordinates [x, y, z]
     * @return Spherical coordinates [r, theta, phi]
     */
    static std::array<double, 3> CartesianToSpherical(const std::array<double, 3>& cartesian);

    /**
     * @brief Convert spherical coordinates to Cartesian
     * @param spherical Spherical coordinates [r, theta, phi]
     * @return Cartesian coordinates [x, y, z]
     */
    static std::array<double, 3> SphericalToCartesian(const std::array<double, 3>& spherical);

private:
    // Physical properties
    double m_mass;                              ///< Mass in kg
    std::array<double, 3> m_position;           ///< Position [x, y, z] in meters
    std::array<double, 3> m_velocity;           ///< Velocity [vx, vy, vz] in m/s
    Type m_type;                                ///< Particle type
    bool m_active;                              ///< Whether particle is active

    // Trajectory tracking
    std::vector<std::array<double, 3>> m_trajectory;  ///< Historical positions
    size_t m_maxTrajectoryLength;               ///< Maximum trajectory points to store

    // Rendering properties
    std::array<float, 4> m_color;               ///< Color [r, g, b, a]
    float m_size;                               ///< Rendering size
    std::string m_name;                         ///< Particle name/label

    // Physical constants
    static constexpr double C = 299792458.0;    ///< Speed of light in m/s

    /**
     * @brief Initialize default properties based on type
     */
    void InitializeDefaultProperties();
};

} // namespace BlackHoleSim