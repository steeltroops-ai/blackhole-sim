/**
 * @file AccretionDisk.cpp
 * @brief Implementation of the AccretionDisk class
 */

#include "AccretionDisk.hpp"
#include "BlackHole.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace BlackHoleSim {

// Physical constants
const double G = 6.67430e-11;          // Gravitational constant (m³/kg⋅s²)
const double c = 299792458.0;           // Speed of light (m/s)
const double sigma_SB = 5.670374419e-8; // Stefan-Boltzmann constant (W⋅m⁻²⋅K⁻⁴)
const double k_B = 1.380649e-23;        // Boltzmann constant (J/K)
const double h = 6.62607015e-34;        // Planck constant (J⋅s)
const double m_p = 1.67262192369e-27;   // Proton mass (kg)
const double m_e = 9.1093837015e-31;    // Electron mass (kg)

// Typical accretion disk parameters
const double ALPHA_DEFAULT = 0.1;       // Shakura-Sunyaev alpha parameter
const double BETA_DEFAULT = 0.5;        // Magnetic pressure ratio

AccretionDisk::AccretionDisk(double innerRadius, double outerRadius, double accretionRate, 
                            ModelType model, double alpha)
    : m_innerRadius(innerRadius)
    , m_outerRadius(outerRadius)
    , m_accretionRate(accretionRate)
    , m_alpha(alpha)
    , m_modelType(model)
    , m_temperature(0.0)
    , m_density(0.0)
    , m_luminosity(0.0)
    , m_isActive(true)
    , m_numRings(50)
    , m_numSectors(32)
{
    // Initialize disk properties
    InitializeDisk();
}

AccretionDisk::~AccretionDisk() {
    // Destructor - nothing special needed
}

void AccretionDisk::InitializeDisk() {
    if (m_innerRadius >= m_outerRadius || m_accretionRate <= 0.0) {
        m_isActive = false;
        return;
    }
    
    // Calculate typical disk properties at middle radius
    double midRadius = std::sqrt(m_innerRadius * m_outerRadius);
    m_temperature = CalculateTemperature(midRadius, 1.0); // Assume 1 solar mass for now
    m_density = CalculateDensity(midRadius, 1.0);
    m_luminosity = CalculateTotalLuminosity(1.0);
    
    m_isActive = true;
    
    std::cout << "AccretionDisk initialized:\n";
    std::cout << "  Inner radius: " << m_innerRadius << " m\n";
    std::cout << "  Outer radius: " << m_outerRadius << " m\n";
    std::cout << "  Accretion rate: " << m_accretionRate << " kg/s\n";
    std::cout << "  Model: " << GetModelString() << "\n";
    std::cout << "  Alpha parameter: " << m_alpha << "\n";
    std::cout << "  Typical temperature: " << m_temperature << " K\n";
    std::cout << "  Total luminosity: " << m_luminosity << " W\n";
}

// Getters
double AccretionDisk::GetInnerRadius() const {
    return m_innerRadius;
}

double AccretionDisk::GetOuterRadius() const {
    return m_outerRadius;
}

double AccretionDisk::GetAccretionRate() const {
    return m_accretionRate;
}

double AccretionDisk::GetAlpha() const {
    return m_alpha;
}

AccretionDisk::ModelType AccretionDisk::GetModel() const {
    return m_modelType;
}

double AccretionDisk::GetTemperature() const {
    return m_temperature;
}

double AccretionDisk::GetDensity() const {
    return m_density;
}

double AccretionDisk::GetLuminosity() const {
    return m_luminosity;
}

bool AccretionDisk::IsActive() const {
    return m_isActive;
}

size_t AccretionDisk::GetNumRings() const {
    return m_numRings;
}

size_t AccretionDisk::GetNumSectors() const {
    return m_numSectors;
}

// Setters
void AccretionDisk::SetInnerRadius(double radius) {
    m_innerRadius = std::max(0.0, radius);
    if (m_isActive) {
        InitializeDisk();
    }
}

void AccretionDisk::SetOuterRadius(double radius) {
    m_outerRadius = std::max(m_innerRadius, radius);
    if (m_isActive) {
        InitializeDisk();
    }
}

void AccretionDisk::SetAccretionRate(double rate) {
    m_accretionRate = std::max(0.0, rate);
    if (m_isActive) {
        InitializeDisk();
    }
}

void AccretionDisk::SetAlpha(double alpha) {
    m_alpha = std::clamp(alpha, 0.01, 1.0); // Reasonable range for alpha
    if (m_isActive) {
        InitializeDisk();
    }
}

void AccretionDisk::SetModel(ModelType model) {
    m_modelType = model;
    if (m_isActive) {
        InitializeDisk();
    }
}

void AccretionDisk::SetActive(bool active) {
    m_isActive = active;
    if (active) {
        InitializeDisk();
    }
}

void AccretionDisk::SetNumRings(size_t numRings) {
    m_numRings = std::max(size_t(10), std::min(size_t(200), numRings));
}

void AccretionDisk::SetNumSectors(size_t numSectors) {
    m_numSectors = std::max(size_t(8), std::min(size_t(128), numSectors));
}

// Physics calculations
double AccretionDisk::CalculateTemperature(double radius, double blackHoleMass) const {
    if (!m_isActive || radius <= 0.0 || blackHoleMass <= 0.0) {
        return 0.0;
    }
    
    switch (m_modelType) {
        case ModelType::SHAKURA_SUNYAEV:
            return CalculateShakuraSunyaevTemperature(radius, blackHoleMass);
        case ModelType::NOVIKOV_THORNE:
            return CalculateNovikovThorneTemperature(radius, blackHoleMass);
        default:
            return CalculateShakuraSunyaevTemperature(radius, blackHoleMass);
    }
}

double AccretionDisk::CalculateShakuraSunyaevTemperature(double radius, double blackHoleMass) const {
    // Shakura-Sunyaev temperature profile
    // T ∝ (GM*Mdot/(4πσr³))^(1/4) * f(r)
    
    const double rs = 2.0 * G * blackHoleMass / (c * c); // Schwarzschild radius
    
    if (radius <= 3.0 * rs) {
        return 0.0; // Inside ISCO
    }
    
    // Basic temperature scaling
    double T_base = std::pow((G * blackHoleMass * m_accretionRate) / 
                            (4.0 * M_PI * sigma_SB * radius * radius * radius), 0.25);
    
    // Correction factor for inner disk
    double f_correction = 1.0 - std::sqrt(3.0 * rs / radius);
    f_correction = std::max(0.0, f_correction);
    
    return T_base * std::pow(f_correction, 0.25);
}

double AccretionDisk::CalculateNovikovThorneTemperature(double radius, double blackHoleMass) const {
    // Novikov-Thorne temperature profile (more accurate for thin disks)
    const double rs = 2.0 * G * blackHoleMass / (c * c);
    const double r_isco = 3.0 * rs; // ISCO for Schwarzschild black hole
    
    if (radius <= r_isco) {
        return 0.0;
    }
    
    // Dimensionless radius
    double x = radius / rs;
    double x_isco = r_isco / rs;
    
    // Novikov-Thorne efficiency function
    double efficiency = (1.0 - std::sqrt(x_isco / x)) / (x * x * (x - 3.0) * (x - 3.0));
    efficiency *= (x - x_isco);
    efficiency = std::max(0.0, efficiency);
    
    // Temperature calculation
    double T_factor = (3.0 * G * blackHoleMass * m_accretionRate) / 
                     (8.0 * M_PI * sigma_SB * radius * radius * radius);
    
    return std::pow(T_factor * efficiency, 0.25);
}

double AccretionDisk::CalculateDensity(double radius, double blackHoleMass) const {
    if (!m_isActive || radius <= 0.0 || blackHoleMass <= 0.0) {
        return 0.0;
    }
    
    // Surface density calculation (Shakura-Sunyaev model)
    double T = CalculateTemperature(radius, blackHoleMass);
    
    if (T <= 0.0) {
        return 0.0;
    }
    
    // Pressure scale height
    double cs = std::sqrt(k_B * T / m_p); // Sound speed
    double Omega = std::sqrt(G * blackHoleMass / (radius * radius * radius)); // Keplerian frequency
    double H = cs / Omega; // Scale height
    
    // Surface density from continuity equation
    double Sigma = m_accretionRate / (3.0 * M_PI * m_alpha * H * cs);
    
    // Volume density (approximate)
    return Sigma / (2.0 * H);
}

double AccretionDisk::CalculateSurfaceDensity(double radius, double blackHoleMass) const {
    if (!m_isActive || radius <= 0.0 || blackHoleMass <= 0.0) {
        return 0.0;
    }
    
    double T = CalculateTemperature(radius, blackHoleMass);
    
    if (T <= 0.0) {
        return 0.0;
    }
    
    // Sound speed and orbital frequency
    double cs = std::sqrt(k_B * T / m_p);
    double Omega = std::sqrt(G * blackHoleMass / (radius * radius * radius));
    double H = cs / Omega;
    
    // Surface density from Shakura-Sunyaev model
    return m_accretionRate / (3.0 * M_PI * m_alpha * H * cs);
}

double AccretionDisk::CalculateLuminosity(double radius, double blackHoleMass) const {
    if (!m_isActive || radius <= 0.0 || blackHoleMass <= 0.0) {
        return 0.0;
    }
    
    double T = CalculateTemperature(radius, blackHoleMass);
    
    if (T <= 0.0) {
        return 0.0;
    }
    
    // Stefan-Boltzmann law for blackbody emission
    double flux = sigma_SB * T * T * T * T;
    
    // Luminosity per unit area (both sides of disk)
    return 2.0 * flux;
}

double AccretionDisk::CalculateTotalLuminosity(double blackHoleMass) const {
    if (!m_isActive || blackHoleMass <= 0.0) {
        return 0.0;
    }
    
    // Integrate luminosity over disk area
    double totalLuminosity = 0.0;
    double dr = (m_outerRadius - m_innerRadius) / 1000.0; // Integration step
    
    for (double r = m_innerRadius; r < m_outerRadius; r += dr) {
        double luminosityDensity = CalculateLuminosity(r, blackHoleMass);
        double area = 2.0 * M_PI * r * dr; // Annular area
        totalLuminosity += luminosityDensity * area;
    }
    
    return totalLuminosity;
}

double AccretionDisk::CalculateEfficiency(double blackHoleMass) const {
    if (!m_isActive || blackHoleMass <= 0.0 || m_accretionRate <= 0.0) {
        return 0.0;
    }
    
    double totalLuminosity = CalculateTotalLuminosity(blackHoleMass);
    double restMassEnergy = m_accretionRate * c * c;
    
    if (restMassEnergy > 0.0) {
        return totalLuminosity / restMassEnergy;
    }
    
    return 0.0;
}

// Emission spectrum
double AccretionDisk::CalculateBlackbodySpectrum(double frequency, double temperature) const {
    if (temperature <= 0.0 || frequency <= 0.0) {
        return 0.0;
    }
    
    // Planck function: B(ν,T) = (2hν³/c²) / (exp(hν/kT) - 1)
    double x = h * frequency / (k_B * temperature);
    
    if (x > 700.0) {
        // Avoid overflow in exponential
        return 0.0;
    }
    
    double prefactor = (2.0 * h * frequency * frequency * frequency) / (c * c);
    double exponential = std::exp(x) - 1.0;
    
    if (exponential <= 0.0) {
        return 0.0;
    }
    
    return prefactor / exponential;
}

double AccretionDisk::CalculateEmissionSpectrum(double frequency, double radius, double blackHoleMass) const {
    double temperature = CalculateTemperature(radius, blackHoleMass);
    return CalculateBlackbodySpectrum(frequency, temperature);
}

Vector3 AccretionDisk::CalculateEmissionColor(double radius, double blackHoleMass) const {
    double temperature = CalculateTemperature(radius, blackHoleMass);
    
    if (temperature <= 0.0) {
        return Vector3(0.0, 0.0, 0.0);
    }
    
    return BlackbodyTemperatureToRGB(temperature);
}

Vector3 AccretionDisk::BlackbodyTemperatureToRGB(double temperature) const {
    // Convert blackbody temperature to RGB color
    // Based on approximation of blackbody spectrum
    
    if (temperature <= 0.0) {
        return Vector3(0.0, 0.0, 0.0);
    }
    
    Vector3 rgb;
    
    // Normalize temperature (typical range: 1000K - 50000K)
    double t = temperature / 1000.0;
    
    if (t < 1.0) {
        // Very cool - deep red
        rgb = Vector3(0.5, 0.0, 0.0);
    } else if (t < 3.0) {
        // Cool - red to orange
        double factor = (t - 1.0) / 2.0;
        rgb = Vector3(1.0, 0.3 * factor, 0.0);
    } else if (t < 5.0) {
        // Warm - orange to yellow
        double factor = (t - 3.0) / 2.0;
        rgb = Vector3(1.0, 0.3 + 0.7 * factor, 0.0);
    } else if (t < 7.0) {
        // Hot - yellow to white
        double factor = (t - 5.0) / 2.0;
        rgb = Vector3(1.0, 1.0, factor);
    } else if (t < 15.0) {
        // Very hot - white to blue-white
        double factor = (t - 7.0) / 8.0;
        rgb = Vector3(1.0 - 0.2 * factor, 1.0 - 0.1 * factor, 1.0);
    } else {
        // Extremely hot - blue
        rgb = Vector3(0.6, 0.8, 1.0);
    }
    
    // Apply intensity based on temperature (Stefan-Boltzmann law)
    double intensity = std::pow(temperature / 5778.0, 4.0); // Relative to Sun
    intensity = std::clamp(intensity, 0.01, 10.0);
    
    rgb = rgb * std::sqrt(intensity); // Square root for better visual scaling
    
    // Clamp to [0, 1]
    double clampedX = std::clamp(rgb.x(), 0.0, 1.0);
    double clampedY = std::clamp(rgb.y(), 0.0, 1.0);
    double clampedZ = std::clamp(rgb.z(), 0.0, 1.0);
    rgb = Vector3(clampedX, clampedY, clampedZ);
    
    return rgb;
}

// Relativistic effects
double AccretionDisk::CalculateDopplerShift(double radius, double observerAngle, double blackHoleMass) const {
    if (radius <= 0.0 || blackHoleMass <= 0.0) {
        return 1.0; // No shift
    }
    
    // Keplerian velocity
    double v_orbital = std::sqrt(G * blackHoleMass / radius);
    double beta = v_orbital / c;
    
    // Doppler factor for circular motion
    // δ = 1 / (γ(1 - β cos θ))
    double gamma = 1.0 / std::sqrt(1.0 - beta * beta);
    double cos_theta = std::cos(observerAngle);
    
    return 1.0 / (gamma * (1.0 - beta * cos_theta));
}

double AccretionDisk::CalculateGravitationalRedshift(double radius, double blackHoleMass) const {
    if (radius <= 0.0 || blackHoleMass <= 0.0) {
        return 1.0;
    }
    
    double rs = 2.0 * G * blackHoleMass / (c * c);
    
    if (radius <= rs) {
        return 0.0; // Infinite redshift at event horizon
    }
    
    return std::sqrt(1.0 - rs / radius);
}

double AccretionDisk::CalculateRelativisticCorrection(double radius, double blackHoleMass) const {
    // Combined relativistic effects
    double redshift = CalculateGravitationalRedshift(radius, blackHoleMass);
    
    // Additional corrections for frame dragging, etc. could be added here
    
    return redshift;
}

// Utility methods
std::string AccretionDisk::GetModelString() const {
    switch (m_modelType) {
        case ModelType::SHAKURA_SUNYAEV: return "Shakura-Sunyaev";
        case ModelType::NOVIKOV_THORNE: return "Novikov-Thorne";
        case ModelType::THICK_DISK: return "Thick Disk";
        case ModelType::CUSTOM: return "Custom";
        default: return "Unknown";
    }
}

bool AccretionDisk::IsInDisk(const Vector3& position, const Vector3& diskCenter, 
                            const Vector3& diskNormal) const {
    if (!m_isActive) {
        return false;
    }
    
    Vector3 relativePos = position - diskCenter;
    double radialDistance = relativePos.Magnitude();
    
    // Check if within radial bounds
    if (radialDistance < m_innerRadius || radialDistance > m_outerRadius) {
        return false;
    }
    
    // Check if close to disk plane
    Vector3 normalizedNormal = diskNormal.Normalized();
    double dotProduct = relativePos.Dot(normalizedNormal);
    double heightAboveDisk = std::abs(dotProduct);
    
    // Disk thickness (approximate)
    double scaleHeight = radialDistance * 0.1; // Typical H/R ~ 0.1
    
    return heightAboveDisk < scaleHeight;
}

Vector3 AccretionDisk::GetDiskPosition(double radius, double angle, const Vector3& center, 
                                      const Vector3& normal) const {
    // Create position in disk plane
    Vector3 normalizedNormal = normal.Normalized();
    
    // Find two orthogonal vectors in the disk plane
    Vector3 u, v;
    double normalZ = normalizedNormal.z();
    if (std::abs(normalZ) < 0.9) {
        u = Vector3(0, 0, 1).Cross(normalizedNormal).Normalized();
    } else {
        u = Vector3(1, 0, 0).Cross(normalizedNormal).Normalized();
    }
    v = normalizedNormal.Cross(u);
    
    // Position in disk plane
    Vector3 diskPos = u * (radius * std::cos(angle)) + v * (radius * std::sin(angle));
    
    return center + diskPos;
}

double AccretionDisk::GetDiskHeight(double radius) const {
    // Typical disk scale height H/R ~ 0.1 for thin disks
    return radius * 0.1;
}

void AccretionDisk::Update(double deltaTime, double blackHoleMass) {
    (void)deltaTime; // Suppress unused parameter warning
    if (!m_isActive) {
        return;
    }
    
    // Update disk properties (could include time evolution)
    // For now, just recalculate based on current parameters
    double midRadius = std::sqrt(m_innerRadius * m_outerRadius);
    m_temperature = CalculateTemperature(midRadius, blackHoleMass);
    m_density = CalculateDensity(midRadius, blackHoleMass);
    m_luminosity = CalculateTotalLuminosity(blackHoleMass);
}

void AccretionDisk::Reset() {
    m_temperature = 0.0;
    m_density = 0.0;
    m_luminosity = 0.0;
    m_isActive = false;
}

} // namespace BlackHoleSim