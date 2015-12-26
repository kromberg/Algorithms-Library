/*****************************************************************************
THIS IS TEST FILE FOR CONNECTING RESULTS IN ONE FILE
******************************************************************************/
#if 0
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

class LongestIncrSubseq
{
private:
    std::vector<uint32_t> m_elements;
public:

    LongestIncrSubseq()
    {
    }


    ~LongestIncrSubseq()
    {
    }

    bool runOptimizedAlgorithm(uint32_t& res /*todo: sequence*/)
    {
        uint32_t numElements = static_cast<uint32_t>(m_elements.size());
        std::vector<uint32_t> sortedElements;
        sortedElements.reserve(numElements);
        sortedElements.push_back(m_elements[0]);
        res = 1;
        for (uint32_t i = 1; i < numElements; ++i)
        {
            if (m_elements[i] < sortedElements.front())
            {
                sortedElements.front() = m_elements[i];
            }
            else if (m_elements[i] > sortedElements.back())
            {
                sortedElements.push_back(m_elements[i]);
                ++res;
            }
            else
            {
                auto it = std::lower_bound(sortedElements.begin(), sortedElements.end(), m_elements[i]);
                if (sortedElements.end() != it)
                {
                    *it = m_elements[i];
                }
            }
        }

        return true;
    }



    friend std::istream& operator>>(std::istream& in, LongestIncrSubseq& l);
};

std::istream& operator>>(std::istream& in, LongestIncrSubseq& l)
{
    uint32_t numElements = 0;
    in >> numElements;
    l.m_elements.resize(numElements);
    for (auto& element : l.m_elements)
    {
        in >> element;
    }
    return in;
}

void LongestIncrSubseqFunc()
{
    LongestIncrSubseq longestIncrSubseq;
    std::cin >> longestIncrSubseq;
    uint32_t res = 0;
    longestIncrSubseq.runOptimizedAlgorithm(res);
    std::cout << res << std::endl;
}

int main(int argc, const char * argv[])
{
    LongestIncrSubseqFunc();

    return 0;
}
#endif 