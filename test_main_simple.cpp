#include <iostream>
#include <exception>
#include <cstdlib>

int main() {
    try {
        std::cout << "Test successful" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}