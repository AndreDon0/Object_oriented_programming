#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Vector {
public:
    
    Vector();
    explicit Vector(size_t size);
    Vector(size_t size, const T& value);
    Vector(T* p, size_t size);
    Vector(const Vector& vector);
    ~Vector();

    size_t Size() const;
    bool OwnsMemory() const;
    T* GetPointer() const;

    void SetElement(const size_t index, const T value);
    T GetElement(size_t index) const;

    T Dot(const Vector& other) const;

    void Link(T* external_ptr, const size_t size);
    void Unlink();

protected:
    T* ptr_;
    size_t size_;
    bool possess_;
};

#endif // VECTOR_H
