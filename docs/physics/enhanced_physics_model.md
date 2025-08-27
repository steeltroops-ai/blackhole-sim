# Enhanced Black Hole Physics Model - Complete Implementation Guide

This document provides the comprehensive mathematical foundations for implementing a visually stunning and scientifically accurate black hole simulation.

## 🌌 **Core Physics Equations**

### **Schwarzschild Metric**
```
ds² = -(1 - Rs/r)c²dt² + (1 - Rs/r)⁻¹dr² + r²(dθ² + sin²θ dφ²)
```

**Key Radii:**
- **Event Horizon**: r = Rs = 2GM/c² ≈ 3.0 km × (M/M☉)
- **Photon Sphere**: r = 1.5Rs = 3GM/c²
- **ISCO**: r = 6GM/c² (innermost stable circular orbit)

### **Time Dilation Formula**
```cpp
double timeDilation(double r, double Rs) {
    return 1.0 / sqrt(1.0 - Rs/r);
}
```

### **Gravitational Redshift**
```cpp
double gravitationalRedshift(double r, double Rs) {
    return 1.0 / sqrt(1.0 - Rs/r) - 1.0;
}
```

## 🎯 **Visual Implementation Equations**

### **Accretion Disk Temperature**
```cpp
double diskTemperature(double r, double Rs) {
    double r_isco = 6.0 * Rs;
    if (r < r_isco) return 0.0; // No stable orbits inside ISCO
    
    double T0 = 1e6; // Base temperature in Kelvin
    double temp = T0 * pow(Rs/r, 0.75) * pow(1.0 - sqrt(3.0*Rs/r), 0.25);
    return temp;
}
```

### **Color Temperature Mapping**
```cpp
Vector3 temperatureToColor(double T) {
    if (T < 3000) return Vector3(1.0, 0.0, 0.0);      // Red
    if (T < 5000) return Vector3(1.0, 0.5, 0.0);      // Orange  
    if (T < 7000) return Vector3(1.0, 1.0, 0.0);      // Yellow
    if (T < 10000) return Vector3(1.0, 1.0, 1.0);     // White
    return Vector3(0.5, 0.8, 1.0);                    // Blue-white
}
```

### **Spacetime Curvature Visualization**
```cpp
double spacetimeCurvature(double x, double z, double Rs) {
    double r = sqrt(x*x + z*z);
    if (r < Rs) return -1000.0; // Inside event horizon
    
    // Curvature falls off as 1/r²
    double curvature = -Rs * Rs / (r * r);
    return curvature * 50.0; // Scale for visibility
}
```

## 🌀 **Particle Trajectory Equations**

### **Effective Potential**
```cpp
double effectivePotential(double r, double L, double Rs) {
    double term1 = -(1.0 - Rs/r);
    double term2 = L*L / (r*r);
    return term1 * (1.0 + term2);
}
```

### **Radial Equation of Motion**
```cpp
double radialAcceleration(double r, double rdot, double L, double Rs) {
    double r2 = r * r;
    double r3 = r2 * r;
    
    // Schwarzschild geodesic equation
    double term1 = -Rs / (2.0 * r2 * (1.0 - Rs/r));
    double term2 = rdot * rdot;
    double term3 = (1.0 - Rs/r) * L*L / r3;
    
    return term1 * term2 + term3;
}
```

### **Orbital Classification**
```cpp
enum OrbitType {
    STABLE_CIRCULAR,    // r > 6Rs, circular orbits
    ELLIPTICAL,         // r > 6Rs, bound elliptical
    UNSTABLE,           // 3Rs < r < 6Rs, unstable
    PLUNGE,             // r < 3Rs, direct infall
    ESCAPE              // Unbound, hyperbolic
};

OrbitType classifyOrbit(double r, double E, double L, double Rs) {
    double r_isco = 6.0 * Rs;
    double r_photon = 1.5 * Rs;
    
    if (r < r_photon) return PLUNGE;
    if (r < r_isco) return UNSTABLE;
    
    double Veff = effectivePotential(r, L, Rs);
    if (E < Veff) return STABLE_CIRCULAR;
    if (E < 0) return ELLIPTICAL;
    return ESCAPE;
}
```

## 💫 **Light Ray Deflection**

### **Impact Parameter**
```cpp
double impactParameter(double x0, double z0, double vx, double vz) {
    // For light ray starting at (x0, z0) with velocity (vx, vz)
    return abs(x0 * vz - z0 * vx) / sqrt(vx*vx + vz*vz);
}
```

### **Deflection Angle**
```cpp
double lightDeflection(double b, double Rs) {
    if (b < 1.5 * Rs) return M_PI; // Light captured
    
    // Einstein's formula for weak field
    double deflection = 4.0 * Rs / b;
    
    // Strong field correction
    if (b < 10.0 * Rs) {
        double u = Rs / b;
        deflection *= (1.0 + 15.0 * u*u / 4.0); // Higher order terms
    }
    
    return deflection;
}
```

## 🎨 **Visual Enhancement Formulas**

### **Glow Intensity**
```cpp
double glowIntensity(double distance, double maxDistance) {
    double normalized = distance / maxDistance;
    return exp(-normalized * normalized * 4.0); // Gaussian falloff
}
```

### **Particle Trail Fade**
```cpp
double trailFade(double age, double maxAge) {
    double t = age / maxAge;
    return (1.0 - t) * exp(-t * 2.0); // Exponential fade
}
```

### **Animation Phase**
```cpp
double animationPhase(double time, double period) {
    return fmod(time, period) / period * 2.0 * M_PI;
}
```

## 🔢 **Numerical Integration (RK4)**

### **RK4 Step for Geodesics**
```cpp
struct State {
    double r, rdot, phi, phidot;
};

State rk4Step(State y, double dt, double L, double Rs) {
    auto f = [L, Rs](State s) -> State {
        return {
            s.rdot,
            radialAcceleration(s.r, s.rdot, L, Rs),
            s.phidot,
            -2.0 * s.rdot * s.phidot / s.r
        };
    };
    
    State k1 = f(y);
    State k2 = f(y + k1 * (dt/2));
    State k3 = f(y + k2 * (dt/2));
    State k4 = f(y + k3 * dt);
    
    return y + (k1 + k2*2 + k3*2 + k4) * (dt/6);
}
```

## 📊 **Physical Constants**

```cpp
namespace Physics {
    const double G = 6.67430e-11;        // m³/kg·s²
    const double c = 2.99792458e8;        // m/s
    const double M_solar = 1.989e30;      // kg
    const double h = 6.62607015e-34;      // J·s
    const double k_B = 1.380649e-23;      // J/K
    
    // Simulation parameters
    const double M_blackhole = 10 * M_solar;
    const double Rs = 2 * G * M_blackhole / (c * c);
    
    // Conversion factors
    const double km_to_Rs = 1000.0 / Rs;
    const double second_to_Rs_over_c = c / Rs;
}
```

## 🎯 **Implementation Priorities**

### **Phase 1: Core Visibility**
1. **Event Horizon**: Black sphere with glowing edge
2. **Accretion Disk**: Temperature-colored rotating disk
3. **Particles**: Bright, clearly visible spheres

### **Phase 2: Physics Effects**
1. **Spacetime Grid**: Curved grid showing warping
2. **Light Rays**: Bent light paths with proper deflection
3. **Orbital Motion**: Realistic particle trajectories

### **Phase 3: Visual Polish**
1. **Bloom Effects**: Glowing halos around bright objects
2. **Particle Trails**: Fading trails showing motion history
3. **Color Gradients**: Smooth temperature transitions

### **Phase 4: Interactivity**
1. **Camera Controls**: Free movement and orbital modes
2. **Layer Toggles**: Show/hide different elements
3. **Parameter Adjustment**: Real-time physics changes

This mathematical foundation ensures that every visual element corresponds to real physics while maintaining the anime-style aesthetic appeal.
