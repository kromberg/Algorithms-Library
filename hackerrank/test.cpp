/*****************************************************************************
THIS IS TEST FILE FOR CONNECTING RESULTS IN ONE FILE
******************************************************************************/
#if 0
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class Clique
{
private:
    uint32_t m_numVertices;
    uint32_t m_numEdges;

public:
    Clique()
    {
    }


    ~Clique()
    {
    }

    bool runAlgorithm(uint32_t& cliqueNumVertices)
    {
        for (uint32_t r = 1; r <= m_numVertices; ++r)
        {
            uint32_t tmp1 = static_cast<uint32_t>(ceil(static_cast<long double>(m_numVertices) / r));
            tmp1 *= tmp1;
            uint32_t tmp2 = static_cast<uint32_t>(floor(static_cast<long double>(m_numVertices) / r));
            tmp2 *= tmp2;
            uint32_t tmp3 = m_numVertices % r;
            uint32_t numVerticesForClique = (m_numVertices * m_numVertices - tmp3 * tmp1 - (r - tmp3) * tmp2) / 2;
            if (numVerticesForClique >= m_numEdges)
            {
                cliqueNumVertices = r;
                return true;
            }
        }
        return false;
    }

    friend std::istream& operator>> (std::istream& in, Clique& clique);
};
std::istream& operator>> (std::istream& in, Clique& clique)
{
    return in >> clique.m_numVertices >> clique.m_numEdges;
}

void CliqueFunc()
{
    Clique clique;
    uint32_t numTestcases = 0;
    std::cin >> numTestcases;
    while (numTestcases--)
    {
        std::cin >> clique;
        uint32_t res = 0;
        clique.runAlgorithm(res);
        std::cout << res << std::endl;
    }

    //std::cin.get();
}

int main(int argc, const char * argv[])
{
    CliqueFunc();

    return 0;
}
#endif