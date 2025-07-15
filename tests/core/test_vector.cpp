#include <iostream>

#include <ZAMath/core/Vector.hpp>


int main() {
    try {
        ZAMath::Vector<double> v(3);
        std::cout << "Test passed: Vector created successfully\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}
