#pragma once

#include <type_traits>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <ostream>

namespace ZAMath {
    template <typename T>
    class Vector {
        private:
            T* _data = nullptr;
            size_t _size = 0;
            double _eps = 1e-10;

            // --- Основные проверки ---

            void _check_index(size_t index) const;
            void _check_size_match(const Vector& rhs, const char* op) const;
            void _check_divisor(T scalar) const;

            // --- Вспомогательные проверки для операторов сравнения ---

            bool _size_equal(const Vector& rhs) const noexcept;
            bool _almost_equal(const Vector& rhs) const noexcept;

        public:
            // --- Конструкторы / Деструктор / Операторы копирования и перемещения ---

            Vector();
            Vector(size_t size);
            Vector(size_t size, T init_val);
            ~Vector();

            Vector(const Vector& other);
            Vector(Vector&& other) noexcept;

            Vector& operator=(const Vector& other);
            Vector& operator=(Vector&& other) noexcept;

            // --- Операторы доступа по индексу ---

            T& operator[](size_t index);
            const T& operator[](size_t index) const;

            // --- Геттеры ---

            size_t size() const noexcept;
            double eps() const noexcept;

            // --- Сеттеры ---

            void set_eps(double new_eps);
            void resize(size_t new_size, T value = T());

            // --- Итераторы ---

            T* begin() noexcept;
            T* end() noexcept;
            const T* begin() const noexcept;
            const T* end() const noexcept;

            // --- Арифметические операторы ---

            Vector operator+(const Vector& rhs) const;
            Vector operator-(const Vector& rhs) const;
            Vector operator*(T scalar) const;
            Vector operator/(T scalar) const;

            // --- Скалярное произведение / Норма вектора ---

            T dot(const Vector& rhs) const;
            T norm() const;

            // --- Операторы сравнения ---

            bool operator==(const Vector& rhs) const;
            bool operator!=(const Vector& rhs) const;

            // --- Оператор вывода ---

            template <typename U>
            friend std::ostream& operator<<(std::ostream& os, const Vector<U>& v);
    };
}


#include "../../../src/core/Vector.tpp"
