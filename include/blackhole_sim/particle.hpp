#pragma once

#include <array>
#include <vector>
#include <string>
#include <cmath>
#include "vector3.hpp"
#include "vector4.hpp"

namespace BlackHoleSim {

/**
 * @brief Spherical coordinate representation
 */
struct SphericalCoords {
    double r;     ///< Radial distance
    double theta; ///< Polar angle
    double phi;   ///< Azimuthal angle
};

/**
 * @brief Represents a particle in the black hole simulation
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
        ASTEROID,       ///< Asteroid object
        COMET,          ///< Comet object
        CUSTOM          ///< User-defined type
    };

    /**
     * @brief Default constructor
     */
    Particle();

    /**
     * @brief Construct a particle with given properties
     * @param mass Particle mass in kg
     * @param position Initial position [x, y, z] in meters
     * @param velocity Initial velocity [vx, vy, vz] in m/s
     * @param type Particle type
     */
    Particle(double mass, 
             const Vector3& position,
             const Vector3& velocity,
             Type type = Type::TEST_PARTICLE);

    /**
     * @brief Destructor
     */
    ~Particle();

    /**
     * @brief Get particle mass
     * @return Mass in kg
     */
    double GetMass() const;

    /**
     * @brief Set particle mass
     * @param mass New mass in kg
     */
    void SetMass(double mass);

    /**
     * @brief Get current position
     * @return Position array [x, y, z] in meters
     */
    Vector3 GetPosition() const;

    /**
     * @brief Set position
     * @param position New position [x, y, z] in meters
     */
    void SetPosition(const Vector3& position);

    /**
     * @brief Get current velocity
     * @return Velocity array [vx, vy, vz] in m/s
     */
    Vector3 GetVelocity() const;

    /**
     * @brief Set velocity
     * @param velocity New velocity [vx, vy, vz] in m/s
     */
    void SetVelocity(const Vector3& velocity);

    /**
     * @brief Get particle type
     * @return Particle type
     */
    Type GetType() const;

    /**
     * @brief Set particle type
     * @param type New particle type
     */
    void SetType(Type type);

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
     * @brief Get radial distance from center point
     * @param center Center point to measure distance from
     * @return Distance from center in meters
     */
    double GetRadialDistance(const Vector3& center = Vector3(0.0, 0.0, 0.0)) const;

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
     * @param center Center point for angular momentum calculation
     * @return Angular momentum magnitude in kg⋅m²/s
     */
    double GetAngularMomentum(const Vector3& center = Vector3(0.0, 0.0, 0.0)) const;

    /**
     * @brief Get angular momentum vector
     * @param center Center point for angular momentum calculation
     * @return Angular momentum vector [Lx, Ly, Lz] in kg⋅m²/s
     */
    Vector3 GetAngularMomentumVector(const Vector3& center = Vector3(0.0, 0.0, 0.0)) const;

    /**
     * @brief Check if particle is active (not fallen into black hole)
     * @return True if active, false if inactive
     */
    bool IsActive() const;

    /**
     * @brief Set particle active state
     * @param active New active state
     */
    void SetActive(bool active);

    /**
     * @brief Get trajectory history
     * @return Vector of historical positions
     */
    const std::vector<Vector3>& GetTrajectory() const;

    /**
     * @brief Add current position to trajectory history
     */
    void RecordTrajectoryPoint();

    /**
     * @brief Clear trajectory history
     */
    void ClearTrajectory();

    /**
     * @brief Add position to trajectory
     * @param position Position to add
     */
    void AddToTrajectory(const Vector3& position);

    /**
     * @brief Reserve trajectory capacity
     * @param capacity Number of points to reserve
     */
    void ReserveTrajectoryCapacity(size_t capacity);

    /**
     * @brief Get maximum trajectory points
     * @return Maximum number of trajectory points
     */
    size_t GetMaxTrajectoryPoints() const;

    /**
     * @brief Set maximum trajectory length
     * @param maxLength Maximum number of points to store
     */
    void SetMaxTrajectoryLength(size_t maxLength) { m_maxTrajectoryLength = maxLength; }

    /**
     * @brief Set maximum trajectory points
     * @param maxPoints Maximum number of points to store
     */
    void SetMaxTrajectoryPoints(size_t maxPoints);

    /**
     * @brief Reset particle to new position and velocity
     * @param position New position
     * @param velocity New velocity
     */
    void Reset(const Vector3& position, const Vector3& velocity);

    /**
     * @brief Apply force to particle for given time
     * @param force Force vector to apply
     * @param deltaTime Time step
     */
    void ApplyForce(const Vector3& force, double deltaTime);

    /**
     * @brief Apply instantaneous impulse to particle
     * @param impulse Impulse vector to apply
     */
    void ApplyImpulse(const Vector3& impulse);

    /**
     * @brief Get orbital energy relative to center
     * @param center Center point
     * @param centralMass Mass of central object
     * @return Orbital energy in Joules
     */
    double GetOrbitalEnergy(const Vector3& center, double centralMass) const;

    /**
     * @brief Get escape velocity from center
     * @param center Center point
     * @param centralMass Mass of central object
     * @return Escape velocity in m/s
     */
    double GetEscapeVelocity(const Vector3& center, double centralMass) const;

    /**
     * @brief Check if particle is gravitationally bound
     * @param center Center point
     * @param centralMass Mass of central object
     * @return True if bound
     */
    bool IsGravitationallyBound(const Vector3& center, double centralMass) const;

    /**
     * @brief Get orbital period
     * @param center Center point
     * @param centralMass Mass of central object
     * @return Orbital period in seconds
     */
    double GetOrbitalPeriod(const Vector3& center, double centralMass) const;

    /**
     * @brief Get circular orbit velocity
     * @param center Center point
     * @param centralMass Mass of central object
     * @return Circular orbit velocity in m/s
     */
    double GetCircularOrbitVelocity(const Vector3& center, double centralMass) const;

    /**
     * @brief Get particle type as string
     * @return Type name as string
     */
    std::string GetTypeString() const;

    /**
     * @brief Get particle color for rendering
     * @return Color vector [r, g, b, a] (0-1 range)
     */
    Vector4 GetColor() const;

    /**
     * @brief Set particle color for rendering
     * @param color Color vector [r, g, b, a] (0-1 range)
     */
    void SetColor(const Vector4& color);

    /**
     * @brief Get particle size for rendering
     * @return Size in pixels
     */
    double GetSize() const;

    /**
     * @brief Set particle size
     * @param size Size in pixels
     */
    void SetSize(double size);

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
     * @return Spherical coordinates
     */
    static SphericalCoords CartesianToSpherical(const Vector3& cartesian);

    /**
     * @brief Convert spherical coordinates to Cartesian
     * @param spherical Spherical coordinates
     * @return Cartesian coordinates [x, y, z]
     */
    static Vector3 SphericalToCartesian(const SphericalCoords& spherical);

    /**
     * @brief Convert spherical coordinates to Cartesian
     * @param r Radial distance
     * @param theta Polar angle
     * @param phi Azimuthal angle
     * @return Cartesian coordinates [x, y, z]
     */
    static Vector3 SphericalToCartesian(double r, double theta, double phi);

private:
    // Physical properties
    double m_mass;                              ///< Mass in kg
    Vector3 m_position;                         ///< Position [x, y, z] in meters
    Vector3 m_velocity;                         ///< Velocity [vx, vy, vz] in m/s
    Type m_type;                                ///< Particle type
    bool m_isActive;                            ///< Whether particle is active

    // Trajectory tracking
    std::vector<Vector3> m_trajectory;          ///< Historical positions
    size_t m_maxTrajectoryLength;               ///< Maximum trajectory points to store
    size_t m_maxTrajectoryPoints;               ///< Maximum trajectory points

    // Rendering properties
    Vector4 m_color;                            ///< Color [r, g, b, a]
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