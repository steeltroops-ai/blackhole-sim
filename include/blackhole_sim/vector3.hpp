#pragma once

#include <array>
#include <cmath>

namespace BlackHoleSim {

/**
 * @brief Simple 3D vector class for physics calculations
 */
class Vector3 {
public:
    // Constructors
    Vector3() : m_data{0.0, 0.0, 0.0} {}
    Vector3(double x, double y, double z) : m_data{x, y, z} {}
    Vector3(const std::array<double, 3>& arr) : m_data{arr[0], arr[1], arr[2]} {}
    
    // Element access
    double& operator[](int index) { return m_data[index]; }
    const double& operator[](int index) const { return m_data[index]; }
    
    double& x() { return m_data[0]; }
    double& y() { return m_data[1]; }
    double& z() { return m_data[2]; }
    
    const double& x() const { return m_data[0]; }
    const double& y() const { return m_data[1]; }
    const double& z() const { return m_data[2]; }
    
    // Vector operations
    Vector3 operator+(const Vector3& other) const {
        return Vector3(m_data[0] + other.m_data[0], 
                      m_data[1] + other.m_data[1], 
                      m_data[2] + other.m_data[2]);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(m_data[0] - other.m_data[0], 
                      m_data[1] - other.m_data[1], 
                      m_data[2] - other.m_data[2]);
    }
    
    Vector3 operator*(double scalar) const {
        return Vector3(m_data[0] * scalar, 
                      m_data[1] * scalar, 
                      m_data[2] * scalar);
    }
    
    Vector3 operator/(double scalar) const {
        return Vector3(m_data[0] / scalar, 
                      m_data[1] / scalar, 
                      m_data[2] / scalar);
    }
    
    Vector3& operator+=(const Vector3& other) {
        m_data[0] += other.m_data[0];
        m_data[1] += other.m_data[1];
        m_data[2] += other.m_data[2];
        return *this;
    }
    
    Vector3& operator-=(const Vector3& other) {
        m_data[0] -= other.m_data[0];
        m_data[1] -= other.m_data[1];
        m_data[2] -= other.m_data[2];
        return *this;
    }
    
    Vector3& operator*=(double scalar) {
        m_data[0] *= scalar;
        m_data[1] *= scalar;
        m_data[2] *= scalar;
        return *this;
    }
    
    Vector3& operator/=(double scalar) {
        m_data[0] /= scalar;
        m_data[1] /= scalar;
        m_data[2] /= scalar;
        return *this;
    }
    
    // Dot product
    double Dot(const Vector3& other) const {
        return m_data[0] * other.m_data[0] + 
               m_data[1] * other.m_data[1] + 
               m_data[2] * other.m_data[2];
    }
    
    // Cross product
    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            m_data[1] * other.m_data[2] - m_data[2] * other.m_data[1],
            m_data[2] * other.m_data[0] - m_data[0] * other.m_data[2],
            m_data[0] * other.m_data[1] - m_data[1] * other.m_data[0]
        );
    }
    
    // Magnitude
    double Magnitude() const {
        return std::sqrt(m_data[0] * m_data[0] + 
                        m_data[1] * m_data[1] + 
                        m_data[2] * m_data[2]);
    }
    
    double MagnitudeSquared() const {
        return m_data[0] * m_data[0] + 
               m_data[1] * m_data[1] + 
               m_data[2] * m_data[2];
    }
    
    // Normalized vector
    Vector3 Normalized() const {
        double mag = Magnitude();
        if (mag > 0.0) {
            return *this / mag;
        }
        return Vector3(0.0, 0.0, 0.0);
    }
    
    // Normalize in place
    void Normalize() {
        double mag = Magnitude();
        if (mag > 0.0) {
            *this /= mag;
        }
    }
    
    // Convert to array
    std::array<double, 3> ToArray() const {
        return m_data;
    }
    
private:
    std::array<double, 3> m_data;
};

// Global operators
inline Vector3 operator*(double scalar, const Vector3& vec) {
    return vec * scalar;
}

} // namespace BlackHoleSim