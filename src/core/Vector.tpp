#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <type_traits>
#include <ostream>


namespace ZAMath {
    // --- Проверки ---

    template <typename T>
    void Vector<T>::_check_index(size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Vector index out of range");
        }
    }

    template <typename T>
    void Vector<T>::_check_size_match(const Vector& rhs, const char* op) const {
        if (_size != rhs._size) {
            throw std::invalid_argument(std::string("Vector sizes must match for ") + op);
        }
    }

    template <typename T>
    void Vector<T>::_check_divisor(T scalar) const {
        if constexpr (std::is_floating_point_v<T>) {
            if (std::abs(scalar) < _eps) {
                throw std::invalid_argument("Division by zero");
            }
        } else {
            if (scalar == 0) {
                throw std::invalid_argument("Division by zero");
            }
        }
    }

    template <typename T>
    bool Vector<T>::_size_equal(const Vector& rhs) const noexcept {
        return _size == rhs._size;
    }

    template <typename T>
    bool Vector<T>::_almost_equal(const Vector& rhs) const noexcept {
        if (!_size_equal(rhs)) {
            return false;
        }

        double eps = std::max(_eps, rhs._eps);

        for (size_t i = 0; i < _size; i++) {
            double diff = static_cast<double>(_data[i]) - static_cast<double>(rhs._data[i]);

            if (std::abs(diff) > eps) {
                return false;
            }
        }

        return true;
    }

    // --- Конструкторы / Деструктор / Операторы копирования и перемещения ---

    template <typename T>
    Vector<T>::Vector() = default;

    template <typename T>
    Vector<T>::Vector(size_t size) : _size(size) {
        _data = new T[_size]();
    }

    template <typename T>
    Vector<T>::Vector(size_t size, T init_val) : _size(size) {
        _data = new T[_size];
        std::fill(_data, _data + _size, init_val);
    }

    template <typename T>
    Vector<T>::~Vector() {
        if (_data) {
            delete[] _data;
            _data = nullptr;
        }
    }

    template <typename T>
    Vector<T>::Vector(const Vector& other) : _size(other._size), _eps(other._eps) {
        _data = new T[_size];
        std::copy(other._data, other._data + _size, _data);
    }

    template <typename T>
    Vector<T>::Vector(Vector&& other) noexcept 
        : _data(other._data), _size(other._size), _eps(other._eps) {
        other._data = nullptr;
        other._size = 0;
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(const Vector& other) {
        if (this != &other) {
            delete[] _data;
            _size = other._size;
            _eps = other._eps;
            _data = new T[_size];
            std::copy(other._data, other._data + _size, _data);
        }

        return *this;
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] _data;
            _data = other._data;
            _size = other._size;
            _eps = other._eps;
            other._data = nullptr;
            other._size = 0;
        }

        return *this;
    }

    // --- Операторы доступа по индексу ---

    template <typename T>
    T& Vector<T>::operator[](size_t index) {
        _check_index(index);
        return _data[index];
    }

    template <typename T>
    const T& Vector<T>::operator[](size_t index) const {
        _check_index(index);
        return _data[index];
    }

    // --- Геттеры ---

    template <typename T>
    size_t Vector<T>::size() const noexcept { return _size; }

    template <typename T>
    double Vector<T>::eps() const noexcept { return _eps; }

    // --- Сеттеры ---

    template <typename T>
    void Vector<T>::set_eps(double new_eps) { _eps = new_eps; }

    template <typename T>
    void Vector<T>::resize(size_t new_size, T value) {
        if (new_size == _size) {
            return;
        }

        T* new_data = new T[new_size];
        size_t min_size = (new_size < _size) ? new_size : _size;
        for (size_t i = 0; i < min_size; i++) {
            new_data[i] = _data[i];
        }

        if (new_size > _size) {
            std::fill(new_data + _size, new_data + new_size, value);
        }

        delete[] _data;
        _data = new_data;
        _size = new_size;
    }

    // --- Итераторы ---

    template <typename T>
    T* Vector<T>::begin() noexcept { return _data; }

    template <typename T>
    T* Vector<T>::end() noexcept { return _data + _size; }

    template <typename T>
    const T* Vector<T>::begin() const noexcept { return _data; }

    template <typename T>
    const T* Vector<T>::end() const noexcept { return _data + _size; }

    // --- Арифметические операторы ---

    template <typename T>
    Vector<T> Vector<T>::operator+(const Vector& rhs) const {
        _check_size_match(rhs, "addition");

        Vector result(_size);
        for (size_t i = 0; i < _size; i++) {
            result._data[i] = _data[i] + rhs._data[i];
        }

        return result;
    }

    template <typename T>
    Vector<T> Vector<T>::operator-(const Vector& rhs) const {
        _check_size_match(rhs, "subtraction");

        Vector result(_size);
        for (size_t i = 0; i < _size; i++) {
            result._data[i] = _data[i] - rhs._data[i];
        }

        return result;
    }

    template <typename T>
    Vector<T> Vector<T>::operator*(T scalar) const {
        Vector result(_size);
        for (size_t i = 0; i < _size; i++) {
            result._data[i] = _data[i] * scalar;
        }

        return result;
    }

    template <typename T>
    Vector<T> Vector<T>::operator/(T scalar) const {
        _check_divisor(scalar);

        Vector result(_size);
        for (size_t i = 0; i < _size; i++) {
            result._data[i] = _data[i] / scalar;
        }

        return result;
    }

    // --- Скалярное произведение / Норма вектора ---

    template <typename T>
    T Vector<T>::dot(const Vector& rhs) const {
        _check_size_match(rhs, "dot product");

        T result = T();
        for (size_t i = 0; i < _size; i++) {
            result += _data[i] * rhs._data[i];
        }

        return result;
    }

    template <typename T>
    T Vector<T>::norm() const {
        T sum = T();
        for (size_t i = 0; i < _size; i++) {
            sum += _data[i] * _data[i];
        }

        if constexpr (std::is_floating_point_v<T>) {
            return std::sqrt(sum);
        } else {
            return static_cast<T>(std::sqrt(static_cast<double>(sum)));
        }
    }

    // --- Операторы сравнения ---

    template <typename T>
    bool Vector<T>::operator==(const Vector& rhs) const {
        return _almost_equal(rhs);
    }

    template <typename T>
    bool Vector<T>::operator!=(const Vector& rhs) const {
        return !_almost_equal(rhs);
    }

    // --- Оператор вывода ---

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
        os << "[";
        for (size_t i = 0; i < v._size; i++) {
            os << v._data[i];
            if (i + 1 < v._size) {
                os << ", ";
            }
        }
        os << "]";

        return os;
    }
}
