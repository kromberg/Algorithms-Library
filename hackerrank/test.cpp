/*****************************************************************************
THIS IS TEST FILE FOR CONNECTING RESULTS IN ONE FILE
******************************************************************************/
#if 0

#include <cstdint>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>

class JackGoesToRapture
{
private:
    // graph
    struct Edge
    {
        int32_t m_end;
        int32_t m_cost;
        Edge():
            m_end(-1),
            m_cost(-1)
        {}
        Edge(const int32_t end, const int32_t cost) :
            m_end(end),
            m_cost(cost)
        {}
        Edge(const Edge& edge) :
            m_end(edge.m_end),
            m_cost(edge.m_cost)
        {}
        Edge& operator=(const Edge& edge)
        {
            if (this == &edge)
            {
                return *this;
            }
            m_end = edge.m_end;
            m_cost = edge.m_cost;
            return *this;
        }
        Edge(Edge&& edge) :
            m_end(edge.m_end),
            m_cost(edge.m_cost)
        {
            edge.m_end = -1;
            edge.m_cost = -1;
        }
        Edge& operator=(Edge&& edge)
        {
            if (this == &edge)
            {
                return *this;
            }
            m_end = edge.m_end;
            m_cost = edge.m_cost;
            edge.m_end = -1;
            edge.m_cost = -1;
            return *this;
        }
    };
    struct Vertex
    {
        std::list<Edge> m_edges;
    };

    uint32_t m_numVertices;
    uint32_t m_numEdges;
    std::vector<Vertex> m_vertices;
    //std::list<Edge> m_edges;

public:
    JackGoesToRapture()
    {
    }

    ~JackGoesToRapture()
    {
    }

    bool runAlgorithm(int32_t& res)
    {
        std::vector<int32_t> lengths[2];
        lengths[0].resize(m_numVertices, -1);
        lengths[1].resize(m_numVertices, -1);
        lengths[0][0] = 0;

        int32_t minVal = -1;
        for (uint32_t i = 1; i <= m_numEdges; ++i)
        {
            for (uint32_t v = 0; v < m_numVertices; ++v)
            {
                minVal = -1;
                Vertex& vertex = m_vertices[v];
                for (auto& edge : vertex.m_edges)
                {
                    if (lengths[(i - 1) % 2][edge.m_end] != -1)
                    {
                        int32_t length = std::max(lengths[(i - 1) % 2][edge.m_end], edge.m_cost);
                        if ((minVal == -1) ||
                            (minVal > length))
                        {
                            minVal = length;
                        }
                    }
                }
                if (lengths[(i - 1) % 2][v] != -1)
                {
                    if ((minVal == -1) ||
                        (minVal > lengths[(i - 1) % 2][v]))
                    {
                        minVal = lengths[(i - 1) % 2][v];
                    }
                }
                lengths[i % 2][v] = minVal;
            }
        }
        if (lengths[m_numEdges % 2][m_numVertices - 1] == -1)
        {
            return false;
        }
        res = lengths[m_numEdges % 2][m_numVertices - 1];

        return true;
    }

    friend std::istream& operator>> (std::istream& in, JackGoesToRapture& jackGoesToRapture);
};

std::istream& operator>> (std::istream& in, JackGoesToRapture& jackGoesToRapture)
{
    in >> jackGoesToRapture.m_numVertices >> jackGoesToRapture.m_numEdges;
    jackGoesToRapture.m_vertices.resize(jackGoesToRapture.m_numVertices);
    int32_t begin, end, val;
    for (uint32_t i = 0; i < jackGoesToRapture.m_numEdges; ++i)
    {
        in >> begin >> end >> val;
        jackGoesToRapture.m_vertices[begin - 1].m_edges.push_back(std::move(JackGoesToRapture::Edge(end - 1, val)));
        jackGoesToRapture.m_vertices[end - 1].m_edges.push_back(std::move(JackGoesToRapture::Edge(begin - 1, val)));
    }
    return in;
}


void JackGoesToRaptureFunc()
{
    JackGoesToRapture jackGoesToRapture;
    std::cin >> jackGoesToRapture;
    int32_t res = 0;
    jackGoesToRapture.runAlgorithm(res);
    std::cout << res << std::endl;

    std::cin.get();
}

int main(int argc, const char * argv[])
{
    JackGoesToRaptureFunc();

    return 0;
}

#endif