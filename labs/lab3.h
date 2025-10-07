#ifndef LAB3_H
#define LAB3_H

#include <iostream>
#include <type_traits>

using namespace std;

template <typename T>
class Matrix
{
public:
    Matrix();
    Matrix(const int rows, const int cols);

    Matrix(const Matrix<T> &other);
    Matrix(Matrix<T> &&other);
    Matrix<T>& operator=(const Matrix<T> &other);
    Matrix<T>& operator=(Matrix<T> &&other);

    ~Matrix();

    template <typename U>
    friend Matrix<U> operator+(const Matrix<U> &m);
    template <typename U>
    friend Matrix<U> operator-(const Matrix<U> &m);

    template <typename U>
    friend Matrix<common_type_t<T, U>> operator+(const Matrix<T> &m1, const Matrix<U> &m2);
    template <typename U>
    friend Matrix<common_type_t<T, U>> operator+(const U &scalar, const Matrix<T> &m);
    template <typename U>
    friend Matrix<common_type_t<T, U>> operator+(const Matrix<T> &m, const U &scalar);

    template <typename U>
    friend Matrix<common_type_t<T, U>> operator-(const Matrix<T> &m1, const Matrix<U> &m2);
    template <typename U>
    friend Matrix<common_type_t<T, U>> operator-(const U &scalar, const Matrix<T> &m);
    template <typename U>
    friend Matrix<common_type_t<T, U>> operator-(const Matrix<T> &m, const U &scalar);

    template <typename U>
    friend Matrix<common_type_t<T, U>> operator*(const Matrix<T> &m1, const Matrix<U> &m2);
    template <typename U>
    friend Matrix<common_type_t<T, U>> operator*(const U &scalar, const Matrix<T> &m);
    template <typename U>
    friend Matrix<common_type_t<T, U>> operator*(const Matrix<T> &m, const U &scalar);

    template <typename U>
    bool operator==(const Matrix<U> &other) const;
    template <typename U>
    bool operator!=(const Matrix<U> &other) const;

    void operator++();
    void operator++(int);
    void operator--();
    void operator--(int);

    template <typename U>
    friend ostream& operator<<(ostream& os, const Matrix<U>& matrix);
    template <typename U>
    friend ostream& operator>>(ostream& os, Matrix<U>& matrix);

    T* operator[](const int row) const;
    T& operator()(const int row, const int col) const;

    Matrix operator^(const int n) const;

    template <typename U>
    void operator+=(const Matrix<U> &other);
    template <typename U>
    void operator+=(const U &value);
    template <typename U>
    void operator-=(const Matrix<U> &other);
    template <typename U>
    void operator-=(const U &value);
    template <typename U>
    void operator*=(const Matrix<U> &other);
    template <typename U>
    void operator*=(const U &value);

    size_t Rows() const;
    size_t Cols() const;
    Matrix<T> Transpose() const;

    template <typename U>
    Matrix<U> apply(U (*f)(T));
    template <typename U, typename F>
    Matrix<F> apply(F (*f)(T, U), Matrix<U> &other);

    Matrix<T>& apply(T (*f)(T));
    template <typename U>
    Matrix<T>& apply(T (*f)(T, U), Matrix<U> &other);

private:
    T **data;
    size_t rows, cols;

    bool is_rows_cols_valid() const;
    template <typename U>
    bool is_rows_cols_equal(const Matrix<U> &other) const;
    void free_memory();
};

#endif