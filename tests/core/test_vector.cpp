#include <iostream>
#include <cassert>
#include <sstream>

#include "ZAMath/core/Vector.hpp"


void test_constructors() {
    ZAMath::Vector<int> v1;
    assert(v1.size() == 0);

    ZAMath::Vector<double> v2(5);
    assert(v2.size() == 5);
    for (size_t i = 0; i < v2.size(); ++i)
        assert(v2[i] == 0.0);

    ZAMath::Vector<int> v3(3, 42);
    assert(v3.size() == 3);
    for (size_t i = 0; i < v3.size(); ++i)
        assert(v3[i] == 42);

    ZAMath::Vector<int> v4 = v3;
    assert(v4.size() == 3 && v4 == v3);

    ZAMath::Vector<int> v5(std::move(v4));
    assert(v5.size() == 3 && v5 == v3);

    std::cout << "[OK] test_constructors passed\n";
}

void test_assignment_and_resize() {
    ZAMath::Vector<float> v1(4, 1.5f);
    ZAMath::Vector<float> v2;
    v2 = v1;
    assert(v2.size() == 4 && v2 == v1);

    v2.resize(6, 2.5f);
    assert(v2.size() == 6);
    assert(v2[4] == 2.5f && v2[5] == 2.5f);

    v2.resize(2);
    assert(v2.size() == 2);
    assert(v2[0] == 1.5f && v2[1] == 1.5f);

    std::cout << "[OK] test_assignment_and_resize passed\n";
}

void test_arithmetic() {
    ZAMath::Vector<int> v1(3, 2);
    ZAMath::Vector<int> v2(3, 5);
    ZAMath::Vector<int> v3 = v1 + v2;
    assert(v3[0] == 7 && v3[1] == 7 && v3[2] == 7);

    ZAMath::Vector<int> v4 = v2 - v1;
    assert(v4[0] == 3 && v4[1] == 3 && v4[2] == 3);

    ZAMath::Vector<int> v5 = v1 * 3;
    assert(v5[0] == 6 && v5[1] == 6 && v5[2] == 6);

    ZAMath::Vector<int> v6 = v2 / 5;
    assert(v6[0] == 1 && v6[1] == 1 && v6[2] == 1);

    std::cout << "[OK] test_arithmetic passed\n";
}

void test_dot_and_norm() {
    ZAMath::Vector<double> v1(3);
    v1[0] = 1.0; v1[1] = 2.0; v1[2] = 3.0;
    ZAMath::Vector<double> v2(3);
    v2[0] = 4.0; v2[1] = 5.0; v2[2] = 6.0;

    double dot = v1.dot(v2);
    assert(dot == 1*4 + 2*5 + 3*6);

    double norm = v1.norm();
    assert(std::abs(norm - std::sqrt(1*1 + 2*2 + 3*3)) < 1e-9);

    std::cout << "[OK] test_dot_and_norm passed\n";
}

void test_comparison_and_eps() {
    ZAMath::Vector<double> v1(2);
    v1[0] = 1.0; v1[1] = 2.0;
    ZAMath::Vector<double> v2(2);
    v2[0] = 1.0 + 1e-11; v2[1] = 2.0 - 1e-11;

    assert(v1 == v2);

    v1.set_eps(1e-15);
    v2.set_eps(1e-15);
    assert(v1 != v2);

    std::cout << "[OK] test_comparison_and_eps passed\n";
}

void test_output() {
    ZAMath::Vector<int> v(3);
    v[0] = 10; v[1] = 20; v[2] = 30;
    std::ostringstream oss;
    oss << v;
    assert(oss.str() == "[10, 20, 30]");

    std::cout << "[OK] test_output passed\n";
}


int main() {
    test_constructors();
    test_assignment_and_resize();
    test_arithmetic();
    test_dot_and_norm();
    test_comparison_and_eps();
    test_output();

    std::cout << "All Vector tests passed successfully!" << std::endl;

    return 0;
}
