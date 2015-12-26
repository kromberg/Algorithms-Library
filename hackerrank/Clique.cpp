#include "Clique.h"

Clique::Clique()
{
}


Clique::~Clique()
{
}

bool Clique::runAlgorithm(uint32_t& cliqueNumVertices)
{
    for (uint32_t r = 1; r <= m_numVertices; ++r)
    {
        uint32_t tmp1 = static_cast<uint32_t>(ceil(static_cast<long double>(m_numVertices) / r));
        tmp1 *= tmp1;
        uint32_t tmp2 = static_cast<uint32_t>(floor(static_cast<long double>(m_numVertices) / r));
        tmp2 *= tmp2;
        uint32_t tmp3 = m_numVertices % r;
        // Turan's formula
        // 1 / 2  * (n ^ 2 - (n mod r) * {upper bound of n / r} ^ 2 - (r - (n mod r)) * {lower bound of n / r} ^ 2)
        // n = num of vertices
        uint32_t numVerticesForClique = (m_numVertices * m_numVertices - tmp3 * tmp1 - (r - tmp3) * tmp2) / 2;
        if (numVerticesForClique >= m_numEdges)
        {
            cliqueNumVertices = r;
            return true;
        }
    }
    return false;
}

std::istream& operator>> (std::istream& in, Clique& clique)
{
    return in >> clique.m_numVertices >> clique.m_numEdges;
}