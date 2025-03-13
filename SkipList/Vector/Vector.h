//
// Created by OvenFoSten on 25-3-13.
//

#ifndef SKIPLIST_VECTOR_H
#define SKIPLIST_VECTOR_H

#include <cstddef>

namespace {
    //Configurations
    const int INITIAL_CAPACITY = 16;
}

namespace ds {
    template<typename T>
    class Vector {
    private:
        T *data;
        size_t size;
        size_t capacity;

    public:
        explicit Vector(size_t size) : size(size), capacity(size) {
            data = new T[size];
        }

        Vector() : size(0), capacity(INITIAL_CAPACITY) {
            data = new T[capacity];
        }

        ~Vector() {
            delete[] data;
        }

        size_t getSize() const {
            return size;
        }

        T operator[](size_t index) {
            return data[index];
        }

        void pushBack(T value) {
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

        T &operator=(const Vector &other) = delete;

        Vector(const Vector &other) = delete;
    };
} // ds

#endif //SKIPLIST_VECTOR_H
