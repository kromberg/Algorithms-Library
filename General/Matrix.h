#ifndef MATRIX_H
#define MATRIX_H

#include <memory>
#include <cstdint>
#include <exception>
#include <algorithm>

template<typename T>
class Matrix
{
public:
    typedef T ValueType;
private:
    uint64_t m_rowsCount;
    uint64_t m_colsCount;

    std::unique_ptr<ValueType[]> m_matrix;

public:
    Matrix();
    Matrix(uint64_t rowsCount, uint64_t colsCount);
    Matrix(const Matrix<ValueType>& matrix);
    Matrix(Matrix<ValueType>&& matrix);
    ~Matrix();
    
    ValueType* operator[] (uint64_t i);

    void reset();
    void resize(uint64_t rowsCount, uint64_t colsCount);

    const uint64_t getRowsCount() const;
    const uint64_t getColsCount() const;

    template <typename U>
    friend std::ostream& operator << (std::ostream& out, Matrix<U>& matrix);
};

template<typename T>
inline const uint64_t Matrix<T>::getRowsCount() const
{
    return m_rowsCount;
}

template<typename T>
inline const uint64_t Matrix<T>::getColsCount() const
{
    return m_colsCount;
}

#include "Matrix.hpp"

#endif