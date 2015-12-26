#ifndef SANSA_AND_XOR_H
#define SANSA_AND_XOR_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

class SansaAndXOR
{
private:
    uint32_t m_numElements;
    std::vector<uint32_t> m_elements;

public:
    SansaAndXOR();
    ~SansaAndXOR();

    bool runAlgorithm(uint32_t& res);

    friend std::istream& operator>> (std::istream& in, SansaAndXOR& sansaAndXor);
};

#endif // SANSA_AND_XOR_H