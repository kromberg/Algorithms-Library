#ifndef ALMOST_SORTED_H
#define ALMOST_SORTED_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

class AlmostSorted
{
public:
    struct Anomaly
    {
        std::string m_what;
        std::pair<uint32_t, uint32_t> m_indices;
    };
private:
    uint32_t m_numElements;
    std::vector<uint32_t> m_array;

public:
    AlmostSorted();
    ~AlmostSorted();

    bool runAlgorithm(Anomaly& anomaly);

    friend std::istream& operator>>(std::istream& in, AlmostSorted& almostSorted);
};

#endif // ALMOST_SORTED_H