#ifndef JACK_GOES_TO_RAPTURE_H
#define JACK_GOES_TO_RAPTURE_H

#include <cstdint>
#include <vector>
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
    JackGoesToRapture();
    ~JackGoesToRapture();

    bool runAlgorithm(int32_t& res);

    friend std::istream& operator>> (std::istream& in, JackGoesToRapture& jackGoesToRapture);
};

#endif // JACK_GOES_TO_RAPTURE_H