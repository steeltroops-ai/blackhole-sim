#pragma once

#include "BlackHole.hpp"
#include "Particle.hpp"
#include "LightRay.hpp"
#include <vector>
#include <array>
#include <memory>
#include <functional>

namespace BlackHoleSim {

/**
 * @brief Core physics engine for black hole simulation
 * 
 * Handles geodesic integration, gravitational lensing calculations,
 * and all physics-related computations using the Schwarzschild metric.
 */
class PhysicsEngine {
public:
    /**
     * @brief Integration method for geodesic equations
     */
    enum class IntegrationMethod {
        EULER,           ///< Simple Euler method (fast, less accurate)
        RUNGE_KUTTA_4,   ///< 4th order Runge-Kutta (balanced)
        ADAPTIVE_RK45    ///< Adaptive Runge-Kutta (accurate, variable step)
    };

    /**
     * @brief Construct physics engine with default parameters
     */
    PhysicsEngine();

    /**
     * @brief Initialize physics engine with black hole
     * @param blackHole Pointer to black hole object
     * @return True if initialization successful
     */
    bool Initialize(std::shared_ptr<BlackHole> blackHole);

    /**
     * @brief Update all physics objects by one time step
     * @param deltaTime Time step in seconds
     * @param particles Vector of particles to update
     * @param lightRays Vector of light rays to update
     */
    void Update(double deltaTime, 
                std::vector<std::unique_ptr<Particle>>& particles,
                std::vector<std::unique_ptr<LightRay>>& lightRays);

    /**
     * @brief Set integration method
     * @param method Integration method to use
     */
    void SetIntegrationMethod(IntegrationMethod method) { m_integrationMethod = method; }

    /**
     * @brief Set integration tolerance for adaptive methods
     * @param tolerance Error tolerance (default: 1e-8)
     */
    void SetIntegrationTolerance(double tolerance) { m_tolerance = tolerance; }

    /**
     * @brief Set maximum integration step size
     * @param maxStep Maximum step size in seconds
     */
    void SetMaxStepSize(double maxStep) { m_maxStepSize = maxStep; }

    /**
     * @brief Calculate geodesic derivatives for massive particle
     * @param state Current state [t, r, theta, phi, dt/dtau, dr/dtau, dtheta/dtau, dphi/dtau]
     * @param derivatives Output derivatives array
     */
    void CalculateMassiveGeodesicDerivatives(const std::array<double, 8>& state,
                                           std::array<double, 8>& derivatives) const;

    /**
     * @brief Calculate geodesic derivatives for light ray (null geodesic)
     * @param state Current state [t, r, theta, phi, dt/dlambda, dr/dlambda, dtheta/dlambda, dphi/dlambda]
     * @param derivatives Output derivatives array
     */
    void CalculateNullGeodesicDerivatives(const std::array<double, 8>& state,
                                        std::array<double, 8>& derivatives) const;

    /**
     * @brief Calculate gravitational lensing deflection angle
     * @param impactParameter Impact parameter of light ray
     * @param sourceDistance Distance to light source
     * @return Deflection angle in radians
     */
    double CalculateLensingDeflection(double impactParameter, double sourceDistance) const;

    /**
     * @brief Calculate gravitational redshift
     * @param emissionRadius Radius where light was emitted
     * @param observerRadius Radius where light is observed
     * @return Redshift factor (z = (observed - emitted) / emitted)
     */
    double CalculateGravitationalRedshift(double emissionRadius, double observerRadius) const;

    /**
     * @brief Calculate Doppler redshift for moving source
     * @param velocity Source velocity relative to observer
     * @param angle Angle between velocity and line of sight
     * @return Doppler redshift factor
     */
    double CalculateDopplerRedshift(double velocity, double angle) const;

    /**
     * @brief Calculate effective potential for circular orbits
     * @param r Radial distance
     * @param angularMomentum Angular momentum per unit mass
     * @return Effective potential
     */
    double CalculateEffectivePotential(double r, double angularMomentum) const;

    /**
     * @brief Find circular orbit radius for given angular momentum
     * @param angularMomentum Angular momentum per unit mass
     * @return Orbital radius (0 if no stable orbit exists)
     */
    double FindCircularOrbitRadius(double angularMomentum) const;

    /**
     * @brief Check if orbit is stable
     * @param r Orbital radius
     * @param angularMomentum Angular momentum per unit mass
     * @return True if orbit is stable
     */
    bool IsOrbitStable(double r, double angularMomentum) const;

    /**
     * @brief Calculate conserved quantities for geodesic
     * @param state Current geodesic state
     * @return [Energy, Angular momentum, Carter constant] (Carter = 0 for Schwarzschild)
     */
    std::array<double, 3> CalculateConservedQuantities(const std::array<double, 8>& state) const;

    /**
     * @brief Get current black hole reference
     * @return Shared pointer to black hole
     */
    std::shared_ptr<BlackHole> GetBlackHole() const { return m_blackHole; }

private:
    std::shared_ptr<BlackHole> m_blackHole;     ///< Reference to black hole
    IntegrationMethod m_integrationMethod;      ///< Current integration method
    double m_tolerance;                         ///< Integration tolerance
    double m_maxStepSize;                       ///< Maximum integration step size
    double m_minStepSize;                       ///< Minimum integration step size

    /**
     * @brief Integrate geodesic using Runge-Kutta 4th order
     * @param state Current state (input/output)
     * @param derivatives Function to calculate derivatives
     * @param stepSize Integration step size
     */
    void IntegrateRK4(std::array<double, 8>& state,
                      std::function<void(const std::array<double, 8>&, std::array<double, 8>&)> derivatives,
                      double stepSize) const;

    /**
     * @brief Integrate geodesic using adaptive Runge-Kutta 4/5
     * @param state Current state (input/output)
     * @param derivatives Function to calculate derivatives
     * @param stepSize Current step size (input/output)
     * @return True if step was successful
     */
    bool IntegrateAdaptiveRK45(std::array<double, 8>& state,
                               std::function<void(const std::array<double, 8>&, std::array<double, 8>&)> derivatives,
                               double& stepSize) const;

    /**
     * @brief Calculate Christoffel symbols for Schwarzschild metric
     * @param r Radial coordinate
     * @param theta Polar angle
     * @return Christoffel symbols (only non-zero components)
     */
    struct ChristoffelSymbols {
        double Gamma_t_tr, Gamma_t_rt;           // Time-radial coupling
        double Gamma_r_tt, Gamma_r_rr, Gamma_r_theta_theta, Gamma_r_phi_phi;  // Radial components
        double Gamma_theta_r_theta, Gamma_theta_theta_r, Gamma_theta_phi_phi; // Theta components
        double Gamma_phi_r_phi, Gamma_phi_phi_r, Gamma_phi_theta_phi, Gamma_phi_phi_theta; // Phi components
    };

    ChristoffelSymbols CalculateChristoffelSymbols(double r, double theta) const;

    /**
     * @brief Validate geodesic state for numerical stability
     * @param state Geodesic state to validate
     * @return True if state is valid
     */
    bool ValidateGeodesicState(const std::array<double, 8>& state) const;
};

} // namespace BlackHoleSim