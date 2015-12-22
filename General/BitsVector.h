#ifndef BITS_VECTOR_H
#define BITS_VECTOR_H

#include <vector>
#include <cstdint>

class BitsVector
{
private:
    std::vector<uint64_t> m_bits;
public:
    BitsVector();
    ~BitsVector();

    // todo: constexpr
    static const uint64_t getCellSize();

    void clear();

    bool next();

    void set(const uint64_t pos);
    void unset(const uint64_t pos);

    bool operator> (const BitsVector& bitsVector);
    bool operator>= (const BitsVector& bitsVector);
    bool operator< (const BitsVector& bitsVector);
    bool operator<= (const BitsVector& bitsVector);
    bool operator== (const BitsVector& bitsVector);
    bool operator!= (const BitsVector& bitsVector);
};

#endif // BITS_VECTOR_H