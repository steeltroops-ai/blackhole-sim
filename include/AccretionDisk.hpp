#pragma once

#include "BlackHole.hpp"
#include <array>
#include <vector>
#include <memory>
#include <functional>

namespace BlackHoleSim {

/**
 * @brief Represents an accretion disk around a black hole
 * 
 * Implements the Shakura-Sunyaev model for thin accretion disks,
 * including temperature profiles, emission spectra, and disk dynamics.
 */
class AccretionDisk {
public:
    /**
     * @brief Disk model type
     */
    enum class ModelType {
        SHAKURA_SUNYAEV,    ///< Standard thin disk model
        NOVIKOV_THORNE,     ///< Relativistic thin disk
        THICK_DISK,         ///< Thick disk (torus)
        CUSTOM              ///< User-defined model
    };

    /**
     * @brief Construct accretion disk with given parameters
     * @param blackHole Pointer to central black hole
     * @param innerRadius Inner disk radius (typically ISCO)
     * @param outerRadius Outer disk radius
     * @param accretionRate Mass accretion rate in kg/s
     * @param alpha Viscosity parameter (Shakura-Sunyaev)
     */
    AccretionDisk(std::shared_ptr<BlackHole> blackHole,
                  double innerRadius,
                  double outerRadius,
                  double accretionRate,
                  double alpha = 0.1);

    /**
     * @brief Get inner disk radius
     * @return Inner radius in meters
     */
    double GetInnerRadius() const { return m_innerRadius; }

    /**
     * @brief Set inner disk radius
     * @param radius New inner radius in meters
     */
    void SetInnerRadius(double radius);

    /**
     * @brief Get outer disk radius
     * @return Outer radius in meters
     */
    double GetOuterRadius() const { return m_outerRadius; }

    /**
     * @brief Set outer disk radius
     * @param radius New outer radius in meters
     */
    void SetOuterRadius(double radius) { m_outerRadius = radius; }

    /**
     * @brief Get mass accretion rate
     * @return Accretion rate in kg/s
     */
    double GetAccretionRate() const { return m_accretionRate; }

    /**
     * @brief Set mass accretion rate
     * @param rate New accretion rate in kg/s
     */
    void SetAccretionRate(double rate);

    /**
     * @brief Get viscosity parameter (alpha)
     * @return Alpha parameter (dimensionless)
     */
    double GetAlpha() const { return m_alpha; }

    /**
     * @brief Set viscosity parameter
     * @param alpha New alpha parameter
     */
    void SetAlpha(double alpha) { m_alpha = alpha; }

    /**
     * @brief Get disk model type
     * @return Current model type
     */
    ModelType GetModelType() const { return m_modelType; }

    /**
     * @brief Set disk model type
     * @param type New model type
     */
    void SetModelType(ModelType type) { m_modelType = type; }

    /**
     * @brief Calculate temperature at given radius
     * @param radius Radial distance from black hole center
     * @return Temperature in Kelvin
     */
    double GetTemperature(double radius) const;

    /**
     * @brief Calculate surface density at given radius
     * @param radius Radial distance from black hole center
     * @return Surface density in kg/m²
     */
    double GetSurfaceDensity(double radius) const;

    /**
     * @brief Calculate disk height (scale height) at given radius
     * @param radius Radial distance from black hole center
     * @return Disk height in meters
     */
    double GetDiskHeight(double radius) const;

    /**
     * @brief Calculate orbital velocity at given radius
     * @param radius Radial distance from black hole center
     * @return Orbital velocity in m/s
     */
    double GetOrbitalVelocity(double radius) const;

    /**
     * @brief Calculate Keplerian frequency at given radius
     * @param radius Radial distance from black hole center
     * @return Orbital frequency in Hz
     */
    double GetKeplerianFrequency(double radius) const;

    /**
     * @brief Calculate blackbody emission at given radius and frequency
     * @param radius Radial distance from black hole center
     * @param frequency Photon frequency in Hz
     * @return Specific intensity in W⋅m⁻²⋅Hz⁻¹⋅sr⁻¹
     */
    double GetBlackbodyEmission(double radius, double frequency) const;

    /**
     * @brief Calculate total luminosity of the disk
     * @return Total luminosity in Watts
     */
    double GetTotalLuminosity() const;

    /**
     * @brief Calculate luminosity in given frequency range
     * @param minFreq Minimum frequency in Hz
     * @param maxFreq Maximum frequency in Hz
     * @return Luminosity in frequency range (Watts)
     */
    double GetLuminosity(double minFreq, double maxFreq) const;

    /**
     * @brief Calculate disk emission spectrum
     * @param frequencies Vector of frequencies in Hz
     * @return Vector of specific luminosities in W⋅Hz⁻¹
     */
    std::vector<double> GetEmissionSpectrum(const std::vector<double>& frequencies) const;

    /**
     * @brief Get disk color at given radius (for rendering)
     * @param radius Radial distance from black hole center
     * @return Color array [r, g, b, a] (0-1 range)
     */
    std::array<float, 4> GetDiskColor(double radius) const;

    /**
     * @brief Calculate relativistic corrections to disk properties
     * @param radius Radial distance from black hole center
     * @return Correction factors [temperature, flux, redshift]
     */
    std::array<double, 3> GetRelativisticCorrections(double radius) const;

    /**
     * @brief Calculate disk instability parameter (Toomre Q)
     * @param radius Radial distance from black hole center
     * @return Toomre Q parameter (dimensionless)
     */
    double GetToomreQ(double radius) const;

    /**
     * @brief Check if disk is stable at given radius
     * @param radius Radial distance from black hole center
     * @return True if stable (Q > 1), false if unstable
     */
    bool IsStable(double radius) const {
        return GetToomreQ(radius) > 1.0;
    }

    /**
     * @brief Calculate viscous timescale at given radius
     * @param radius Radial distance from black hole center
     * @return Viscous timescale in seconds
     */
    double GetViscousTimescale(double radius) const;

    /**
     * @brief Calculate thermal timescale at given radius
     * @param radius Radial distance from black hole center
     * @return Thermal timescale in seconds
     */
    double GetThermalTimescale(double radius) const;

    /**
     * @brief Update disk properties (for time evolution)
     * @param deltaTime Time step in seconds
     */
    void Update(double deltaTime);

    /**
     * @brief Generate random emission point on disk
     * @return [radius, theta, phi] coordinates of emission point
     */
    std::array<double, 3> GenerateEmissionPoint() const;

    /**
     * @brief Calculate disk opacity at given radius and frequency
     * @param radius Radial distance from black hole center
     * @param frequency Photon frequency in Hz
     * @return Opacity in m²/kg
     */
    double GetOpacity(double radius, double frequency) const;

    /**
     * @brief Calculate optical depth through disk
     * @param radius Radial distance from black hole center
     * @param frequency Photon frequency in Hz
     * @return Optical depth (dimensionless)
     */
    double GetOpticalDepth(double radius, double frequency) const;

    /**
     * @brief Get reference to central black hole
     * @return Shared pointer to black hole
     */
    std::shared_ptr<BlackHole> GetBlackHole() const { return m_blackHole; }

private:
    std::shared_ptr<BlackHole> m_blackHole;     ///< Central black hole
    double m_innerRadius;                       ///< Inner disk radius
    double m_outerRadius;                       ///< Outer disk radius
    double m_accretionRate;                     ///< Mass accretion rate (kg/s)
    double m_alpha;                             ///< Viscosity parameter
    ModelType m_modelType;                      ///< Disk model type

    // Cached values for performance
    mutable double m_cachedRadius;              ///< Last calculated radius
    mutable double m_cachedTemperature;         ///< Cached temperature
    mutable double m_cachedDensity;             ///< Cached surface density
    mutable bool m_cacheValid;                  ///< Cache validity flag

    // Physical constants
    static constexpr double STEFAN_BOLTZMANN = 5.670374419e-8;  ///< Stefan-Boltzmann constant
    static constexpr double BOLTZMANN = 1.380649e-23;           ///< Boltzmann constant
    static constexpr double H_PLANCK = 6.62607015e-34;          ///< Planck constant
    static constexpr double C = 299792458.0;                    ///< Speed of light
    static constexpr double PROTON_MASS = 1.67262192369e-27;    ///< Proton mass
    static constexpr double ELECTRON_SCATTERING = 6.6524587321e-29; ///< Thomson scattering cross-section

    /**
     * @brief Calculate disk properties using Shakura-Sunyaev model
     * @param radius Radial distance from black hole center
     * @return [temperature, surface_density, height]
     */
    std::array<double, 3> CalculateShakuraSunyaevProperties(double radius) const;

    /**
     * @brief Calculate disk properties using Novikov-Thorne model
     * @param radius Radial distance from black hole center
     * @return [temperature, surface_density, height]
     */
    std::array<double, 3> CalculateNovikovThorneProperties(double radius) const;

    /**
     * @brief Calculate relativistic factor for circular orbits
     * @param radius Radial distance from black hole center
     * @return Relativistic correction factor
     */
    double GetRelativisticFactor(double radius) const;

    /**
     * @brief Invalidate cached values
     */
    void InvalidateCache() { m_cacheValid = false; }

    /**
     * @brief Update cached values for given radius
     * @param radius Radial distance to cache
     */
    void UpdateCache(double radius) const;

    /**
     * @brief Convert temperature to RGB color
     * @param temperature Temperature in Kelvin
     * @return RGB color [r, g, b] (0-1 range)
     */
    static std::array<float, 3> TemperatureToRGB(double temperature);
};

} // namespace BlackHoleSim