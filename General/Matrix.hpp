template<typename T>
Matrix<T>::Matrix():
    m_rowsCount(0),
    m_colsCount(0),
    m_matrix()
{
}

template<typename T>
Matrix<T>::Matrix(uint64_t rowsCount, uint64_t colsCount) :
    m_rowsCount(rowsCount),
    m_colsCount(colsCount),
    m_matrix()
{
    m_matrix.reset(new ValueType[m_rowsCount * m_colsCount]);
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& matrix) :
    m_rowsCount(matrix.m_rowsCount),
    m_colsCount(matrix.m_colsCount),
    m_matrix()
{
    m_matrix.reset(new ValueType[m_rowsCount * m_colsCount]);
    memcpy(m_matrix.get(), matrix.m_matrix.get(), sizeof(ValueType) * m_rowsCount * m_colsCount);
}

template<typename T>
Matrix<T>::Matrix(Matrix&& matrix):
    m_rowsCount(matrix.m_rowsCount),
    m_colsCount(matrix.m_colsCount)
{
    m_matrix = std::move(matrix.m_matrix);
    matrix.m_rowsCount = 0;
    matrix.m_colsCount = 0;
}

template<typename T>
Matrix<T>::~Matrix()
{
    m_matrix.reset();
}

template<typename T>
T* Matrix<T>::operator[] (uint64_t i)
{
    return &m_matrix.get()[i * m_colsCount];
}

template<typename T>
void Matrix<T>::reset()
{
    m_matrix.reset();
    m_rowsCount = 0;
    m_colsCount = 0;
}

template<typename T>
void Matrix<T>::resize(uint64_t rowsCount, uint64_t colsCount)
{
    std::unique_ptr<ValueType[]> matrix(new ValueType[rowsCount * colsCount]);
    uint64_t minColsCount = std::min(m_colsCount, colsCount);
    uint64_t minRowsCount = std::min(m_rowsCount, rowsCount);
    for (uint64_t i = 0; i < minRowsCount; ++i)
    {
        memcpy(&matrix.get()[i * colsCount], &m_matrix.get()[i * m_colsCount], sizeof(ValueType) * minColsCount);
    }
    m_matrix.reset();
    m_rowsCount = rowsCount;
    m_colsCount = colsCount;
    m_matrix = std::move(matrix);
}

template<typename T>
void Matrix<T>::fill(ValueType val)
{
    uint64_t numElements = m_rowsCount * m_colsCount;
    for (uint64_t i = 0; i < numElements; ++i)
    {
        m_matrix[i] = val;
    }
}

template <typename U>
std::ostream& operator << (std::ostream& out, Matrix<U>& matrix)
{
    for (uint64_t i = 0; i < matrix.m_rowsCount; ++i)
    {
        for (uint64_t j = 0; j < matrix.m_colsCount; ++j)
        {
            if (j != 0)
            {
                out << ' ';
            }
            out << matrix[i][j];
        }
        out << std::endl;
    }
    return out;
}