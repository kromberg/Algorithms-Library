#include <algorithm>

#include "MaxWIS.h"


MaxWIS::MaxWIS()
{
}


MaxWIS::~MaxWIS()
{
}


bool MaxWIS::read(std::istream& in)
{
    size_t numVertices = 0;
    in >> numVertices;
    m_vertices.resize(numVertices);

    for (auto &v : m_vertices)
    {
        in >> v;
    }

    return true;
}

bool MaxWIS::run(uint32_t& res, std::list<std::pair<size_t, uint32_t> >& resList)
{
    // create support structure
    std::vector<uint32_t> wis;
    wis.resize(m_vertices.size() + 1, 0);
    // initialize wis
    wis[0] = 0;
    wis[1] = m_vertices[0];

    uint32_t currentIdx = 2;

    size_t numVertices = m_vertices.size();
    while (currentIdx <= numVertices)
    {
        wis[currentIdx++] = std::max(wis[currentIdx - 1], wis[currentIdx - 2] + m_vertices[currentIdx - 1]);
    }
    res = wis[numVertices - 1];

    // reconstruction of the list
    for (size_t i = numVertices; i >= 2;)
    {
        if (wis[i - 1] >= wis[i - 2] + m_vertices[i - 1])
        {
            if (2 == i)
            {
                resList.push_front(std::make_pair(0, m_vertices[0]));
            }
            --i;
        }
        else
        {
            resList.push_front(std::make_pair(i - 1, m_vertices[i - 1]));
            i -= 2;
        }
    }

    return true;
}