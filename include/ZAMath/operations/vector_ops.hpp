#pragma once
#include "../core/Vector.hpp"

namespace ZAMath {
    template <typename T>
    Vector<T> cross_product(const Vector<T>& a, const Vector<T>& b);

    template <typename T>
    Vector<T> normalize(const Vector<T>& v);
    // ... другие операции
}