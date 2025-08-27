# Black Hole Simulation Physics Model

This document outlines the mathematical foundation and physics models used in the black hole simulation, focusing on the Schwarzschild metric for non-rotating black holes.

## Table of Contents

1. [Overview](#overview)
2. [Schwarzschild Metric](#schwarzschild-metric)
3. [Geodesic Equations](#geodesic-equations)
4. [Gravitational Lensing](#gravitational-lensing)
5. [Accretion Disk Physics](#accretion-disk-physics)
6. [Numerical Methods](#numerical-methods)
7. [Physical Constants](#physical-constants)
8. [References](#references)

## Overview

The simulation models the spacetime geometry around a non-rotating (Schwarzschild) black hole using Einstein's General Theory of Relativity. The key physical phenomena modeled include:

- **Geodesic motion** of massive particles and photons
- **Gravitational time dilation** and redshift effects
- **Gravitational lensing** of light rays
- **Accretion disk dynamics** and emission
- **Event horizon** and photon sphere boundaries

## Schwarzschild Metric

### Metric Tensor

The Schwarzschild metric describes the curvature of spacetime around a spherically symmetric, non-rotating massive object. In spherical coordinates (t, r, θ, φ), the line element is:

```
ds² = -(1 - rs/r)c²dt² + (1 - rs/r)⁻¹dr² + r²dθ² + r²sin²θdφ²
```

Where:
- `rs = 2GM/c²` is the **Schwarzschild radius** (event horizon)
- `G` is the gravitational constant
- `M` is the black hole mass
- `c` is the speed of light

### Metric Components

The non-zero components of the metric tensor are:

```
g_tt = -(1 - rs/r)
g_rr = (1 - rs/r)⁻¹
g_θθ = r²
g_φφ = r²sin²θ
```

### Key Radii

1. **Schwarzschild Radius (Event Horizon)**: `rs = 2GM/c²`
   - Point of no return for any object or light
   - Coordinate singularity (not physical singularity)

2. **Photon Sphere**: `r_ph = 3GM/c² = 1.5rs`
   - Unstable circular orbits for photons
   - Critical radius for gravitational lensing

3. **Innermost Stable Circular Orbit (ISCO)**: `r_isco = 6GM/c² = 3rs`
   - Innermost stable orbit for massive particles
   - Important for accretion disk physics

### Time Dilation

The gravitational time dilation factor is:

```
α = √(1 - rs/r)
```

Time runs slower closer to the black hole, approaching zero at the event horizon.

### Gravitational Redshift

For light emitted at radius `r_source` and observed at `r_observer`:

```
z = √((1 - rs/r_observer)/(1 - rs/r_source)) - 1
```

Where `z` is the redshift parameter.

## Geodesic Equations

### General Form

Geodesics represent the paths of freely falling objects in curved spacetime. The geodesic equation is:

```
d²x^μ/dτ² + Γ^μ_νλ (dx^ν/dτ)(dx^λ/dτ) = 0
```

Where:
- `x^μ` are the spacetime coordinates
- `τ` is the proper time
- `Γ^μ_νλ` are the Christoffel symbols

### Christoffel Symbols

Key non-zero Christoffel symbols for the Schwarzschild metric:

```
Γ^t_tr = rs/(2r(r-rs))
Γ^r_tt = rs(r-rs)/(2r³)
Γ^r_rr = -rs/(2r(r-rs))
Γ^r_θθ = -(r-rs)
Γ^r_φφ = -(r-rs)sin²θ
Γ^θ_rθ = 1/r
Γ^θ_φφ = -sinθ cosθ
Γ^φ_rφ = 1/r
Γ^φ_θφ = cotθ
```

### Conserved Quantities

Due to the symmetries of the Schwarzschild metric, certain quantities are conserved along geodesics:

1. **Energy per unit mass**: `E = (1 - rs/r)(dt/dτ)`
2. **Angular momentum per unit mass**: `L = r²sin²θ(dφ/dτ)`

### Effective Potential

For radial motion in the equatorial plane (θ = π/2), the effective potential is:

**For massive particles**:
```
V_eff = -GM/r + L²/(2r²) - GML²/(c²r³)
```

**For photons**:
```
V_eff = (1 - rs/r) × b²/r²
```

Where `b` is the impact parameter.

### Equations of Motion

For massive particles in the equatorial plane:

```
dt/dτ = E/(1 - rs/r)
dφ/dτ = L/r²
(dr/dτ)² = E² - (1 - rs/r)(1 + L²/r²)
```

For photons:

```
dt/dλ = E/(1 - rs/r)
dφ/dλ = L/r²
(dr/dλ)² = E² - (1 - rs/r)(L²/r²)
```

Where `λ` is an affine parameter along the photon path.

## Gravitational Lensing

### Light Ray Trajectories

Light rays follow null geodesics in curved spacetime. For the Schwarzschild metric, the geodesic equations for photons are:

```
d²t/dλ² + Γᵗₘᵥ (dxᵐ/dλ)(dxᵥ/dλ) = 0
d²r/dλ² + Γʳₘᵥ (dxᵐ/dλ)(dxᵥ/dλ) = 0
d²θ/dλ² + Γᶿₘᵥ (dxᵐ/dλ)(dxᵥ/dλ) = 0
d²φ/dλ² + Γᶠₘᵥ (dxᵐ/dλ)(dxᵥ/dλ) = 0
```

where λ is an affine parameter along the light ray.

### Conserved Quantities

For photons in Schwarzschild spacetime, we have two conserved quantities:

**Energy per photon:**
```
E = (1 - rs/r) dt/dλ
```

**Angular momentum per photon:**
```
L = r² sin²θ dφ/dλ
```

### Impact Parameter

The impact parameter b relates the conserved quantities:
```
b = L/E
```

For a photon at radius r with radial velocity dr/dλ:
```
(dr/dλ)² = E² - (1 - rs/r)(L²/r² + 1)
```

### Light Ray Deflection

The deflection angle for a light ray with impact parameter `b` is:

```
α ≈ 4GM/(bc²) + 15πG²M²/(4b²c⁴) + ...
```

The first term is the Einstein deflection, the second is the post-Newtonian correction.

For the exact solution, the deflection angle involves elliptic integrals:
```
α = 2∫[r₀ to ∞] dr / (r²√[(E²/L²) - (1-rs/r)(1/r² + 1/L²)])
```

where r₀ is the closest approach distance.

### Critical Impact Parameter

For strong lensing near the photon sphere:

```
b_crit = 3√3 GM/c² = 1.5√3 rs
```

**Photon Sphere:** Circular photon orbits exist at r = 3rs/2

**Einstein Ring:** When source, lens, and observer are aligned, the deflection creates a ring with angular radius:
```
θE = √(4GM DLS / (c² DL DS))
```
where DL, DS, DLS are lens-observer, source-observer, and lens-source distances.

### Lensing Cross-Section

The effective cross-section for strong lensing is:

```
σ_lens = π × (3√3 GM/c²)²
```

### Ray-Tracing Algorithm

1. Initialize photon position and direction
2. Calculate conserved quantities E and L
3. Integrate geodesic equations using RK4 or adaptive methods
4. Track photon until escape or absorption
5. Apply gravitational redshift corrections

## Accretion Disk Physics

### Shakura-Sunyaev Model

The standard thin disk model assumes:

1. **Surface density**: `Σ(r) ∝ r^(-3/4)` (for r >> rs)
2. **Temperature**: `T(r) ∝ r^(-3/4)`
3. **Viscosity parameter**: `α ~ 0.1`

The standard thin disk model with viscosity parameter α:

**Temperature Profile:**
```
T(r) = [3GM Ṁ / (8πσr³)]^(1/4) × [1 - √(3rs/r)]^(1/4)
```

**Surface Density:**
```
Σ(r) = Ṁ / (3πα H cs)
```

where H is the disk scale height and cs is the sound speed.

**Scale Height:**
```
H = cs / Ω = √(kT r³ / GMmp)
```

### Temperature Profile

For a thin accretion disk:

```
T(r) = T₀ × (r/r_in)^(-3/4) × [1 - (r_in/r)^(1/2)]^(1/4)
```

Where `r_in` is the inner disk radius (typically 3rs).

### Novikov-Thorne Model

More accurate relativistic thin disk model:

**Energy Flux:**
```
F(r) = -Ṁ dE/dr / (4πr √(-gtt grr))
```

**Temperature:**
```
T(r) = [F(r) / σ]^(1/4)
```

### Luminosity

The total luminosity from accretion is:

```
L = η × Ṁ × c²
```

Where:
- `η ≈ 0.06` is the radiative efficiency for a Schwarzschild black hole
- `Ṁ` is the mass accretion rate

### Emission Properties

**Blackbody Spectrum:**
```
B(ν,T) = (2hν³/c²) / [exp(hν/kT) - 1]
```

**Total Luminosity:**
```
L = ∫∫ 2σT⁴(r,φ) r dr dφ
```

**Efficiency:**
```
η = L / (Ṁc²) ≈ 0.057 (for Schwarzschild)
```

### Emission Spectrum

The disk emission follows a modified blackbody spectrum with relativistic corrections:

```
I(ν,r) = (2hν³/c²) × 1/(exp(hν/kT(r)) - 1) × g(r)
```

Where `g(r)` includes gravitational redshift and Doppler effects.

## Numerical Methods

### Geodesic Integration

The simulation uses adaptive Runge-Kutta methods for integrating geodesic equations:

1. **RK4 (Runge-Kutta 4th order)**: Standard fixed-step integration
2. **RK45 (Runge-Kutta-Fehlberg)**: Adaptive step size control
3. **Dormand-Prince**: Higher-order adaptive method

**Runge-Kutta 4th Order (RK4):**
```
k₁ = h f(yₙ)
k₂ = h f(yₙ + k₁/2)
k₃ = h f(yₙ + k₂/2)
k₄ = h f(yₙ + k₃)
yₙ₊₁ = yₙ + (k₁ + 2k₂ + 2k₃ + k₄)/6
```

**Adaptive Runge-Kutta-Fehlberg (RKF45):**
- 5th order accurate solution with 4th order error estimate
- Automatic step size control based on error tolerance
- Optimal for long-term orbital integration

**Conservation Monitoring:**
- Energy conservation: |E(t) - E(0)| / E(0) < εₑ
- Angular momentum: |L(t) - L(0)| / L(0) < εₗ
- Typical tolerance: ε ~ 10⁻⁸

### Step Size Control

Adaptive step size based on local truncation error:

```
Δτ_new = Δτ_old × (tolerance/error)^(1/5)
```

### Coordinate Systems

1. **Schwarzschild coordinates**: Standard (t, r, θ, φ)
2. **Cartesian coordinates**: For visualization (x, y, z)
3. **Tortoise coordinates**: For numerical stability near horizon

**Schwarzschild Coordinates (t,r,θ,φ):**
- Natural for static observers
- Coordinate singularity at r = rs

**Cartesian Conversion:**
```
x = r sin θ cos φ
y = r sin θ sin φ
z = r cos θ
```

**Velocity Transformation:**
```
vₓ = (∂x/∂r)vᵣ + (∂x/∂θ)vθ + (∂x/∂φ)vφ
```

## Physical Constants

### Fundamental Constants

```cpp
const double G = 6.67430e-11;     // Gravitational constant (m³/kg⋅s²)
const double c = 299792458.0;     // Speed of light (m/s)
const double h = 6.62607015e-34;  // Planck constant (J⋅s)
const double k_B = 1.380649e-23;  // Boltzmann constant (J/K)
const double σ_SB = 5.670374419e-8; // Stefan-Boltzmann (W⋅m⁻²⋅K⁻⁴)
const double m_p = 1.67262192369e-27; // Proton mass (kg)
const double M_sun = 1.989e30;   // Solar mass (kg)
```

### Derived Quantities

**Schwarzschild Radius:**
```
rs = 2GM/c²
```

**Photon Sphere:**
```
rph = 3rs/2 = 3GM/c²
```

**Innermost Stable Circular Orbit (ISCO):**
```
risco = 3rs = 6GM/c²
```

**Gravitational Time Dilation:**
```
dt_proper = dt_coordinate × √(1 - rs/r)
```

**Escape Velocity:**
```
vesc = c√(rs/r) = √(2GM/r)
```

### Typical Scales

For a solar mass black hole:
- **Schwarzschild radius**: `rs ≈ 3.0 km`
- **Photon sphere**: `r_ph ≈ 4.5 km`
- **ISCO**: `r_isco ≈ 9.0 km`
- **Light crossing time**: `rs/c ≈ 10 μs`

## Implementation Notes

### Numerical Stability

1. **Coordinate singularities**: Use appropriate coordinate transformations
2. **Step size limits**: Prevent integration from becoming unstable
3. **Conservation checks**: Monitor energy and angular momentum conservation

### Performance Optimizations

1. **Vectorization**: Use SIMD instructions for parallel calculations
2. **GPU acceleration**: Offload ray tracing to compute shaders
3. **Adaptive quality**: Reduce precision for distant objects

### Validation Tests

1. **Circular orbits**: Verify stable circular motion at ISCO
2. **Light deflection**: Compare with analytical predictions
3. **Energy conservation**: Check conservation to machine precision
4. **Redshift calculations**: Validate against known solutions

## References

1. **Misner, C. W., Thorne, K. S., & Wheeler, J. A.** (1973). *Gravitation*. W. H. Freeman.
2. **Chandrasekhar, S.** (1983). *The Mathematical Theory of Black Holes*. Oxford University Press.
3. **Shakura, N. I., & Sunyaev, R. A.** (1973). Black holes in binary systems. *Astronomy and Astrophysics*, 24, 337-355.
4. **Novikov, I. D., & Thorne, K. S.** (1973). Astrophysics of black holes. *Black Holes*, 343-450.
5. **Bardeen, J. M., Press, W. H., & Teukolsky, S. A.** (1972). Rotating black holes. *Astrophysical Journal*, 178, 347-370.
6. **Luminet, J. P.** (1979). Image of a spherical black hole with thin accretion disk. *Astronomy and Astrophysics*, 75, 228-235.
7. **Event Horizon Telescope Collaboration** (2019). First M87 Event Horizon Telescope Results. *Astrophysical Journal Letters*, 875, L1.

## Appendix: Derivations

### A.1 Schwarzschild Metric Derivation

Starting from the Einstein field equations:
```
G_μν = 8πG/c⁴ × T_μν
```

For vacuum (T_μν = 0) with spherical symmetry, the solution is:
```
ds² = -f(r)c²dt² + h(r)dr² + r²dΩ²
```

Where dΩ² = dθ² + sin²θdφ² and the functions f(r) and h(r) are determined by the field equations.

### A.2 Geodesic Equation Derivation

From the principle of extremal proper time:
```
δ∫dτ = 0
```

Using the calculus of variations leads to the geodesic equation.

### A.3 Effective Potential Derivation

From the conserved quantities and the normalization condition for massive particles:
```
g_μν (dx^μ/dτ)(dx^ν/dτ) = -1
```

This leads to the effective potential formulation for radial motion.

---

*This document provides the mathematical foundation for the black hole simulation. For implementation details, see the source code documentation.*