#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

using namespace std;

template <typename T>
class Vector {
public:
    
    Vector() : ptr_(nullptr), size_(0), possess_(true) {
    }
    explicit Vector(size_t size) : size_(size), possess_(true) {
        ptr_ = size > 0 ? new T[size]() : nullptr;
    }
    Vector(size_t size, const T& value) : size_(size), possess_(true) {
        ptr_ = size > 0 ? new T[size] : nullptr;
        fill_n(ptr_, size, value);
    }
    Vector(T* ptr, size_t size) : ptr_(ptr), size_(size), possess_(false) {}
    Vector(const Vector& vector) {
        size_ = vector.Size();
        possess_ = vector.OwnsMemory();
        if (possess_) {
            ptr_ = new T[size_];
            copy_n(vector.GetPointer(), size_, ptr_);
        } else {
            ptr_ = vector.GetPointer();
        }
    }
    ~Vector() {
        if (possess_)
            delete[] ptr_;
    }

    size_t Size() const { return size_; }
    bool OwnsMemory() const { return possess_; }
    T* GetPointer() const { return ptr_; }

    void SetElement(const size_t index, const T value) {
        if (index < 0 || index >= size_)
            throw out_of_range("Index out of range");
        ptr_[index] = value;
    }
    T GetElement(size_t index) const {
        if (index < 0 || index >= size_)
            throw out_of_range("Index out of range");
        return ptr_[index];
    }

    T Dot(const Vector& other) const {
        if (size_ != other.Size())
            throw invalid_argument("Vectors have different sizes");
        T result = T();
        for (size_t i = 0; i < size_; i++)
            result += ptr_[i] * other.GetPointer()[i];
        return result;
    }

    void Link(T* external_ptr, const size_t size) {
        if (possess_)
            delete[] ptr_;
        ptr_ = external_ptr;
        size_ = size;
        possess_ = false;
    }
    void Unlink() {
        if (!possess_)
            ptr_ = nullptr;
        size_ = 0;
        possess_ = true;
    }

protected:
    T* ptr_;
    size_t size_;
    bool possess_;
};

int main() {
    try {
        // --- Test 1: default constructor ---
        Vector<int> v0;
        cout << "v0.Size() = " << v0.Size() << ", OwnsMemory = " << v0.OwnsMemory() << "\n";

        // --- Test 2: sized constructor ---
        Vector<int> v1(5);
        cout << "v1.Size() = " << v1.Size() << "\n";
        v1.SetElement(2, 42);
        cout << "v1.GetElement(2) = " << v1.GetElement(2) << "\n";

        // --- Test 3: fill constructor ---
        Vector<int> v2(4, 7);
        cout << "v2 = ";
        for (size_t i = 0; i < v2.Size(); i++)
            cout << v2.GetElement(i) << " ";
        cout << "\n";

        // --- Test 4: dot product ---
        Vector<int> v3(4, 2);
        cout << "Dot(v2, v3) = " << v2.Dot(v3) << "\n";  // expect 7*2*4 = 56

        // --- Test 5: link to external memory ---
        int raw[3] = {10, 20, 30};
        Vector<int> v4;
        v4.Link(raw, 3);
        cout << "v4 linked to raw: ";
        for (size_t i = 0; i < v4.Size(); i++)
            cout << v4.GetElement(i) << " ";
        cout << "\n";

        v4.SetElement(1, 99); // modifies raw array
        cout << "raw[1] after modification = " << raw[1] << "\n";

        // --- Test 6: unlink ---
        v4.Unlink();
        cout << "v4 after unlink: Size = " << v4.Size() << ", OwnsMemory = " << v4.OwnsMemory() << "\n";

        // --- Test 7: out_of_range exception ---
        try {
            v1.GetElement(100);  // should throw
        } catch (const out_of_range& e) {
            cout << "Caught out_of_range: " << e.what() << "\n";
        }

    } catch (const exception& e) {
        cout << "Unexpected exception: " << e.what() << "\n";
    }

    return 0;
    }

