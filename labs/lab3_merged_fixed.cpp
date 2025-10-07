#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <format>

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

    friend Matrix<T> operator+(const Matrix<T> &m);
    friend Matrix<T> operator-(const Matrix<T> &m);

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

// ===== IMPLEMENTATIONS =====

template <typename T>
bool Matrix<T>::is_rows_cols_valid() const {
    if (this->rows < 0 || this->cols < 0) {
        throw invalid_argument("Invalid matrix dimensions");
    }
    return true;
}

template <typename T>
template <typename U>
bool Matrix<T>::is_rows_cols_equal(const Matrix<U> &other) const {
    if (this->rows == other.rows and this->cols == other.cols) {
        return true;
    } else {
        return false;
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
Matrix<T>::Matrix() : Matrix<T>(0, 0) {}

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
Matrix<T>::Matrix(const Matrix<T> &other) 
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
Matrix<T>::Matrix(Matrix<T> &&other) 
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
Matrix<T>& Matrix<T>::operator=(Matrix<T> &&other) {
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
Matrix<U> Matrix<T>::apply(U (*f)(T)) {
    Matrix<U> result(this->rows, this->cols);
    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            result[row][col] = f(this->data[row][col]);
        }
    }

    return result;
}

template <typename T>
template <typename U, typename F>
Matrix<F> Matrix<T>::apply(F (*f)(T, U), Matrix<U> &other) {
    if (!is_rows_cols_equal(other))
        throw invalid_argument("Matrix dimensions do not match");

    Matrix<F> result(this->rows, this->cols);
    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            result[row][col] = f(this->data[row][col], other.data[row][col]);
        }
    }

    return result;
}

template <typename T>
Matrix<T>& Matrix<T>::apply(T (*f)(T)) {
    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            this->data[row][col] = f(this->data[row][col]);
        }
    }

    return *this;
}

template <typename T>
template <typename U>
Matrix<T>& Matrix<T>::apply(T (*f)(T, U), Matrix<U> &other) {
    if (!is_rows_cols_equal(other))
        throw invalid_argument("Matrix dimensions do not match");

    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            this->data[row][col] = f(this->data[row][col], other.data[row][col]);
        }
    }

    return *this;
}

// Helper functions for your beautiful apply functions!
template<typename T>
T negate_func(T x) { return -x; }

template<typename T, typename U>
common_type_t<T, U> add_func(T x, U y) { return x + y; }

template<typename T, typename U>
common_type_t<T, U> sub_func(T x, U y) { return x - y; }

template<typename T, typename U>
common_type_t<T, U> mul_func(T x, U y) { return x * y; }

template<typename T, typename U>
common_type_t<T, U> add_scalar_func(T x, U scalar) { return x + scalar; }

template<typename T, typename U>
common_type_t<T, U> sub_scalar_func(T x, U scalar) { return x - scalar; }

template<typename T, typename U>
common_type_t<T, U> mul_scalar_func(T x, U scalar) { return x * scalar; }

template<typename T, typename U>
common_type_t<T, U> scalar_sub_func(T scalar, U x) { return scalar - x; }

template <typename T>
Matrix<T> operator+(const Matrix<T> &m) {
    return Matrix<T>(m);
}

template <typename T>
Matrix<T> operator-(const Matrix<T> &m) {
    Matrix<T> result(m);
    result.apply(negate_func<T>);
    return result;
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator+(const Matrix<T>& m1, const Matrix<U>& m2) {
    return Matrix<common_type_t<T, U>>(m1).apply(m2, add_func<T, U>);
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator+(const Matrix<T>& m, const U& scalar) {
    return Matrix<common_type_t<T, U>>(m).apply(add_scalar_func<T, U>);
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator+(const U& scalar, const Matrix<T>& m) {
    return m + scalar;
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator-(const Matrix<T>& m1, const Matrix<U>& m2) {
    return Matrix<common_type_t<T, U>>(m1).apply(m2, sub_func<T, U>);
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator-(const Matrix<T>& m, const U& scalar) {
    return Matrix<common_type_t<T, U>>(m).apply(sub_scalar_func<T, U>, scalar);
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator-(const U& scalar, const Matrix<T>& m) {
    return Matrix<common_type_t<T, U>>(m).apply(scalar_sub_func<T, U>, scalar);
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator*(const Matrix<T>& m1, const Matrix<U>& m2) {
    if (m1.cols != m2.rows)
        throw invalid_argument("Matrix dimensions do not match for multiplication");

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
    return Matrix<common_type_t<T, U>>(m).apply(mul_scalar_func<T, U>, scalar);
}

template <typename T, typename U>
Matrix<common_type_t<T, U>> operator*(const U& scalar, const Matrix<T>& m) {
    return m * scalar;
}

template <typename T>
template <typename U>
bool Matrix<T>::operator==(const Matrix<U> &other) const {
    if (!is_rows_cols_equal(other)) return false;

    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            if (this->data[row][col] != other.data[row][col]) return false;
        }
    }
    return true;
}

template <typename T>
template <typename U>
bool Matrix<T>::operator!=(const Matrix<U> &other) const {
    return !(*this == other);
}

template <typename T>
void Matrix<T>::operator++() {
    Matrix<T> new_matrix = Matrix<T>(this->rows + 1, this->cols + 1);
    for (int row = 0; row < this->rows; ++row) {
        for (int col = 0; col < this->cols; ++col) {
            new_matrix.data[row + 1][col + 1] = this->data[row][col];
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
    for (int row = 0; row < this->rows - 1; ++row) {
        for (int col = 0; col < this->cols - 1; ++col) {
            new_matrix.data[row][col] = this->data[row + 1][col + 1];
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
    
    if (n == 0) {
        Matrix<T> identity(this->rows, this->cols);
        for (int i = 0; i < this->rows; ++i) {
            identity.data[i][i] = T{1};
        }
        return identity;
    }
    if (n == 1) {
        return *this;
    }
    
    Matrix<T> result = *this;
    for (int i = 1; i < n; ++i) {
        result = result * (*this);
    }
    return result;
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

// ===== MAIN FUNCTION =====
int main() {
    cout << "=== Matrix Class Testing Program ===" << endl << endl;
    
    try {
        // Test 1: Default Constructor
        cout << "Test 1: Default Constructor" << endl;
        Matrix<int> m1;
        cout << "Default matrix dimensions: " << m1.Rows() << "x" << m1.Cols() << endl << endl;
        
        // Test 2: Parameterized Constructor
        cout << "Test 2: Parameterized Constructor" << endl;
        Matrix<int> m2(3, 4);
        cout << "Matrix m2(3,4):" << endl << m2 << endl;
        
        // Test 3: Copy Constructor
        cout << "Test 3: Copy Constructor" << endl;
        Matrix<int> m3 = m2;
        cout << "Matrix m3 (copy of m2):" << endl << m3 << endl;
        
        // Test 4: Fill matrix with values
        cout << "Test 4: Fill matrix with values" << endl;
        for (int i = 0; i < m2.Rows(); ++i) {
            for (int j = 0; j < m2.Cols(); ++j) {
                m2(i, j) = i * m2.Cols() + j + 1;
            }
        }
        cout << "Matrix m2 filled with values:" << endl << m2 << endl;
        
        // Test 5: Assignment Operator
        cout << "Test 5: Assignment Operator" << endl;
        Matrix<int> m4;
        m4 = m2;
        cout << "Matrix m4 (assigned from m2):" << endl << m4 << endl;
        
        // Test 6: Move Constructor
        cout << "Test 6: Move Constructor" << endl;
        Matrix<int> m5 = Matrix<int>(2, 3);
        for (int i = 0; i < m5.Rows(); ++i) {
            for (int j = 0; j < m5.Cols(); ++j) {
                m5(i, j) = (i + 1) * 10 + (j + 1);
            }
        }
        cout << "Matrix m5 (moved):" << endl << m5 << endl;
        
        // Test 7: Arithmetic Operations
        cout << "Test 7: Arithmetic Operations" << endl;
        Matrix<int> m6(2, 2);
        Matrix<int> m7(2, 2);
        
        // Fill matrices
        m6(0, 0) = 1; m6(0, 1) = 2;
        m6(1, 0) = 3; m6(1, 1) = 4;
        
        m7(0, 0) = 5; m7(0, 1) = 6;
        m7(1, 0) = 7; m7(1, 1) = 8;
        
        cout << "Matrix m6:" << endl << m6 << endl;
        cout << "Matrix m7:" << endl << m7 << endl;
        
        // Addition
        Matrix<int> m8 = m6 + m7;
        cout << "m6 + m7:" << endl << m8 << endl;
        
        // Subtraction
        Matrix<int> m9 = m7 - m6;
        cout << "m7 - m6:" << endl << m9 << endl;
        
        // Scalar operations
        Matrix<int> m10 = m6 + 10;
        cout << "m6 + 10:" << endl << m10 << endl;
        
        Matrix<int> m11 = m6 * 2;
        cout << "m6 * 2:" << endl << m11 << endl;
        
        // Test 8: Matrix Multiplication
        cout << "Test 8: Matrix Multiplication" << endl;
        Matrix<int> m12(2, 3);
        Matrix<int> m13(3, 2);
        
        // Fill m12
        for (int i = 0; i < m12.Rows(); ++i) {
            for (int j = 0; j < m12.Cols(); ++j) {
                m12(i, j) = i * m12.Cols() + j + 1;
            }
        }
        
        // Fill m13
        for (int i = 0; i < m13.Rows(); ++i) {
            for (int j = 0; j < m13.Cols(); ++j) {
                m13(i, j) = (i + 1) * 2 + j;
            }
        }
        
        cout << "Matrix m12 (2x3):" << endl << m12 << endl;
        cout << "Matrix m13 (3x2):" << endl << m13 << endl;
        
        Matrix<int> m14 = m12 * m13;
        cout << "m12 * m13:" << endl << m14 << endl;
        
        // Test 9: Comparison Operators
        cout << "Test 9: Comparison Operators" << endl;
        Matrix<int> m15 = m6;
        cout << "m6 == m15: " << (m6 == m15 ? "true" : "false") << endl;
        cout << "m6 == m7: " << (m6 == m7 ? "true" : "false") << endl;
        cout << "m6 != m7: " << (m6 != m7 ? "true" : "false") << endl << endl;
        
        // Test 10: Increment/Decrement Operators
        cout << "Test 10: Increment/Decrement Operators" << endl;
        Matrix<int> m16(2, 2);
        m16(0, 0) = 1; m16(0, 1) = 2;
        m16(1, 0) = 3; m16(1, 1) = 4;
        
        cout << "Original m16:" << endl << m16 << endl;
        cout << "Dimensions: " << m16.Rows() << "x" << m16.Cols() << endl;
        
        ++m16;
        cout << "After ++m16:" << endl << m16 << endl;
        cout << "Dimensions: " << m16.Rows() << "x" << m16.Cols() << endl;
        
        --m16;
        cout << "After --m16:" << endl << m16 << endl;
        cout << "Dimensions: " << m16.Rows() << "x" << m16.Cols() << endl << endl;
        
        // Test 11: Transpose
        cout << "Test 11: Transpose" << endl;
        Matrix<int> m17(2, 3);
        for (int i = 0; i < m17.Rows(); ++i) {
            for (int j = 0; j < m17.Cols(); ++j) {
                m17(i, j) = i * m17.Cols() + j + 1;
            }
        }
        cout << "Original m17:" << endl << m17 << endl;
        cout << "Transposed m17:" << endl << m17.Transpose() << endl << endl;
        
        // Test 12: Power Operator
        cout << "Test 12: Power Operator" << endl;
        Matrix<int> m18(2, 2);
        m18(0, 0) = 1; m18(0, 1) = 1;
        m18(1, 0) = 1; m18(1, 1) = 0;
        
        cout << "Matrix m18:" << endl << m18 << endl;
        Matrix<int> m19 = m18^2;
        cout << "m18^2:" << endl << m19 << endl;
        Matrix<int> m20 = m18^3;
        cout << "m18^3:" << endl << m20 << endl << endl;
        
        // Test 13: Apply Function
        cout << "Test 13: Apply Function" << endl;
        Matrix<int> m21(2, 2);
        m21(0, 0) = 1; m21(0, 1) = 2;
        m21(1, 0) = 3; m21(1, 1) = 4;
        
        cout << "Original m21:" << endl << m21 << endl;
        
        // Test 14: Compound Assignment Operators
        cout << "Test 14: Compound Assignment Operators" << endl;
        Matrix<int> m22(2, 2);
        Matrix<int> m23(2, 2);
        
        m22(0, 0) = 1; m22(0, 1) = 2;
        m22(1, 0) = 3; m22(1, 1) = 4;
        
        m23(0, 0) = 1; m23(0, 1) = 1;
        m23(1, 0) = 1; m23(1, 1) = 1;
        
        cout << "m22:" << endl << m22 << endl;
        cout << "m23:" << endl << m23 << endl;
        
        m22 += m23;
        cout << "After m22 += m23:" << endl << m22 << endl;
        
        m22 -= m23;
        cout << "After m22 -= m23:" << endl << m22 << endl;
        
        m22 *= 2;
        cout << "After m22 *= 2:" << endl << m22 << endl << endl;
        
        // Test 15: Access Operators
        cout << "Test 15: Access Operators" << endl;
        Matrix<int> m24(2, 2);
        m24(0, 0) = 10; m24(0, 1) = 20;
        m24(1, 0) = 30; m24(1, 1) = 40;
        
        cout << "Matrix m24:" << endl << m24 << endl;
        cout << "m24[0][1] = " << m24[0][1] << endl;
        cout << "m24(1, 0) = " << m24(1, 0) << endl << endl;
        
        // Test 16: Error Handling
        cout << "Test 16: Error Handling" << endl;
        try {
            Matrix<int> m25(2, 2);
            Matrix<int> m26(3, 3);
            Matrix<int> m27 = m25 + m26; // Should throw exception
        } catch (const invalid_argument& e) {
            cout << "Caught expected exception: " << e.what() << endl;
        }
        
        try {
            Matrix<int> m28(2, 2);
            Matrix<int> m29(3, 2);
            Matrix<int> m30 = m28 * m29; // Should throw exception
        } catch (const invalid_argument& e) {
            cout << "Caught expected exception: " << e.what() << endl;
        }
        
        try {
            Matrix<int> m31(1, 1);
            --m31; // Should throw exception
        } catch (const invalid_argument& e) {
            cout << "Caught expected exception: " << e.what() << endl;
        }
        
        cout << endl << "=== All tests completed successfully! ===" << endl;
        
    } catch (const exception& e) {
        cout << "Unexpected error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}

// Explicit instantiations for common cases
template class Matrix<int>;
template Matrix<int> operator+<int>(const Matrix<int>&);
template Matrix<int> operator-<int>(const Matrix<int>&);
template Matrix<common_type_t<int, int>> operator+<int, int>(const Matrix<int>&, const Matrix<int>&);
template Matrix<common_type_t<int, int>> operator-<int, int>(const Matrix<int>&, const Matrix<int>&);
template Matrix<common_type_t<int, int>> operator*<int, int>(const Matrix<int>&, const Matrix<int>&);
template Matrix<common_type_t<int, int>> operator+<int, int>(const Matrix<int>&, const int&);
template Matrix<common_type_t<int, int>> operator*<int, int>(const Matrix<int>&, const int&);

