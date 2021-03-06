#include "SansaAndXOR.h"


SansaAndXOR::SansaAndXOR()
{
}

SansaAndXOR::~SansaAndXOR()
{
}

bool SansaAndXOR::runAlgorithm(uint32_t& res)
{
    res = 0;
    uint32_t power = 0;
    for (uint32_t k = 0; k < m_numElements; ++k)
    {
        if (k + 1 < m_numElements / 2)
        {
            power = m_numElements * (k + 1) - (k + 1) * k;
            if (power & 1)
            {
                res ^= m_elements[k];
            }
        }
        else
        {
            uint32_t m = m_numElements + 1 - (k + 1);
            power = m_numElements * m - m * (m - 1);
            if (power & 1)
            {
                res ^= m_elements[k];
            }
        }
    }
    return true;
}

std::istream& operator>> (std::istream& in, SansaAndXOR& sansaAndXor)
{
    in >> sansaAndXor.m_numElements;
    sansaAndXor.m_elements.resize(sansaAndXor.m_numElements);
    for (auto& element : sansaAndXor.m_elements)
    {
        in >> element;
    }
    return in;
}