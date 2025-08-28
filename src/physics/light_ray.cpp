/**
 * @file LightRay.cpp
 * @brief Implementation of the LightRay class
 */

#include "blackhole_sim/light_ray.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace BlackHoleSim {

// Physical constants
const double c = 299792458.0;           // Speed of light (m/s)
const double h = 6.62607015e-34;        // Planck constant (J⋅s)
const double k_B = 1.380649e-23;        // Boltzmann constant (J/K)

// Wavelength ranges for visible light (in meters)
const double LAMBDA_RED = 700e-9;       // 700 nm
const double LAMBDA_GREEN = 550e-9;     // 550 nm
const double LAMBDA_BLUE = 450e-9;      // 450 nm
const double LAMBDA_VIOLET = 380e-9;    // 380 nm
const double LAMBDA_INFRARED = 1000e-9; // 1000 nm
const double LAMBDA_ULTRAVIOLET = 300e-9; // 300 nm

// Default constructor removed - not declared in header

LightRay::LightRay(const std::array<double, 3>& position,
                   const std::array<double, 3>& direction,
                   double frequency,
                   double intensity,
                   Type type)
    : m_position(position)
    , m_direction(direction)
    , m_frequency(frequency)
    , m_intensity(intensity)
    , m_type(type)
    , m_status(Status::ACTIVE)
    , m_path()
    , m_maxPathLength(1000)
{
    // Normalize direction
    Vector3 dir(direction[0], direction[1], direction[2]);
    Vector3 normalized = dir.Normalized();
    m_direction = normalized.ToArray();
    
    m_path.reserve(m_maxPathLength);
    
    // Add initial position to path
    AddToPath(m_position);
}

// Destructor - using default implementation

// Getters
// GetPosition and GetDirection are implemented inline in the header

// GetFrequency is implemented inline in the header

double LightRay::GetWavelength() const {
    return c / m_frequency;
}

double LightRay::GetWavelengthNm() const {
    return (c / m_frequency) * 1e9; // Convert to nanometers
}

// GetIntensity is implemented inline in the header

// GetType and GetStatus are implemented inline in the header

bool LightRay::IsActive() const {
    return m_status == Status::ACTIVE;
}

// GetPath is implemented inline in the header
// GetMaxPathPoints -> GetMaxPathLength is implemented inline in the header
// SetPosition is implemented inline in the header

void LightRay::SetDirection(const std::array<double, 3>& direction) {
    Vector3 dir(direction[0], direction[1], direction[2]);
    Vector3 normalized = dir.Normalized();
    m_direction = normalized.ToArray();
}

// SetFrequency is implemented inline in the header

void LightRay::SetWavelength(double wavelength) {
    if (wavelength > 0.0) {
        m_frequency = c / wavelength;
    }
}

void LightRay::SetWavelengthNm(double wavelength_nm) {
    if (wavelength_nm > 0.0) {
        m_frequency = c / (wavelength_nm * 1e-9);
    }
}

// SetIntensity, SetType, and SetStatus are implemented inline in the header
// SetMaxPathLength is implemented inline in the header

// Physics calculations
double LightRay::GetPhotonEnergy() const {
    return h * m_frequency;
}

double LightRay::GetPhotonMomentum() const {
    return h * m_frequency / c;
}

Vector3 LightRay::GetPhotonMomentumVector() const {
    Vector3 direction(m_direction[0], m_direction[1], m_direction[2]);
    return direction * GetPhotonMomentum();
}

double LightRay::CalculateRedshift(double initialFrequency, double finalFrequency) const {
    if (finalFrequency > 0.0 && initialFrequency > 0.0) {
        return (initialFrequency - finalFrequency) / finalFrequency;
    }
    return 0.0;
}

double LightRay::CalculateRedshift(double initialFrequency) const {
    return CalculateRedshift(initialFrequency, m_frequency);
}

double LightRay::GetImpactParameter(const Vector3& center) const {
    // Calculate impact parameter for light ray relative to center
    Vector3 position_vec(m_position);
    Vector3 direction_vec(m_direction);
    Vector3 r = position_vec - center;
    Vector3 L = r.Cross(direction_vec); // Angular momentum vector
    return L.Magnitude();
}

double LightRay::GetConservedEnergy(const Vector3& center, double centralMass) const {
    // For photons in Schwarzschild metric: E = (1 - rs/r) * frequency
    Vector3 position_vec(m_position);
    double r = (position_vec - center).Magnitude();
    double rs = 2.0 * 6.67430e-11 * centralMass / (c * c); // Schwarzschild radius
    
    if (r > rs && r > 0.0) {
        return (1.0 - rs / r) * GetFrequency();
    }
    
    return GetFrequency();
}

double LightRay::GetConservedAngularMomentum(const Vector3& center) const {
    // L = r * sin(θ) * (dφ/dt) for photons
    Vector3 position_vec(m_position);
    Vector3 direction_vec(m_direction);
    Vector3 r_vec = position_vec - center;
    double r = r_vec.Magnitude();
    
    if (r > 0.0) {
        // Calculate angular component of direction
        Vector3 r_hat = r_vec / r;
        Vector3 tangential = direction_vec - r_hat * direction_vec.Dot(r_hat);
        return r * tangential.Magnitude() * GetWavelength(); // Use wavelength as momentum proxy
    }
    
    return 0.0;
}

// Color and rendering
std::array<float, 3> LightRay::FrequencyToRGB(double frequency) {
    double wavelength = c / frequency; // in meters
    double wavelength_nm = wavelength * 1e9; // convert to nanometers
    
    return WavelengthToRGB(wavelength_nm);
}

std::array<float, 3> LightRay::WavelengthToRGB(double wavelength_nm) {
    std::array<float, 3> rgb = {0.0f, 0.0f, 0.0f};
    
    if (wavelength_nm >= 380 && wavelength_nm <= 750) {
        // Visible spectrum
        if (wavelength_nm >= 380 && wavelength_nm < 440) {
            // Violet to Blue
            double t = (wavelength_nm - 380) / (440 - 380);
            rgb[0] = 0.5 - 0.5 * t; // Red component
            rgb[1] = 0.0;           // Green component
            rgb[2] = 1.0;           // Blue component
        } else if (wavelength_nm >= 440 && wavelength_nm < 490) {
            // Blue to Cyan
            double t = (wavelength_nm - 440) / (490 - 440);
            rgb[0] = 0.0;           // Red component
            rgb[1] = t;             // Green component
            rgb[2] = 1.0;           // Blue component
        } else if (wavelength_nm >= 490 && wavelength_nm < 510) {
            // Cyan to Green
            double t = (wavelength_nm - 490) / (510 - 490);
            rgb[0] = 0.0;           // Red component
            rgb[1] = 1.0;           // Green component
            rgb[2] = 1.0 - t;       // Blue component
        } else if (wavelength_nm >= 510 && wavelength_nm < 580) {
            // Green to Yellow
            double t = (wavelength_nm - 510) / (580 - 510);
            rgb[0] = t;             // Red component
            rgb[1] = 1.0;           // Green component
            rgb[2] = 0.0;           // Blue component
        } else if (wavelength_nm >= 580 && wavelength_nm < 645) {
            // Yellow to Orange
            double t = (wavelength_nm - 580) / (645 - 580);
            rgb[0] = 1.0;           // Red component
            rgb[1] = 1.0 - 0.5 * t; // Green component
            rgb[2] = 0.0;           // Blue component
        } else if (wavelength_nm >= 645 && wavelength_nm <= 750) {
            // Orange to Red
            double t = (wavelength_nm - 645) / (750 - 645);
            rgb[0] = 1.0;           // Red component
            rgb[1] = 0.5 - 0.5 * t; // Green component
            rgb[2] = 0.0;           // Blue component
        }
        
        // Apply intensity falloff at edges of visible spectrum
        double intensity = 1.0;
        if (wavelength_nm < 420) {
            intensity = 0.3 + 0.7 * (wavelength_nm - 380) / (420 - 380);
        } else if (wavelength_nm > 700) {
            intensity = 0.3 + 0.7 * (750 - wavelength_nm) / (750 - 700);
        }
        
        rgb[0] *= intensity;
        rgb[1] *= intensity;
        rgb[2] *= intensity;
    } else if (wavelength_nm < 380) {
        // Ultraviolet - represent as dim violet
        rgb[0] = 0.3f;
        rgb[1] = 0.0f;
        rgb[2] = 0.3f;
    } else if (wavelength_nm > 750) {
        // Infrared - represent as dim red
        rgb[0] = 0.3f;
        rgb[1] = 0.0f;
        rgb[2] = 0.0f;
    }
    
    // Clamp values to [0, 1]
    rgb[0] = std::min(std::max(rgb[0], 0.0f), 1.0f);
    rgb[1] = std::min(std::max(rgb[1], 0.0f), 1.0f);
    rgb[2] = std::min(std::max(rgb[2], 0.0f), 1.0f);
    
    return rgb;
}

std::array<float, 4> LightRay::GetColor() const {
    std::array<float, 3> baseColor = FrequencyToRGB(m_frequency);
    
    // Modulate by intensity
    double intensityFactor = std::min(std::max(m_intensity, 0.0), 1.0);
    
    return {static_cast<float>(baseColor[0] * intensityFactor), 
            static_cast<float>(baseColor[1] * intensityFactor), 
            static_cast<float>(baseColor[2] * intensityFactor), 
            1.0f};
}



// Path management
void LightRay::AddToPath(const Vector3& position) {
    m_path.push_back(position.ToArray());
    
    // Remove oldest points if we exceed the maximum
    if (m_path.size() > m_maxPathLength) {
        m_path.erase(m_path.begin());
    }
}



void LightRay::ReservePathCapacity(size_t capacity) {
    m_path.reserve(capacity);
}

// Utility methods
std::string LightRay::GetTypeString() const {
    switch (m_type) {
        case Type::DIRECT: return "Direct";
        case Type::LENSED: return "Lensed";
        case Type::SCATTERED: return "Scattered";
        case Type::BACKGROUND: return "Background";
        case Type::CUSTOM: return "Custom";
        default: return "Unknown";
    }
}

std::string LightRay::GetStatusString() const {
    switch (m_status) {
        case Status::ACTIVE: return "Active";
        case Status::ESCAPED: return "Escaped";
        case Status::ABSORBED: return "Absorbed";
        case Status::TERMINATED: return "Terminated";
        default: return "Unknown";
    }
}

std::string LightRay::GetSpectralRegion() const {
    double wavelength_nm = GetWavelength() * 1e9; // Convert from meters to nanometers
    
    if (wavelength_nm < 10) {
        return "Gamma Ray";
    } else if (wavelength_nm < 0.01) {
        return "X-Ray";
    } else if (wavelength_nm < 380) {
        return "Ultraviolet";
    } else if (wavelength_nm < 450) {
        return "Violet";
    } else if (wavelength_nm < 495) {
        return "Blue";
    } else if (wavelength_nm < 570) {
        return "Green";
    } else if (wavelength_nm < 590) {
        return "Yellow";
    } else if (wavelength_nm < 620) {
        return "Orange";
    } else if (wavelength_nm < 750) {
        return "Red";
    } else if (wavelength_nm < 1000000) {
        return "Infrared";
    } else if (wavelength_nm < 1000000000) {
        return "Microwave";
    } else {
        return "Radio";
    }
}

void LightRay::Reset(const Vector3& position, const Vector3& direction, double frequency) {
    m_position = position.ToArray();
    m_direction = direction.Normalized().ToArray();
    m_frequency = frequency;
    m_intensity = 1.0;
    m_status = Status::ACTIVE;
    ClearPath();
}

void LightRay::ApplyRedshift(double redshiftFactor) {
    if (redshiftFactor > 0.0) {
        m_frequency *= redshiftFactor;
        
        // Intensity typically decreases with redshift (cosmological dimming)
        // For gravitational redshift, this relationship may be different
        m_intensity *= redshiftFactor * redshiftFactor;
    }
}

void LightRay::ApplyBlueshift(double blueshiftFactor) {
    if (blueshiftFactor > 0.0) {
        m_frequency *= blueshiftFactor;
        
        // Intensity typically increases with blueshift
        m_intensity *= blueshiftFactor * blueshiftFactor;
    }
}

double LightRay::GetDistanceTraveled() const {
    if (m_path.size() < 2) {
        return 0.0;
    }
    
    double totalDistance = 0.0;
    for (size_t i = 1; i < m_path.size(); ++i) {
        Vector3 current(m_path[i]);
        Vector3 previous(m_path[i-1]);
        totalDistance += (current - previous).Magnitude();
    }
    
    return totalDistance;
}

Vector3 LightRay::GetInitialPosition() const {
    if (!m_path.empty()) {
        return m_path[0];
    }
    return m_position;
}

Vector3 LightRay::GetInitialDirection() const {
    if (m_path.size() >= 2) {
        Vector3 pos1(m_path[1]);
        Vector3 pos0(m_path[0]);
        Vector3 direction = pos1 - pos0;
        double magnitude = direction.Magnitude();
        if (magnitude > 1e-10) {
            return direction / magnitude;
        }
    }
    return Vector3(m_direction);
}

bool LightRay::HasTraveledDistance(double minDistance) const {
    return GetDistanceTraveled() >= minDistance;
}

// Static utility methods
LightRay LightRay::CreateFromWavelength(const Vector3& position, const Vector3& direction, 
                                       double wavelength_nm, Type type) {
    return LightRay(position.ToArray(), direction.ToArray(), wavelength_nm, 1.0, type);
}

LightRay LightRay::CreateRedLight(const Vector3& position, const Vector3& direction) {
    return CreateFromWavelength(position, direction, 650.0, Type::DIRECT);
}

LightRay LightRay::CreateGreenLight(const Vector3& position, const Vector3& direction) {
    return CreateFromWavelength(position, direction, 550.0, Type::DIRECT);
}

LightRay LightRay::CreateBlueLight(const Vector3& position, const Vector3& direction) {
    return CreateFromWavelength(position, direction, 450.0, Type::DIRECT);
}

LightRay LightRay::CreateWhiteLight(const Vector3& position, const Vector3& direction) {
    return CreateFromWavelength(position, direction, 550.0, Type::DIRECT); // Use green as representative
}

std::vector<LightRay> LightRay::CreateSpectrum(const Vector3& position, const Vector3& direction, 
                                              double minWavelength_nm, double maxWavelength_nm, 
                                              int numRays) {
    std::vector<LightRay> spectrum;
    spectrum.reserve(numRays);
    
    if (numRays <= 1) {
        double avgWavelength = (minWavelength_nm + maxWavelength_nm) / 2.0;
        spectrum.push_back(CreateFromWavelength(position, direction, avgWavelength, Type::DIRECT));
        return spectrum;
    }
    
    double wavelengthStep = (maxWavelength_nm - minWavelength_nm) / (numRays - 1);
    
    for (int i = 0; i < numRays; ++i) {
        double wavelength = minWavelength_nm + i * wavelengthStep;
        spectrum.push_back(CreateFromWavelength(position, direction, wavelength, Type::DIRECT));
    }
    
    return spectrum;
}

} // namespace BlackHoleSim