#pragma once

#include <array>
#include <vector>
#include <complex>

namespace BlackHoleSim {

/**
 * @brief Represents a light ray (photon) following null geodesics
 * 
 * Handles light ray propagation through curved spacetime, including
 * gravitational lensing, redshift calculations, and ray-tracing.
 */
class LightRay {
public:
    /**
     * @brief Light ray type enumeration
     */
    enum class Type {
        DIRECT,          ///< Direct ray from source to observer
        LENSED,          ///< Gravitationally lensed ray
        SCATTERED,       ///< Scattered by accretion disk
        BACKGROUND,      ///< Background starlight
        CUSTOM           ///< User-defined type
    };

    /**
     * @brief Ray status enumeration
     */
    enum class Status {
        ACTIVE,          ///< Ray is actively propagating
        ESCAPED,         ///< Ray has escaped to infinity
        ABSORBED,        ///< Ray fell into black hole
        TERMINATED       ///< Ray was manually terminated
    };

    /**
     * @brief Construct a light ray with given properties
     * @param position Initial position [x, y, z] in meters
     * @param direction Initial direction [dx, dy, dz] (normalized)
     * @param frequency Initial frequency in Hz
     * @param intensity Initial intensity (arbitrary units)
     * @param type Ray type
     */
    LightRay(const std::array<double, 3>& position,
             const std::array<double, 3>& direction,
             double frequency,
             double intensity = 1.0,
             Type type = Type::DIRECT);

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
     * @brief Get current direction
     * @return Direction array [dx, dy, dz] (normalized)
     */
    const std::array<double, 3>& GetDirection() const { return m_direction; }

    /**
     * @brief Set direction
     * @param direction New direction [dx, dy, dz] (will be normalized)
     */
    void SetDirection(const std::array<double, 3>& direction);

    /**
     * @brief Get current frequency
     * @return Frequency in Hz
     */
    double GetFrequency() const { return m_frequency; }

    /**
     * @brief Set frequency
     * @param frequency New frequency in Hz
     */
    void SetFrequency(double frequency) { m_frequency = frequency; }

    /**
     * @brief Get initial frequency (at emission)
     * @return Initial frequency in Hz
     */
    double GetInitialFrequency() const { return m_initialFrequency; }

    /**
     * @brief Get current intensity
     * @return Intensity (arbitrary units)
     */
    double GetIntensity() const { return m_intensity; }

    /**
     * @brief Set intensity
     * @param intensity New intensity
     */
    void SetIntensity(double intensity) { m_intensity = intensity; }

    /**
     * @brief Get ray type
     * @return Ray type
     */
    Type GetType() const { return m_type; }

    /**
     * @brief Set ray type
     * @param type New ray type
     */
    void SetType(Type type) { m_type = type; }

    /**
     * @brief Get ray status
     * @return Current status
     */
    Status GetStatus() const { return m_status; }

    /**
     * @brief Set ray status
     * @param status New status
     */
    void SetStatus(Status status) { m_status = status; }

    /**
     * @brief Get geodesic state for physics integration
     * @return State array [t, r, theta, phi, dt/dlambda, dr/dlambda, dtheta/dlambda, dphi/dlambda]
     */
    std::array<double, 8> GetGeodesicState() const;

    /**
     * @brief Set geodesic state from physics integration
     * @param state State array [t, r, theta, phi, dt/dlambda, dr/dlambda, dtheta/dlambda, dphi/dlambda]
     */
    void SetGeodesicState(const std::array<double, 8>& state);

    /**
     * @brief Get radial distance from origin
     * @return Radial distance in meters
     */
    double GetRadialDistance() const;

    /**
     * @brief Get impact parameter (conserved quantity)
     * @return Impact parameter in meters
     */
    double GetImpactParameter() const { return m_impactParameter; }

    /**
     * @brief Set impact parameter
     * @param b Impact parameter in meters
     */
    void SetImpactParameter(double b) { m_impactParameter = b; }

    /**
     * @brief Get energy (conserved quantity for null geodesics)
     * @return Energy (dimensionless)
     */
    double GetEnergy() const { return m_energy; }

    /**
     * @brief Set energy
     * @param energy Energy (dimensionless)
     */
    void SetEnergy(double energy) { m_energy = energy; }

    /**
     * @brief Calculate current redshift factor
     * @return Redshift z = (observed - emitted) / emitted
     */
    double GetRedshift() const;

    /**
     * @brief Calculate gravitational redshift component
     * @return Gravitational redshift factor
     */
    double GetGravitationalRedshift() const;

    /**
     * @brief Calculate Doppler redshift component
     * @return Doppler redshift factor
     */
    double GetDopplerRedshift() const;

    /**
     * @brief Get ray path history
     * @return Vector of historical positions
     */
    const std::vector<std::array<double, 3>>& GetPath() const { return m_path; }

    /**
     * @brief Add current position to path history
     */
    void RecordPathPoint();

    /**
     * @brief Clear path history
     */
    void ClearPath() { m_path.clear(); }

    /**
     * @brief Set maximum path length
     * @param maxLength Maximum number of path points to store
     */
    void SetMaxPathLength(size_t maxLength) { m_maxPathLength = maxLength; }

    /**
     * @brief Get ray color for rendering (based on frequency)
     * @return Color array [r, g, b, a] (0-1 range)
     */
    std::array<float, 4> GetColor() const;

    /**
     * @brief Get ray wavelength
     * @return Wavelength in meters
     */
    double GetWavelength() const;

    /**
     * @brief Check if ray is in visible spectrum
     * @return True if visible (380-750 nm)
     */
    bool IsVisible() const;

    /**
     * @brief Get affine parameter (integration parameter)
     * @return Current affine parameter value
     */
    double GetAffineParameter() const { return m_affineParameter; }

    /**
     * @brief Set affine parameter
     * @param lambda New affine parameter value
     */
    void SetAffineParameter(double lambda) { m_affineParameter = lambda; }

    /**
     * @brief Calculate deflection angle from initial direction
     * @return Deflection angle in radians
     */
    double GetDeflectionAngle() const;

    /**
     * @brief Calculate time delay due to gravitational lensing
     * @return Time delay in seconds
     */
    double GetTimeDelay() const;

    /**
     * @brief Check if ray will be captured by black hole
     * @param schwarzschildRadius Black hole event horizon radius
     * @return True if ray will be captured
     */
    bool WillBeCaptured(double schwarzschildRadius) const;

    /**
     * @brief Calculate minimum approach distance
     * @return Minimum radial distance in meters
     */
    double GetMinimumApproachDistance() const;

    /**
     * @brief Convert frequency to RGB color
     * @param frequency Frequency in Hz
     * @return RGB color [r, g, b] (0-1 range)
     */
    static std::array<float, 3> FrequencyToRGB(double frequency);

    /**
     * @brief Convert wavelength to RGB color
     * @param wavelength Wavelength in meters
     * @return RGB color [r, g, b] (0-1 range)
     */
    static std::array<float, 3> WavelengthToRGB(double wavelength);

private:
    // Physical properties
    std::array<double, 3> m_position;           ///< Current position [x, y, z]
    std::array<double, 3> m_direction;          ///< Current direction [dx, dy, dz]
    std::array<double, 3> m_initialDirection;   ///< Initial direction for deflection calculation
    double m_frequency;                         ///< Current frequency in Hz
    double m_initialFrequency;                  ///< Initial frequency in Hz
    double m_intensity;                         ///< Current intensity
    double m_initialIntensity;                  ///< Initial intensity

    // Geodesic properties
    double m_impactParameter;                   ///< Impact parameter (conserved)
    double m_energy;                            ///< Energy (conserved)
    double m_affineParameter;                   ///< Affine parameter (integration variable)

    // Ray properties
    Type m_type;                                ///< Ray type
    Status m_status;                            ///< Current status

    // Path tracking
    std::vector<std::array<double, 3>> m_path;  ///< Historical positions
    size_t m_maxPathLength;                     ///< Maximum path points to store

    // Physical constants
    static constexpr double C = 299792458.0;    ///< Speed of light in m/s
    static constexpr double H = 6.62607015e-34; ///< Planck constant

    // Visible light spectrum bounds (wavelength in meters)
    static constexpr double VISIBLE_MIN_WAVELENGTH = 380e-9;  ///< Violet
    static constexpr double VISIBLE_MAX_WAVELENGTH = 750e-9;  ///< Red

    /**
     * @brief Normalize direction vector
     */
    void NormalizeDirection();

    /**
     * @brief Calculate conserved quantities from current state
     */
    void UpdateConservedQuantities();
};

} // namespace BlackHoleSim