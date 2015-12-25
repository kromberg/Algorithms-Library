#ifndef LONGEST_INCR_SUBSEQ_H
#define LONGEST_INCR_SUBSEQ_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

class LongestIncrSubseq
{
private:
    std::vector<uint32_t> m_elements;

public:
    LongestIncrSubseq();
    ~LongestIncrSubseq();

    bool runAlgorithm(uint32_t& res /*todo: sequence*/);

    friend std::istream& operator>>(std::istream& in, LongestIncrSubseq& l);
};

#endif // LONGEST_INCR_SUBSEQ_H