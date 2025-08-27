#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    std::cout << "Testing basic C++ compilation..." << std::endl;
    
    // Test basic math
    double pi = M_PI;
    double result = sin(pi / 2.0);
    
    std::cout << "sin(pi/2) = " << result << std::endl;
    
    if (std::abs(result - 1.0) < 1e-10) {
        std::cout << "✓ Math test passed!" << std::endl;
        return 0;
    } else {
        std::cout << "✗ Math test failed!" << std::endl;
        return 1;
    }
}