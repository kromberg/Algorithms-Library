#include "BitsVector.h"

const uint64_t BitsVector::getCellSize()
{
    return (sizeof(uint64_t) * 8);
}

BitsVector::BitsVector():
    m_bits(1, 0)
{
}


BitsVector::~BitsVector()
{
}

void BitsVector::clear()
{
    m_bits.resize(1);
    memset(&m_bits[0], 0, sizeof(uint64_t));
}

bool BitsVector::next()
{
    // TODO:
    return false;
}

void BitsVector::set(const uint64_t pos)
{
    uint64_t cell = pos / getCellSize();
    uint64_t cellPos = pos % getCellSize();
    if (cell + 1 > m_bits.size())
    {
        m_bits.resize(cell + 1, 0);
    }
    m_bits[cell] |= (1 << cellPos);
}

void BitsVector::unset(const uint64_t pos)
{
    uint64_t cell = pos / getCellSize();
    uint64_t cellPos = pos % getCellSize();
    if (cell + 1 > m_bits.size())
    {
        return;
    }
    m_bits[cell] ^= (1 << cellPos);
}

bool BitsVector::operator> (const BitsVector& bitsVector)
{
    uint64_t ownSize = m_bits.size();
    uint64_t otherSize = bitsVector.m_bits.size();
    if (ownSize == otherSize)
    {
        for (uint64_t i = 0; i < ownSize; ++i)
        {
            if (m_bits[i] > bitsVector.m_bits[i])
            {
                return true;
            }
            else if (m_bits[i] < bitsVector.m_bits[i])
            {
                return false;
            }
            // other - equal - continue checking
        }
    }
    else if (ownSize > otherSize)
    {
        for (uint64_t i = ownSize; i >= 1; --i)
        {
            if (i > otherSize)
            {
                if (m_bits[i - 1] > 0)
                {
                    return true;
                }
            }
            else // (i <= otherSize)
            {
                if (m_bits[i - 1] > bitsVector.m_bits[i - 1])
                {
                    return true;
                }
                else if (m_bits[i - 1] < bitsVector.m_bits[i - 1])
                {
                    return false;
                }
            }
        }
    }
    else //  (ownSize < otherSize)
    {
        for (uint64_t i = otherSize; i >= 1; --i)
        {
            if (i > ownSize)
            {
                if (bitsVector.m_bits[i - 1] > 0)
                {
                    return false;
                }
            }
            else // (i <= ownSize)
            {
                if (m_bits[i - 1] > bitsVector.m_bits[i - 1])
                {
                    return true;
                }
                else if (m_bits[i - 1] < bitsVector.m_bits[i - 1])
                {
                    return false;
                }
            }
        }
    }
    return false;
}

bool BitsVector::operator>= (const BitsVector& bitsVector)
{
    return (*this > bitsVector) || (*this == bitsVector);
}

bool BitsVector::operator< (const BitsVector& bitsVector)
{
    return !(*this >= bitsVector);
}

bool BitsVector::operator<= (const BitsVector& bitsVector)
{
    return !(*this > bitsVector);
}

bool BitsVector::operator== (const BitsVector& bitsVector)
{
    uint64_t ownSize = m_bits.size();
    uint64_t otherSize = bitsVector.m_bits.size();
    if (ownSize == otherSize)
    {
        return (m_bits == bitsVector.m_bits);
    }
    else
    {
        const uint64_t& maxSize = (ownSize > otherSize) ? ownSize : otherSize;
        const uint64_t& minSize = (ownSize < otherSize) ? ownSize : otherSize;
        const std::vector<uint64_t>& maxBitsVector = (ownSize > otherSize) ? m_bits : bitsVector.m_bits;
        const std::vector<uint64_t>& minBitsVector = (ownSize < otherSize) ? m_bits : bitsVector.m_bits;
        for (uint64_t i = maxSize; i >= 1; --i)
        {
            if (i > minSize)
            {
                if (maxBitsVector[i - 1] != 0)
                {
                    return false;
                }
            }
            else // (i <= minSize)
            {
                if (maxBitsVector[i - 1] != minBitsVector[i - 1])
                {
                    return false;
                }
            }
        }
        return true;
    }
}

bool BitsVector::operator!= (const BitsVector& bitsVector)
{
    return !(*this == bitsVector);
}