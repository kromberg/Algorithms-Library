#ifndef CLIQUE_H
#define CLIQUE_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cmath>

class Clique
{
private:
    uint32_t m_numVertices;
    uint32_t m_numEdges;

public:
    Clique();
    ~Clique();

    bool runAlgorithm(uint32_t& cliqueNumVertices);

    friend std::istream& operator>> (std::istream& in, Clique& clique);
};

#endif // CLIQUE_H