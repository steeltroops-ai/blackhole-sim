#pragma once

#include <array>
#include <cmath>

namespace BlackHoleSim {

/**
 * @brief Simple 4D vector class for color and homogeneous coordinates
 */
class Vector4 {
public:
    // Constructors
    Vector4() : m_data{0.0, 0.0, 0.0, 0.0} {}
    Vector4(double x, double y, double z, double w) : m_data{x, y, z, w} {}
    Vector4(const std::array<double, 4>& arr) : m_data{arr[0], arr[1], arr[2], arr[3]} {}
    
    // Element access
    double& operator[](int index) { return m_data[index]; }
    const double& operator[](int index) const { return m_data[index]; }
    
    double& x() { return m_data[0]; }
    double& y() { return m_data[1]; }
    double& z() { return m_data[2]; }
    double& w() { return m_data[3]; }
    
    const double& x() const { return m_data[0]; }
    const double& y() const { return m_data[1]; }
    const double& z() const { return m_data[2]; }
    const double& w() const { return m_data[3]; }
    
    // Vector operations
    Vector4 operator+(const Vector4& other) const {
        return Vector4(m_data[0] + other.m_data[0], 
                      m_data[1] + other.m_data[1], 
                      m_data[2] + other.m_data[2],
                      m_data[3] + other.m_data[3]);
    }
    
    Vector4 operator-(const Vector4& other) const {
        return Vector4(m_data[0] - other.m_data[0], 
                      m_data[1] - other.m_data[1], 
                      m_data[2] - other.m_data[2],
                      m_data[3] - other.m_data[3]);
    }
    
    Vector4 operator*(double scalar) const {
        return Vector4(m_data[0] * scalar, 
                      m_data[1] * scalar, 
                      m_data[2] * scalar,
                      m_data[3] * scalar);
    }
    
    Vector4 operator/(double scalar) const {
        return Vector4(m_data[0] / scalar, 
                      m_data[1] / scalar, 
                      m_data[2] / scalar,
                      m_data[3] / scalar);
    }
    
    Vector4& operator+=(const Vector4& other) {
        m_data[0] += other.m_data[0];
        m_data[1] += other.m_data[1];
        m_data[2] += other.m_data[2];
        m_data[3] += other.m_data[3];
        return *this;
    }
    
    Vector4& operator-=(const Vector4& other) {
        m_data[0] -= other.m_data[0];
        m_data[1] -= other.m_data[1];
        m_data[2] -= other.m_data[2];
        m_data[3] -= other.m_data[3];
        return *this;
    }
    
    Vector4& operator*=(double scalar) {
        m_data[0] *= scalar;
        m_data[1] *= scalar;
        m_data[2] *= scalar;
        m_data[3] *= scalar;
        return *this;
    }
    
    Vector4& operator/=(double scalar) {
        m_data[0] /= scalar;
        m_data[1] /= scalar;
        m_data[2] /= scalar;
        m_data[3] /= scalar;
        return *this;
    }
    
    // Dot product
    double Dot(const Vector4& other) const {
        return m_data[0] * other.m_data[0] + 
               m_data[1] * other.m_data[1] + 
               m_data[2] * other.m_data[2] +
               m_data[3] * other.m_data[3];
    }
    
    // Magnitude
    double Magnitude() const {
        return std::sqrt(m_data[0] * m_data[0] + 
                        m_data[1] * m_data[1] + 
                        m_data[2] * m_data[2] +
                        m_data[3] * m_data[3]);
    }
    
    double MagnitudeSquared() const {
        return m_data[0] * m_data[0] + 
               m_data[1] * m_data[1] + 
               m_data[2] * m_data[2] +
               m_data[3] * m_data[3];
    }
    
    // Normalized vector
    Vector4 Normalized() const {
        double mag = Magnitude();
        if (mag > 0.0) {
            return *this / mag;
        }
        return Vector4(0.0, 0.0, 0.0, 0.0);
    }
    
    // Normalize in place
    void Normalize() {
        double mag = Magnitude();
        if (mag > 0.0) {
            *this /= mag;
        }
    }
    
    // Convert to array
    std::array<double, 4> ToArray() const {
        return m_data;
    }
    
private:
    std::array<double, 4> m_data;
};

// Global operators
inline Vector4 operator*(double scalar, const Vector4& vec) {
    return vec * scalar;
}

} // namespace BlackHoleSim