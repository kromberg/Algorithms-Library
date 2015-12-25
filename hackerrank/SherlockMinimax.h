#ifndef SHEKLOCK_MINIMAX_H
#define SHEKLOCK_MINIMAX_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

class SherlockMinimax
{
private:
    uint32_t m_numElements;
    std::vector<int32_t> m_elements;
    int32_t m_p;
    int32_t m_q;

public:
    SherlockMinimax();
    ~SherlockMinimax();

    bool runAlgorithm(int32_t& res, int32_t& val);

    friend std::istream& operator>> (std::istream& in, SherlockMinimax& sherlockMinimax);
};

#endif // SHEKLOCK_MINIMAX_H