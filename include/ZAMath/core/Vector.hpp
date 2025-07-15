#pragma once


namespace ZAMath {
    template <typename T>
    class Vector {
        static_assert(
            std::is_arithmetic_v<T>, 
            "Vector requires integer or floating-point type"
        );

    private:
        T* _data = nullptr;
        size_t _size = 0;

        using EpsilonType = std::conditional_t<
            std::is_floating_point_v<T>,
            T,
            std::nullptr_t
        >;

        EpsilonType _eps = []{
            if constexpr (std::is_floating_point_v<T>) {
                return T(1e-10);
            } else {
                return 1e-10;
            }
        }();

        void _check_index(size_t index) const {
            if (index >= _size) {
                throw std::out_of_range("Vector index out of range");
            }
        }

    public:
        Vector() = default;

        explicit Vector(size_t size) : _size(size) {
            _data = new T[_size]();
        }

        Vector(size_t size, T init_val) : _size(size) {
            _data = new T[_size];
            std::fill(_data, _data + _size, init_val);
        }

        ~Vector() { delete[] _data; }

        Vector(const Vector& other) : _size(other._size) {
            _data = new T[_size];
            std::copy(other._data, other._data + _size, _data);
        }

        Vector& operator=(const Vector& other) {
            if (this != &other) {
                delete[] _data;
                _size = other._size;
                _data = new T[_size];
                std::copy(other._data, other._data + _size, _data);
            }

            return *this;
        }

        Vector(Vector&& other) noexcept 
            : _data(other._data), _size(other._size) {
            other._data = nullptr;
            other._size = 0;
        }

        Vector& operator=(Vector&& other) noexcept {
            if (this != &other) {
                delete[] _data;
                _data = other._data;
                _size = other._size;
                other._data = nullptr;
                other._size = 0;
            }

            return *this;
        }

        T& operator[](size_t index) {
            _check_index(index);
            return _data[index];
        }

        const T& operator[](size_t index) const {
            _check_index(index);
            return _data[index];
        }

        size_t size() const noexcept { return _size; }
        EpsilonType eps() const noexcept { return _eps; }
    };
}