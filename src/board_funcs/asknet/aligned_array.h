// aligned_array.h
// Rohan Weeden
// Created: Feb. 13, 2018

// A smart array for managing aligned heap memory

#ifndef ASK_NET_ALIGNED_ARRAY_H
#define ASK_NET_ALIGNED_ARRAY_H

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>

template <typename T, size_t A>
class AlignedArray {
private:
    T * _data_unaligned;
    T * _data;
    size_t _size;
    size_t _size_unaligned;

    void * _aligned_alloc(size_t size, T*& unaligned_storage) {
        const size_t align_size = A;
        size_t request_size = size + align_size;
        const size_t needed = request_size;
        _size_unaligned = needed;

        T * alloc = new T[needed];
        void * alloc_void = (void *) alloc;
        unaligned_storage = alloc;
        void * ptr = std::align(
            align_size,
            size,
            alloc_void,
            request_size
        );
        return ptr;
    }


    void _allocate(size_t size) {
        T * data_unaligned;
        _data = (T *) _aligned_alloc(size, data_unaligned);
        _data_unaligned = data_unaligned;
    }


    void _del() {
        if (_data_unaligned != nullptr) {
            delete[] _data_unaligned;
        }
    }

public:
    AlignedArray() {
        _data = nullptr;
        _data_unaligned = nullptr;
        _size = 0;
        _size_unaligned = 0;

    }

    AlignedArray(size_t size) {
        _size = size;
        _allocate(size);
    }

    AlignedArray(const std::vector<T> & other) {
        _size = other.size();
        _allocate(_size);

        std::copy(other.begin(), other.end(), _data);
    }


    AlignedArray(const AlignedArray & other) {
        _size = other._size;
        _allocate(_size);

        std::copy(other._data, other._data + _size, _data);
    }


    AlignedArray(AlignedArray && other) {
        *this = std::move(other);
    }


    AlignedArray & operator=(const AlignedArray & other) {
        if (this == &other) {
            return *this;
        }
        _del();

        _size = other._size;
        _size_unaligned = other._size_unaligned;
        _allocate(_size);

        std::copy(other._data_unaligned, other._data_unaligned + _size_unaligned, _data_unaligned);

        return *this;
    }


    AlignedArray & operator=(AlignedArray && other) {
        if (this == &other) {
            return *this;
        }

        _data = other._data;
        other._data = nullptr;
        _data_unaligned = other._data_unaligned;
        other._data_unaligned = nullptr;

        _size = other._size;
        _size_unaligned = other._size_unaligned;
        other._size = 0;
        other._size_unaligned = 0;

        return *this;
    }

    const T & operator[](int index) const {
        if(index >= _size) {
            throw std::out_of_range("Index out of bounds" + std::to_string(index));
        }
        return _data[index];
    }

    T & operator[](int index) {
        if(index >= _size) {
            throw std::out_of_range("Index out of bounds" + std::to_string(index));
        }
        return _data[index];
    }


    virtual ~AlignedArray () {
        _del();
    }

    void resize(size_t newsize) {
        _del();
        _size = newsize;

        _allocate(newsize);
    }

    size_t size() const {
        return _size;
    }

    void setOverflow(T & val) {
        T * start = _data + _size;
        while (start < _data_unaligned + _size + A) {
            start[0] = val;
            start++;
        }
    }
    void setOverflow(T && val) {
        T * start = _data + _size;
        while (start < _data_unaligned + _size + A) {
            start[0] = val;
            start++;
        }
    }

    void fill(T & val) {
        for (size_t i = 0; i < _size_unaligned; i++) {
            _data_unaligned[i] = val;
        }
    }

    void fill(T && val) {
        for (size_t i = 0; i < _size_unaligned; i++) {
            _data_unaligned[i] = val;
        }
    }
};

#endif
