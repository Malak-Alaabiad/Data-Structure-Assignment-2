#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
using namespace std;
#define endl '\n'
#define Template template <typename T>

Template
class Vector {
private:
    T* data;
    size_t capacity;
    size_t currentSize;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < currentSize; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    Vector() : data(new T[1]), capacity(1), currentSize(0) {}

    ~Vector() {
        delete[] data;
    }

    Vector(const Vector& other) : data(new T[other.capacity]), capacity(other.capacity), currentSize(other.currentSize) {
        for (size_t i = 0; i < currentSize; ++i) {
            data[i] = other.data[i];
        }
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) return *this; 

        delete[] data;

        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];
        for (size_t i = 0; i < currentSize; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    void push_back(const T& value) {
        if (currentSize == capacity) {
            resize();
        }
        data[currentSize++] = value;
    }

    void pop_back() {
        if (currentSize > 0) {
            --currentSize;
        }
    }

    T& operator[](size_t index) {
        if (index >= currentSize) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t size() const {
        return currentSize;
    }

    bool empty() const {
        return currentSize == 0;
    }

    void clear() {
        currentSize = 0;
    }
};

#endif