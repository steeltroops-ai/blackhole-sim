# Black Hole Simulation - Physics Design Document

## Overview

This document provides a comprehensive overview of the physics models, equations, assumptions, and implementation details for the black hole simulation. The simulation focuses on Schwarzschild black holes (non-rotating) and includes gravitational lensing, particle dynamics, and accretion disk physics.

## Table of Contents

1. [Fundamental Physics](#fundamental-physics)
2. [Schwarzschild Metric](#schwarzschild-metric)
3. [Geodesic Equations](#geodesic-equations)
4. [Gravitational Lensing](#gravitational-lensing)
5. [Particle Dynamics](#particle-dynamics)
6. [Accretion Disk Physics](#accretion-disk-physics)
7. [Numerical Methods](#numerical-methods)
8. [Physical Constants](#physical-constants)
9. [Assumptions and Limitations](#assumptions-and-limitations)
10. [Implementation Details](#implementation-details)
11. [Validation and Testing](#validation-and-testing)
12. [References](#references)

## Fundamental Physics

### General Relativity Basics

The simulation is based on Einstein's General Theory of Relativity, where gravity is described as the curvature of spacetime. The fundamental equation is:

```
G_μν = 8πT_μν
```

Where:
- `G_μν` is the Einstein tensor (describes spacetime curvature)
- `T_μν` is the stress-energy tensor (describes matter and energy)
- Units are chosen such that `G = c = 1`

### Coordinate Systems

The simulation uses multiple coordinate systems:

1. **Schwarzschild Coordinates** (r, θ, φ, t)
   - Natural for the black hole metric
   - Singular at the event horizon

2. **Cartesian Coordinates** (x, y, z)
   - Used for visualization and user interaction
   - Converted to/from spherical as needed

3. **Proper Time** (τ)
   - Used for massive particle trajectories
   - Related to coordinate time by time dilation

## Schwarzschild Metric

### Metric Tensor

For a non-rotating black hole of mass M, the Schwarzschild metric in natural units (G = c = 1) is:

```
ds² = -(1 - 2M/r)dt² + (1 - 2M/r)⁻¹dr² + r²(dθ² + sin²θ dφ²)
```

### Key Radii

1. **Schwarzschild Radius (Event Horizon)**:
   ```
   r_s = 2M = 2GM/c²
   ```

2. **Photon Sphere**:
   ```
   r_ph = 3M = 1.5 × r_s
   ```

3. **Innermost Stable Circular Orbit (ISCO)**:
   ```
   r_isco = 6M = 3 × r_s
   ```

### Metric Components

In matrix form:
```
g_μν = diag[-(1-2M/r), (1-2M/r)⁻¹, r², r²sin²θ]
```

### Derived Quantities

1. **Gravitational Potential**:
   ```
   Φ(r) = -M/r
   ```

2. **Time Dilation Factor**:
   ```
   √(g_tt) = √(1 - 2M/r)
   ```

3. **Gravitational Redshift**:
   ```
   z = √(g_tt(r_observer)/g_tt(r_source)) - 1
   ```

## Geodesic Equations

### General Form

Particles and light rays follow geodesics in curved spacetime:

```
d²x^μ/dλ² + Γ^μ_αβ (dx^α/dλ)(dx^β/dλ) = 0
```

Where:
- `λ` is an affine parameter (proper time for massive particles)
- `Γ^μ_αβ` are Christoffel symbols

### Christoffel Symbols

For the Schwarzschild metric, the non-zero Christoffel symbols are:

```
Γ^t_tr = Γ^t_rt = M/(r²(1-2M/r))
Γ^r_tt = M(1-2M/r)/r²
Γ^r_rr = -M/(r²(1-2M/r))
Γ^r_θθ = -(r-2M)
Γ^r_φφ = -(r-2M)sin²θ
Γ^θ_rθ = Γ^θ_θr = 1/r
Γ^θ_φφ = -sinθ cosθ
Γ^φ_rφ = Γ^φ_φr = 1/r
Γ^φ_θφ = Γ^φ_φθ = cotθ
```

### Conserved Quantities

Due to symmetries of the Schwarzschild metric:

1. **Energy** (time translation symmetry):
   ```
   E = (1 - 2M/r)(dt/dλ)
   ```

2. **Angular Momentum** (rotational symmetry):
   ```
   L = r²sin²θ(dφ/dλ)
   ```

### Effective Potential

#### For Massive Particles:
```
V_eff(r) = (1 - 2M/r)(1 + L²/r²)
```

#### For Photons:
```
V_eff(r) = (1 - 2M/r)(L²/r²)
```

## Gravitational Lensing

### Light Ray Trajectories

Photons follow null geodesics with the constraint:
```
ds² = 0
```

This leads to the orbit equation:
```
(du/dφ)² = (1/b²) - u²(1 - 2Mu)
```

Where:
- `u = 1/r`
- `b = L/E` is the impact parameter

### Critical Phenomena

1. **Photon Sphere Capture**: `b < b_crit = 3√3 M`
2. **Strong Lensing**: Multiple images for `b ≈ b_crit`
3. **Weak Lensing**: Small deflections for `b >> M`

### Deflection Angle

For weak lensing (r >> M):
```
α ≈ 4M/b
```

For strong lensing, numerical integration is required.

### Einstein Ring

When source, lens, and observer are aligned:
```
θ_E = √(4M D_ls/(D_l D_s))
```

Where D_l, D_s, D_ls are angular diameter distances.

## Particle Dynamics

### Massive Particle Motion

Massive particles follow timelike geodesics with:
```
g_μν (dx^μ/dτ)(dx^ν/dτ) = -1
```

### Orbital Types

1. **Circular Orbits**:
   - Stable: r > 6M
   - Unstable: 3M < r < 6M
   - No circular orbits: r < 3M

2. **Elliptical Orbits**:
   - Bound orbits with periapsis > 6M
   - Exhibit relativistic precession

3. **Parabolic/Hyperbolic**:
   - Unbound trajectories
   - Gravitational scattering

### Orbital Velocity

For circular orbits:
```
v_circ = √(M/r) / √(1 - 3M/r)
```

### Orbital Period

For circular orbits:
```
T = 2π√(r³/M) / (1 - 3M/r)^(3/2)
```

## Accretion Disk Physics

### Shakura-Sunyaev Model

Standard thin disk model with viscosity parameter α:

#### Surface Density:
```
Σ(r) = (3Ṁ/8π) × (1/√(GM r³)) × f(r)
```

Where f(r) accounts for relativistic corrections.

#### Temperature Profile:
```
T(r) = (3GMṀ/8πσr³)^(1/4) × g(r)^(1/4)
```

Where g(r) includes relativistic factors.

### Novikov-Thorne Model

Relativistic thin disk model:

#### Efficiency:
```
η = 1 - √(1 - 2/(3r_isco)) ≈ 0.057 for Schwarzschild
```

#### Radial Structure:
Includes proper relativistic treatment of:
- Frame dragging effects
- Redshift corrections
- Proper disk thickness

### Emission Properties

#### Blackbody Spectrum:
```
B_ν(T) = (2hν³/c²) / (exp(hν/kT) - 1)
```

#### Disk Luminosity:
```
L_disk = ηṀc² = ∫∫ σT⁴(r) dA
```

### Relativistic Effects

1. **Doppler Shift**:
   ```
   ν_obs = ν_em × (1 + β·n̂) / √(1 - β²)
   ```

2. **Gravitational Redshift**:
   ```
   ν_obs = ν_em × √(g_tt(r_obs)/g_tt(r_em))
   ```

3. **Light Bending**:
   - Ray tracing through curved spacetime
   - Multiple images possible

## Numerical Methods

### Geodesic Integration

#### Runge-Kutta 4th Order (RK4):
```
k1 = h × f(t, y)
k2 = h × f(t + h/2, y + k1/2)
k3 = h × f(t + h/2, y + k2/2)
k4 = h × f(t + h, y + k3)
y_{n+1} = y_n + (k1 + 2k2 + 2k3 + k4)/6
```

#### Adaptive Runge-Kutta-Fehlberg (RKF45):
- 4th and 5th order estimates
- Error control: `|y5 - y4| < tolerance`
- Adaptive step size adjustment

### Conservation Monitoring

For validation, monitor conserved quantities:
```
ΔE/E < 10⁻⁶
ΔL/L < 10⁻⁶
```

### Coordinate Transformations

#### Spherical to Cartesian:
```
x = r sin θ cos φ
y = r sin θ sin φ
z = r cos θ
```

#### Cartesian to Spherical:
```
r = √(x² + y² + z²)
θ = arccos(z/r)
φ = arctan2(y, x)
```

## Physical Constants

### Fundamental Constants
```
G = 6.67430 × 10⁻¹¹ m³ kg⁻¹ s⁻²    (Gravitational constant)
c = 2.99792458 × 10⁸ m s⁻¹           (Speed of light)
ℏ = 1.054571817 × 10⁻³⁴ J s          (Reduced Planck constant)
k_B = 1.380649 × 10⁻²³ J K⁻¹         (Boltzmann constant)
σ = 5.670374419 × 10⁻⁸ W m⁻² K⁻⁴    (Stefan-Boltzmann constant)
```

### Derived Constants
```
M_sun = 1.98847 × 10³⁰ kg             (Solar mass)
r_s_sun = 2.95 km                     (Solar Schwarzschild radius)
t_P = √(ℏG/c⁵) = 5.39 × 10⁻⁴⁴ s      (Planck time)
l_P = √(ℏG/c³) = 1.62 × 10⁻³⁵ m      (Planck length)
```

### Natural Units
In the simulation, we often use natural units where G = c = 1:
- Mass has units of length
- Time has units of length
- Energy has units of inverse length

## Assumptions and Limitations

### Assumptions

1. **Schwarzschild Geometry**:
   - Non-rotating black hole
   - Spherically symmetric
   - Vacuum solution (no external fields)

2. **Test Particle Approximation**:
   - Particles don't affect spacetime geometry
   - No particle-particle interactions
   - Point particles (no finite size effects)

3. **Classical General Relativity**:
   - No quantum effects
   - No Hawking radiation
   - No information paradox considerations

4. **Thin Disk Model**:
   - Geometrically thin (H << R)
   - Optically thick
   - Local thermodynamic equilibrium

### Limitations

1. **No Rotation**: Kerr metric not implemented
2. **No Magnetic Fields**: Pure hydrodynamic disk
3. **No Radiation Pressure**: Gas pressure dominated
4. **No Disk Instabilities**: Steady-state assumption
5. **No Relativistic Jets**: No magnetohydrodynamics

### Numerical Limitations

1. **Coordinate Singularities**: Care needed at r = 2M
2. **Stiff Equations**: Near-horizon dynamics challenging
3. **Floating Point Precision**: Limited accuracy for extreme ratios
4. **Integration Errors**: Accumulate over long times

## Implementation Details

### Class Structure

1. **BlackHole**: Metric calculations, physical properties
2. **Particle**: Massive particle dynamics, orbital mechanics
3. **LightRay**: Photon trajectories, lensing effects
4. **AccretionDisk**: Disk physics, emission properties
5. **PhysicsEngine**: Integration, coordinate transforms

### Key Algorithms

1. **Geodesic Integration**:
   - RK4 for standard accuracy
   - RKF45 for adaptive stepping
   - Conservation monitoring

2. **Ray Tracing**:
   - Backward ray tracing from observer
   - Adaptive step size near photon sphere
   - Termination conditions (horizon, infinity)

3. **Disk Rendering**:
   - Temperature-dependent emission
   - Relativistic Doppler effects
   - Gravitational lensing

### Performance Optimizations

1. **Vectorization**: SIMD operations where possible
2. **Caching**: Pre-compute metric components
3. **Adaptive Grids**: Higher resolution near black hole
4. **Parallel Processing**: Multi-threaded particle updates

## Validation and Testing

### Analytical Tests

1. **Circular Orbits**:
   - Verify orbital velocities
   - Check stability conditions
   - Validate periods

2. **Light Deflection**:
   - Weak field limit (α = 4M/b)
   - Strong field numerical results
   - Critical impact parameters

3. **Redshift Calculations**:
   - Gravitational redshift formula
   - Doppler shift verification
   - Combined effects

### Numerical Tests

1. **Conservation Laws**:
   - Energy conservation (ΔE/E < 10⁻⁶)
   - Angular momentum (ΔL/L < 10⁻⁶)
   - Proper time parameterization

2. **Integration Accuracy**:
   - Step size convergence
   - Method comparison (Euler vs RK4 vs RKF45)
   - Long-term stability

3. **Coordinate Transformations**:
   - Round-trip accuracy
   - Singularity handling
   - Numerical stability

### Physical Validation

1. **Known Solutions**:
   - Radial free fall
   - Circular orbits
   - Photon sphere trajectories

2. **Limiting Cases**:
   - Newtonian limit (r >> M)
   - Post-Newtonian corrections
   - Ultra-relativistic regime

3. **Observational Comparisons**:
   - Black hole shadows
   - Accretion disk spectra
   - Gravitational lensing

## References

### Primary Sources

1. **Misner, C. W., Thorne, K. S., & Wheeler, J. A.** (1973). *Gravitation*. W. H. Freeman.

2. **Chandrasekhar, S.** (1983). *The Mathematical Theory of Black Holes*. Oxford University Press.

3. **Rindler, W.** (2006). *Relativity: Special, General, and Cosmological*. Oxford University Press.

### Specialized References

4. **Shakura, N. I., & Sunyaev, R. A.** (1973). Black holes in binary systems. *Astronomy and Astrophysics*, 24, 337-355.

5. **Novikov, I. D., & Thorne, K. S.** (1973). Astrophysics of black holes. *Black Holes*, 343-450.

6. **Bardeen, J. M., Press, W. H., & Teukolsky, S. A.** (1972). Rotating black holes: locally nonrotating frames, energy extraction, and scalar synchrotron radiation. *Astrophysical Journal*, 178, 347-370.

### Numerical Methods

7. **Press, W. H., et al.** (2007). *Numerical Recipes: The Art of Scientific Computing*. Cambridge University Press.

8. **Hairer, E., Nørsett, S. P., & Wanner, G.** (1993). *Solving Ordinary Differential Equations I: Nonstiff Problems*. Springer.

### Modern Applications

9. **Event Horizon Telescope Collaboration** (2019). First M87 Event Horizon Telescope Results. *Astrophysical Journal Letters*, 875, L1.

10. **LIGO Scientific Collaboration** (2016). Observation of Gravitational Waves from a Binary Black Hole Merger. *Physical Review Letters*, 116, 061102.

---

*This document serves as the authoritative reference for the physics implementation in the black hole simulation. All equations and methods described here are implemented in the corresponding C++ classes.*