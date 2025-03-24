//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_VECTOR_H
#define SKIPLIST_VECTOR_H

#include <cstddef>

namespace ds {
    namespace detail {
        constexpr int INITIAL_VECTOR_CAPACITY = 32;
    }

    template<typename T>
    class Vector {
    private:
        T *data;
        size_t size;
        size_t capacity;

    public:
        explicit Vector(size_t size)
            : size(size),
              capacity((size > detail::INITIAL_VECTOR_CAPACITY) ? size : detail::INITIAL_VECTOR_CAPACITY) {
            data = new T[capacity];
        }

        explicit Vector(size_t size, const T &value)
            : size(size),
              capacity((size > detail::INITIAL_VECTOR_CAPACITY) ? size : detail::INITIAL_VECTOR_CAPACITY) {
            data = new T[capacity];
            for (size_t i = 0; i < size; i++) {
                data[i] = value;
            }
        }

        Vector() : size(0), capacity(detail::INITIAL_VECTOR_CAPACITY) {
            data = new T[capacity];
        }

        ~Vector() {
            delete[] data;
        }

        [[nodiscard]] size_t getSize() const {
            return size;
        }

        T &operator[](size_t index) {
            return data[index];
        }

        void pushBack(const T &value) {
            if (size == capacity) {
                capacity *= 2;
                T *new_data = new T[capacity];
                for (size_t i = 0; i < size; i++) {
                    new_data[i] = data[i];
                }
                delete[] data;
                data = new_data;
            }
            data[size++] = value;
        }

        void popBack() {
            size--;
        }

        T getBack() {
            return data[size - 1];
        }

        T *begin() {
            return data;
        }

        T *end() {
            return data + size;
        }

        Vector &operator=(const Vector &other) {
            if (this == &other) {
                return *this;
            }
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
            return *this;
        }

        Vector(const Vector &other) {
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }

        [[nodiscard]] bool empty() const {
            return size == 0;
        }
    };
} // ds

#endif //SKIPLIST_VECTOR_H
