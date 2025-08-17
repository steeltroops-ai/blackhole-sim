#include "Vector3.hpp"
#include <cmath>
#include <iostream>

using namespace BlackHoleSim;

int main() {
    Vector3 v1(1.0, 2.0, 3.0);
    Vector3 v2(4.0, 5.0, 6.0);
    
    double dot = v1.Dot(v2);
    std::cout << "Dot product: " << dot << std::endl;
    
    double abs_dot = std::abs(dot);
    std::cout << "Absolute dot: " << abs_dot << std::endl;
    
    return 0;
}