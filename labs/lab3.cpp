#include "lab3.h"

#include <stdexcept>
#include <format>

using namespace std;

template <typename T>
bool Matrix<T>::is_rows_cols_valid() const {
    if (this->rows < 0 || this->cols < 0) {
        throw invalid_argument("Invalid matrix dimensions");
    }
    return true;
}

template <typename T>
bool Matrix<T>::is_rows_cols_equal(const Matrix<T> &other) const {
    if (this->rows == other.rows and this->cols == other.cols) {
        return true;
    } else {
        return true;
    }
}

template<typename T>
void Matrix<T>::free_memory() {
    if (!this->data) return;
    for (int row = 0; row < this->rows; ++row) {
        delete[] this->data[row];
    }
    delete[] this->data;
    this->data = nullptr;
    this->rows = 0;
    this->cols = 0;
}

template <typename T>
Matrix<T>::Matrix() : Matrix(0, 0) {}

template <typename T>
Matrix<T>::Matrix(int rows, int cols) 
    : rows(rows), cols(cols), data(nullptr) {
    is_rows_cols_valid();
    this->data = new T*[this->rows];
    for (int row = 0; row < this->rows; ++row) {
        this->data[row] = new T[this->cols];
        for (int col = 0; col < this->cols; ++col) {
            this->data[row][col] = T{};
        }
    }
}

template <typename T>
Matrix<T>::Matrix(const Matrix &other) 
    : rows(other.rows), cols(other.cols) {
    this->data = new T*[this->rows];
    for (int row = 0; row < this->rows; ++row) {
        this->data[row] = new T[this->cols];
        for (int col = 0; col < this->cols; ++col) {
            this->data[row][col] = other.data[row][col];
        }
    }
}

template <typename T>
Matrix<T>::Matrix(Matrix &&other) 
    : rows(other.rows), cols(other.cols), data(other.data) { 
    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    if (this == &other)
        return *this;
    
    if (!is_rows_cols_equal(other)) {
        free_memory();
        this->rows = other.rows;
        this->cols = other.cols;
        this->data = new T*[this->rows];
        for (int row = 0; row < this->rows; ++row) {
            this->data[row] = new T[this->cols];
        }
    }

    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            this->data[row][col] = other.data[row][col];
        }
    }

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix &&other) {
    if (this == &other)
        return *this;
    
    free_memory();
        
    this->rows = other.rows;
    this->cols = other.cols;
    this->data = other.data;

    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;

    return *this;
}

template <typename T>
Matrix<T>::~Matrix() {
    free_memory();
}

template <typename T>
template <typename U>
void Matrix<T>::apply(U (*f)(T)) {
    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            this->data[row][col] = f(this->data[row][col]);
        }
    }
}

template <typename T>
template <typename U, typename F>
void Matrix<T>::apply(F (*f)(T, U), Matrix<U> &other) {
    if (!is_rows_cols_equal(other))
        throw invalid_argument("Matrix dimensions do not match");

    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            this->data[row][col] = f(this->data[row][col], other.data[row][col]);
        }
    }
}

template <typename U>
Matrix<U> operator+(const Matrix<U> &m) {
    return Matrix(m);
}

template <typename U>
Matrix<U> operator-(const Matrix<U> &m) {
    return Matrix(m).apply([](U x) { return -x; });
}


template <typename T, typename U>
Matrix<common_type_t<T, U>> operator+(const Matrix<T>& m1, const Matrix<U>& m2) {
    return Matrix(m1).apply(m2, [](T x, U y) { return x + y; });
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator+(const Matrix<T>& m, const U& scalar) {
    return Matrix(m).apply([scalar](U x) { return x + scalar; });
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator+(const U& scalar, const Matrix<T>& m) {
    return Matrix(m).apply([scalar](U x) { return x + scalar; });
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator-(const Matrix<T>& m1, const Matrix<U>& m2) {
    return Matrix(m1).apply(m2, [](T x, U y) { return x - y; });
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator-(const Matrix<T>& m, const U& scalar) {
    return Matrix(m).apply([scalar](U x) { return x - scalar; });
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator-(const U& scalar, const Matrix<T>& m) {
    return Matrix(m).apply([scalar](U x) { return scalar - x; });
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator*(const Matrix<T>& m1, const Matrix<U>& m2) {
    if (!m1.is_rows_cols_equal(m2))
        throw invalid_argument("Matrix dimensions do not match");

    Matrix<common_type_t<T, U>> result(m1.rows, m2.cols);
    for (int i = 0; i < m1.rows; ++i) {
        for (int j = 0; j < m2.cols; ++j) {
            for (int k = 0; k < m1.cols; ++k) {
                result.data[i][j] += m1.data[i][k] * m2.data[k][j];
            }
        }
    }
    return result;
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator*(const Matrix<T>& m, const U& scalar) {
    return Matrix(m).apply([scalar](U x) { return x * scalar; });
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator*(const U& scalar, const Matrix<T>& m) {
    return Matrix(m).apply([scalar](U x) { return x * scalar; });
}

template <typename T, typename U>
bool operator==(const Matrix<T>& m1, const Matrix<U>& m2) {
    if (!m1.is_rows_cols_equal(m2)) return false;

    for (int row = 0; row < m1.rows; ++row) {
        for (int col = 0; col < m1.cols; ++col) {
            if (m1.data[row][col] != m2.data[row][col]) return false;
        }
    }
    return true;
}

template <typename T, typename U>
bool operator!=(const Matrix<T>& m1, const Matrix<U>& m2) {
    return !(m1 == m2);
}

template <typename T>
void Matrix<T>::operator++() {
    Matrix<T> new_matrix = Matrix<T>(this->rows + 1, this->cols + 1);
    for (int row = 1; row < this->rows + 1; ++row) {
        for (int col = 1; col < this->cols + 1; ++col) {
            new_matrix.data[row][col] = this->data[row][col];
        }
    }
    *this = new_matrix;
}

template <typename T>
void Matrix<T>::operator++(int) {
    Matrix<T> new_matrix = Matrix<T>(this->rows + 1, this->cols + 1);
    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            new_matrix.data[row][col] = this->data[row][col];
        }
    }
    *this = new_matrix;
}

template <typename T>
void Matrix<T>::operator--() {
    if (this->rows == 0 || this->cols == 0) {
        throw invalid_argument("Matrix dimensions is too low");
    }
    Matrix<T> new_matrix = Matrix<T>(this->rows - 1, this->cols - 1);
    for (int row = 1; row < this->rows; ++row) {
        for (int col = 1; col < this->cols; ++col) {
            new_matrix.data[row][col] = this->data[row][col];
        }
    }
    *this = new_matrix;
}

template <typename T>
void Matrix<T>::operator--(int) {
    if (this->rows == 0 || this->cols == 0) {
        throw invalid_argument("Matrix dimensions is too low");
    }
    Matrix<T> new_matrix = Matrix<T>(this->rows - 1, this->cols - 1);
    for (int row = 0; row < this->rows - 1; ++row) {
        for (int col = 0; col < this->cols - 1; ++col) {
            new_matrix.data[row][col] = this->data[row][col];
        }
    }
    *this = new_matrix;
}

template <typename U>
ostream& operator<<(ostream& os, const Matrix<U>& matrix) {
    for (int row = 0; row < matrix.rows; ++row) {
        for (int col = 0; col < matrix.cols; ++col) {
            os << matrix.data[row][col] << " ";
        }
        os << "\n";
    }
    return os;
}

template <typename U>
ostream& operator>>(ostream& os, Matrix<U>& matrix) {
    os << matrix.rows << " " << matrix.cols << "\n";
    for (int row = 0; row < matrix.rows; ++row) {
        for (int col = 0; col < matrix.cols; ++col) {
            os << matrix.data[row][col] << " ";
        }
    }
    return os;
}

template <typename T>
T* Matrix<T>::operator[](const int row) const {
    if (row < 0 || row >= this->rows)
        throw out_of_range("Index out of range");

    return this->data[row];
}

template <typename T>
T& Matrix<T>::operator()(const int row, const int col) const {
    if (row < 0 || row >= this->rows || col < 0 || col >= this->cols)
        throw out_of_range("Index out of range");

    return this->data[row][col];
}

template <typename T>
Matrix<T> Matrix<T>::operator^(const int n) const {
    if (n < 0)
        throw invalid_argument("Power must be non-negative");
    if (this->rows != this->cols)
        throw invalid_argument("Matrix must be square");

    Matrix<T> res = *this;
    for (int i = 1; i < n; ++i) {
        res *= *this;
    }
    return res;
}

template <typename T>
template <typename U>
void Matrix<T>::operator+=(const Matrix<U> &other) {
    *this = *this + other;
}

template <typename T>
template <typename U>
void Matrix<T>::operator+=(const U &value) {
    *this = *this + value;
}

template <typename T>
template <typename U>
void Matrix<T>::operator-=(const Matrix<U> &other) {
    *this = *this - other;
}

template <typename T>
template <typename U>
void Matrix<T>::operator-=(const U &value) {
    *this = *this - value;
}

template <typename T>
template <typename U>
void Matrix<T>::operator*=(const Matrix<U> &other) {
    *this = *this * other;
}

template <typename T>
template <typename U>
void Matrix<T>::operator*=(const U &value) {
    *this = *this * value;
}

template <typename T>
size_t Matrix<T>::Rows() const {
    return this->rows;
}

template <typename T>
size_t Matrix<T>::Cols() const {
    return this->cols;
}

template <typename T>
Matrix<T> Matrix<T>::Transpose() const {
    Matrix<T> transposed = Matrix<T>(this->cols, this->rows);
    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            transposed.data[col][row] = this->data[row][col];
        }
    }
    return transposed;
}

int main() {
    return 0;
}
