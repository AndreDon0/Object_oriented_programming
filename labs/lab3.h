#ifndef LAB3_H
#define LAB3_H

#include <iostream>

using namespace std;

template <typename T>
class Matrix
{
public:
    Matrix();
    Matrix(const int rows, const int cols);

    Matrix(const Matrix &other);
    Matrix(Matrix &&other);
    Matrix& operator=(const Matrix &other);
    Matrix& operator=(Matrix &&other);

    ~Matrix();

    template <typename U>
    friend Matrix<U> operator+(const Matrix<U> &m);
    template <typename U>
    friend Matrix<U> operator-(const Matrix<U> &m);

    template <typename U>
    friend istream& operator>>(istream& is, Matrix<U>& matrix);
    template <typename U>
    friend ostream& operator<<(ostream& os, const Matrix<U>& matrix);

    template <typename U>
    friend Matrix<U> operator+(const Matrix<U> &m1, const Matrix<U> &m2);
    template <typename U>
    friend Matrix<U> operator+(const U &scalar, const Matrix<U> &m);
    template <typename U>
    friend Matrix<U> operator+(const Matrix<U> &m, const U &scalar);

    template <typename U>
    friend Matrix<U> operator-(const Matrix<U> &m1, const Matrix<U> &m2);
    template <typename U>
    friend Matrix<U> operator-(const U &scalar, const Matrix<U> &m);
    template <typename U>
    friend Matrix<U> operator-(const Matrix<U> &m, const U &scalar);

    template <typename U>
    friend Matrix<U> operator*(const Matrix<U> &m1, const Matrix<U> &m2);
    template <typename U>
    friend Matrix<U> operator*(const U &scalar, const Matrix<U> &m);
    template <typename U>
    friend Matrix<U> operator*(const Matrix<U> &m, const U &scalar);

    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;

    Matrix& operator++();
    Matrix operator++(int);
    Matrix& operator--();
    Matrix operator--(int);

    template <typename U> 
    istream& operator>>(istream& is, Matrix<U>& matrix);
    template <typename U> 
    ostream& operator<<(ostream& os, const Matrix<U>& matrix);

    T* operator[](const int i) const;
    T operator()(const int i, const int j) const;

    Matrix operator^(const int n) const;

    void operator+=(const Matrix &other);
    void operator+=(const T &value);
    void operator-=(const Matrix &other);
    void operator-=(const T &value);
    void operator*=(const Matrix &other);
    void operator*=(const T &value);

    int Rows() const;
    int Cols() const;
    Matrix Transpose() const;

private:
    T **data;
    int rows;
    int cols;
};

#endif
