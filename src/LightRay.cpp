/**
 * @file LightRay.cpp
 * @brief Implementation of the LightRay class
 */

#include "LightRay.hpp"
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

LightRay::LightRay()
    : m_position(0.0, 0.0, 0.0)
    , m_direction(1.0, 0.0, 0.0)
    , m_frequency(c / LAMBDA_GREEN) // Default to green light
    , m_intensity(1.0)
    , m_type(Type::DIRECT)
    , m_status(Status::ACTIVE)
    , m_maxPathPoints(1000)
{
    m_path.reserve(m_maxPathPoints);
    m_direction = m_direction.Normalized();
}

LightRay::LightRay(const Vector3& position, const Vector3& direction, double frequency, Type type)
    : m_position(position)
    , m_direction(direction.Normalized())
    , m_frequency(frequency)
    , m_intensity(1.0)
    , m_type(type)
    , m_status(Status::ACTIVE)
    , m_maxPathPoints(1000)
{
    m_path.reserve(m_maxPathPoints);
    
    // Add initial position to path
    AddToPath(m_position);
}

LightRay::LightRay(const Vector3& position, const Vector3& direction, double wavelength_nm, Type type)
    : m_position(position)
    , m_direction(direction.Normalized())
    , m_frequency(c / (wavelength_nm * 1e-9)) // Convert nm to m, then to frequency
    , m_intensity(1.0)
    , m_type(type)
    , m_status(Status::ACTIVE)
    , m_maxPathPoints(1000)
{
    m_path.reserve(m_maxPathPoints);
    
    // Add initial position to path
    AddToPath(m_position);
}

LightRay::~LightRay() {
    // Destructor - nothing special needed
}

// Getters
Vector3 LightRay::GetPosition() const {
    return m_position;
}

Vector3 LightRay::GetDirection() const {
    return m_direction;
}

double LightRay::GetFrequency() const {
    return m_frequency;
}

double LightRay::GetWavelength() const {
    return c / m_frequency;
}

double LightRay::GetWavelengthNm() const {
    return (c / m_frequency) * 1e9; // Convert to nanometers
}

double LightRay::GetIntensity() const {
    return m_intensity;
}

LightRay::Type LightRay::GetType() const {
    return m_type;
}

LightRay::Status LightRay::GetStatus() const {
    return m_status;
}

bool LightRay::IsActive() const {
    return m_status == Status::ACTIVE;
}

const std::vector<Vector3>& LightRay::GetPath() const {
    return m_path;
}

size_t LightRay::GetMaxPathPoints() const {
    return m_maxPathPoints;
}

// Setters
void LightRay::SetPosition(const Vector3& position) {
    m_position = position;
}

void LightRay::SetDirection(const Vector3& direction) {
    m_direction = direction.Normalized();
}

void LightRay::SetFrequency(double frequency) {
    m_frequency = std::max(1e10, frequency); // Minimum frequency to avoid issues
}

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

void LightRay::SetIntensity(double intensity) {
    m_intensity = std::max(0.0, intensity);
}

void LightRay::SetType(Type type) {
    m_type = type;
}

void LightRay::SetStatus(Status status) {
    m_status = status;
}

void LightRay::SetMaxPathPoints(size_t maxPoints) {
    m_maxPathPoints = maxPoints;
    
    // Resize path if necessary
    if (m_path.size() > maxPoints) {
        // Keep the most recent points
        m_path.erase(m_path.begin(), 
                    m_path.begin() + (m_path.size() - maxPoints));
    }
    
    m_path.reserve(maxPoints);
}

// Physics calculations
double LightRay::GetPhotonEnergy() const {
    return h * m_frequency;
}

double LightRay::GetPhotonMomentum() const {
    return h * m_frequency / c;
}

Vector3 LightRay::GetPhotonMomentumVector() const {
    return m_direction * GetPhotonMomentum();
}

double LightRay::CalculateRedshift(double initialFrequency, double finalFrequency) {
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
    Vector3 r = m_position - center;
    Vector3 L = r.Cross(m_direction); // Angular momentum vector
    return L.Magnitude();
}

double LightRay::GetConservedEnergy(const Vector3& center, double centralMass) const {
    // For photons in Schwarzschild metric: E = (1 - rs/r) * frequency
    double r = (m_position - center).Magnitude();
    double rs = 2.0 * 6.67430e-11 * centralMass / (c * c); // Schwarzschild radius
    
    if (r > rs && r > 0.0) {
        return (1.0 - rs / r) * GetPhotonEnergy();
    }
    
    return GetPhotonEnergy();
}

double LightRay::GetConservedAngularMomentum(const Vector3& center) const {
    // L = r * sin(θ) * (dφ/dt) for photons
    Vector3 r_vec = m_position - center;
    double r = r_vec.Magnitude();
    
    if (r > 0.0) {
        // Calculate angular component of direction
        Vector3 r_hat = r_vec / r;
        Vector3 tangential = m_direction - r_hat * m_direction.Dot(r_hat);
        return r * tangential.Magnitude() * GetPhotonMomentum();
    }
    
    return 0.0;
}

// Color and rendering
Vector3 LightRay::FrequencyToRGB(double frequency) {
    double wavelength = c / frequency; // in meters
    double wavelength_nm = wavelength * 1e9; // convert to nanometers
    
    return WavelengthToRGB(wavelength_nm);
}

Vector3 LightRay::WavelengthToRGB(double wavelength_nm) {
    Vector3 rgb(0.0, 0.0, 0.0);
    
    if (wavelength_nm >= 380 && wavelength_nm <= 750) {
        // Visible spectrum
        if (wavelength_nm >= 380 && wavelength_nm < 440) {
            // Violet to Blue
            double t = (wavelength_nm - 380) / (440 - 380);
            rgb.x = 0.5 - 0.5 * t; // Red component
            rgb.y = 0.0;           // Green component
            rgb.z = 1.0;           // Blue component
        } else if (wavelength_nm >= 440 && wavelength_nm < 490) {
            // Blue to Cyan
            double t = (wavelength_nm - 440) / (490 - 440);
            rgb.x = 0.0;           // Red component
            rgb.y = t;             // Green component
            rgb.z = 1.0;           // Blue component
        } else if (wavelength_nm >= 490 && wavelength_nm < 510) {
            // Cyan to Green
            double t = (wavelength_nm - 490) / (510 - 490);
            rgb.x = 0.0;           // Red component
            rgb.y = 1.0;           // Green component
            rgb.z = 1.0 - t;       // Blue component
        } else if (wavelength_nm >= 510 && wavelength_nm < 580) {
            // Green to Yellow
            double t = (wavelength_nm - 510) / (580 - 510);
            rgb.x = t;             // Red component
            rgb.y = 1.0;           // Green component
            rgb.z = 0.0;           // Blue component
        } else if (wavelength_nm >= 580 && wavelength_nm < 645) {
            // Yellow to Orange
            double t = (wavelength_nm - 580) / (645 - 580);
            rgb.x = 1.0;           // Red component
            rgb.y = 1.0 - 0.5 * t; // Green component
            rgb.z = 0.0;           // Blue component
        } else if (wavelength_nm >= 645 && wavelength_nm <= 750) {
            // Orange to Red
            double t = (wavelength_nm - 645) / (750 - 645);
            rgb.x = 1.0;           // Red component
            rgb.y = 0.5 - 0.5 * t; // Green component
            rgb.z = 0.0;           // Blue component
        }
        
        // Apply intensity falloff at edges of visible spectrum
        double intensity = 1.0;
        if (wavelength_nm < 420) {
            intensity = 0.3 + 0.7 * (wavelength_nm - 380) / (420 - 380);
        } else if (wavelength_nm > 700) {
            intensity = 0.3 + 0.7 * (750 - wavelength_nm) / (750 - 700);
        }
        
        rgb = rgb * intensity;
    } else if (wavelength_nm < 380) {
        // Ultraviolet - represent as dim violet
        rgb = Vector3(0.3, 0.0, 0.3);
    } else if (wavelength_nm > 750) {
        // Infrared - represent as dim red
        rgb = Vector3(0.3, 0.0, 0.0);
    }
    
    // Clamp values to [0, 1]
    rgb.x = std::clamp(rgb.x, 0.0, 1.0);
    rgb.y = std::clamp(rgb.y, 0.0, 1.0);
    rgb.z = std::clamp(rgb.z, 0.0, 1.0);
    
    return rgb;
}

Vector3 LightRay::GetColor() const {
    Vector3 baseColor = FrequencyToRGB(m_frequency);
    
    // Modulate by intensity
    double intensityFactor = std::clamp(m_intensity, 0.0, 1.0);
    
    return baseColor * intensityFactor;
}

Vector4 LightRay::GetColorWithAlpha(double alpha) const {
    Vector3 color = GetColor();
    return Vector4(color.x, color.y, color.z, std::clamp(alpha, 0.0, 1.0));
}

// Path management
void LightRay::AddToPath(const Vector3& position) {
    m_path.push_back(position);
    
    // Remove oldest points if we exceed the maximum
    if (m_path.size() > m_maxPathPoints) {
        m_path.erase(m_path.begin());
    }
}

void LightRay::ClearPath() {
    m_path.clear();
    // Add current position as the starting point
    m_path.push_back(m_position);
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
        case Type::EMITTED: return "Emitted";
        default: return "Unknown";
    }
}

std::string LightRay::GetStatusString() const {
    switch (m_status) {
        case Status::ACTIVE: return "Active";
        case Status::ESCAPED: return "Escaped";
        case Status::ABSORBED: return "Absorbed";
        case Status::SCATTERED: return "Scattered";
        default: return "Unknown";
    }
}

std::string LightRay::GetSpectralRegion() const {
    double wavelength_nm = GetWavelengthNm();
    
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
    m_position = position;
    m_direction = direction.Normalized();
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
        totalDistance += (m_path[i] - m_path[i-1]).Magnitude();
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
        Vector3 direction = m_path[1] - m_path[0];
        double magnitude = direction.Magnitude();
        if (magnitude > 1e-10) {
            return direction / magnitude;
        }
    }
    return m_direction;
}

bool LightRay::HasTraveledDistance(double minDistance) const {
    return GetDistanceTraveled() >= minDistance;
}

// Static utility methods
LightRay LightRay::CreateFromWavelength(const Vector3& position, const Vector3& direction, 
                                       double wavelength_nm, Type type) {
    return LightRay(position, direction, wavelength_nm, type);
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