#pragma once

#include <array>
#include <vector>
#include <complex>
#include "Vector3.hpp"

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

    // Static factory methods
    /**
     * @brief Create red light ray
     * @param position Starting position
     * @param direction Direction vector
     * @return Red light ray
     */
    static LightRay CreateRedLight(const Vector3& position, const Vector3& direction);

    /**
     * @brief Create green light ray
     * @param position Starting position
     * @param direction Direction vector
     * @return Green light ray
     */
    static LightRay CreateGreenLight(const Vector3& position, const Vector3& direction);

    /**
     * @brief Create blue light ray
     * @param position Starting position
     * @param direction Direction vector
     * @return Blue light ray
     */
    static LightRay CreateBlueLight(const Vector3& position, const Vector3& direction);

    /**
     * @brief Create white light ray
     * @param position Starting position
     * @param direction Direction vector
     * @return White light ray
     */
    static LightRay CreateWhiteLight(const Vector3& position, const Vector3& direction);

    /**
     * @brief Create spectrum of light rays
     * @param position Starting position
     * @param direction Direction vector
     * @param minWavelength_nm Minimum wavelength in nm
     * @param maxWavelength_nm Maximum wavelength in nm
     * @param numRays Number of rays to create
     * @return Vector of light rays
     */
    static std::vector<LightRay> CreateSpectrum(const Vector3& position, const Vector3& direction,
                                               double minWavelength_nm, double maxWavelength_nm,
                                               int numRays);

    /**
     * @brief Create light ray from wavelength
     * @param position Starting position
     * @param direction Direction vector
     * @param wavelength_nm Wavelength in nanometers
     * @param type Ray type
     * @return Light ray with specified wavelength
     */
    static LightRay CreateFromWavelength(const Vector3& position, const Vector3& direction,
                                        double wavelength_nm, Type type = Type::DIRECT);

    /**
     * @brief Get current position
     * @return Position array [x, y, z] in meters
     */
    const std::array<double, 3>& GetPosition() const { return m_position; }

    /**
     * @brief Get initial position
     * @return Initial position as Vector3
     */
    Vector3 GetInitialPosition() const;

    /**
     * @brief Get initial direction
     * @return Initial direction as Vector3
     */
    Vector3 GetInitialDirection() const;

    /**
     * @brief Check if ray has traveled minimum distance
     * @param minDistance Minimum distance to check
     * @return True if traveled distance exceeds minimum
     */
    bool HasTraveledDistance(double minDistance) const;

    /**
     * @brief Get total distance traveled by the ray
     * @return Total distance in meters
     */
    double GetDistanceTraveled() const;

    /**
     * @brief Reset ray with new parameters
     * @param position New starting position
     * @param direction New direction
     * @param frequency New frequency
     */
    void Reset(const Vector3& position, const Vector3& direction, double frequency);

    /**
     * @brief Apply redshift to the ray
     * @param redshiftFactor Redshift factor to apply
     */
    void ApplyRedshift(double redshiftFactor);

    /**
     * @brief Apply blueshift to the ray
     * @param blueshiftFactor Blueshift factor to apply
     */
    void ApplyBlueshift(double blueshiftFactor);

    /**
     * @brief Get type as string
     * @return String representation of ray type
     */
    std::string GetTypeString() const;

    /**
     * @brief Get status as string
     * @return String representation of ray status
     */
    std::string GetStatusString() const;

    /**
     * @brief Get spectral region
     * @return String representation of spectral region
     */
    std::string GetSpectralRegion() const;

    /**
     * @brief Reserve capacity for path storage
     * @param capacity Number of path points to reserve
     */
    void ReservePathCapacity(size_t capacity);

    /**
     * @brief Add position to ray path
     * @param position Position to add to path
     */
    void AddToPath(const Vector3& position);

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
     * @param frequency Frequency in Hz
     */
    void SetFrequency(double frequency) { m_frequency = frequency; }

    /**
     * @brief Set wavelength
     * @param wavelength Wavelength in meters
     */
    void SetWavelength(double wavelength);

    /**
     * @brief Set wavelength in nanometers
     * @param wavelength_nm Wavelength in nanometers
     */
    void SetWavelengthNm(double wavelength_nm);

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
     * @brief Get current status
     * @return Current status
     */
    Status GetStatus() const { return m_status; }

    /**
     * @brief Check if ray is active
     * @return True if ray is active
     */
    bool IsActive() const;

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
     * @param e Energy (dimensionless)
     */
    void SetEnergy(double e) { m_energy = e; }

    /**
     * @brief Get conserved energy for geodesic motion
     * @param center Center of mass position
     * @param centralMass Central mass in kg
     * @return Conserved energy
     */
    double GetConservedEnergy(const Vector3& center, double centralMass) const;

    /**
     * @brief Get conserved angular momentum for geodesic motion
     * @param center Center of mass position
     * @return Conserved angular momentum
     */
    double GetConservedAngularMomentum(const Vector3& center) const;

    /**
     * @brief Calculate redshift between two frequencies
     * @param initialFrequency Initial frequency
     * @param finalFrequency Final frequency
     * @return Redshift value
     */
    double CalculateRedshift(double initialFrequency, double finalFrequency) const;

    /**
     * @brief Calculate redshift from initial frequency
     * @param initialFrequency Initial frequency
     * @return Redshift value
     */
    double CalculateRedshift(double initialFrequency) const;

    /**
     * @brief Get impact parameter relative to center
     * @param center Center position
     * @return Impact parameter
     */
    double GetImpactParameter(const Vector3& center) const;

    /**
     * @brief Get photon energy
     * @return Photon energy
     */
    double GetPhotonEnergy() const;

    /**
     * @brief Get photon momentum
     * @return Photon momentum
     */
    double GetPhotonMomentum() const;

    /**
     * @brief Get photon momentum vector
     * @return Photon momentum vector
     */
    Vector3 GetPhotonMomentumVector() const;

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
     * @brief Get wavelength in meters
     * @return Wavelength in meters
     */
    double GetWavelength() const;

    /**
     * @brief Get wavelength in nanometers
     * @return Wavelength in nanometers
     */
    double GetWavelengthNm() const;

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