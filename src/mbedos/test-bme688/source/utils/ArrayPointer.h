#ifndef ARRAY_POINTER_H
#define ARRAY_POINTER_H

#include <cstdint>
#include <cstddef>

template <class T>
class ArrayPointer {
    public:
        T* const ptr;
        const size_t size;
        ArrayPointer(T * ptr, size_t size): ptr(ptr), size(size){}
};

#endif // ARRAY_POINTER_H