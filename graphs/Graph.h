//
//  Graph.h
//  graphs
//
//  Created by Egor Kromberg on 13.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <cstdint>

class Graph
{
public:
    struct Edge
    {
        int32_t m_begin;
        int32_t m_end;
        int32_t m_cost;
        Edge();
        Edge(const int32_t begin, const int32_t end, const int32_t cost);
        Edge(const Edge& edge);
        Edge& operator= (const Edge& edge);
        Edge(Edge&& edge);
        Edge& operator= (Edge&& edge);
    };
    typedef std::list<Edge> EdgesList;
    struct Vertex
    {
        EdgesList m_edges;
        bool m_explored;

        Vertex();
        Vertex(const Vertex& vertex);
        Vertex& operator= (const Vertex& vertex);
        Vertex(Vertex&& vertex);
        Vertex& operator=(Vertex&& vertex);
        ~Vertex();
    };
    typedef std::vector<Vertex> VerticesVector;

private:
    VerticesVector m_vertices;
    EdgesList m_edges;
    bool m_isDirected;

    bool DFSv2(uint32_t startingVertex, std::vector<uint32_t>& labels, uint32_t &currentLabel);

public:
    Graph();
    ~Graph();
    Graph(const Graph &graph);
    Graph& operator=(const Graph &graph);
    
    void clear();
    
    bool read(std::istream& in);
    bool write(std::ostream& out);
    
    const VerticesVector& getVertices() const;
    const EdgesList& getEdges() const;
    const bool isDirected() const;
    
    bool BFS(uint32_t startingVertex);
    bool DFS(uint32_t startingVertex);
    bool DFSLoop(std::vector<uint32_t> &labels);
    bool Dijkstra(uint32_t startingVertex, uint32_t endVertex, int32_t& length);

    bool doesVertexCoverExists(const uint32_t k);
};

inline const Graph::VerticesVector& Graph::getVertices() const
{
    return m_vertices;
}

inline const Graph::EdgesList& Graph::getEdges() const
{
    return m_edges;
}

inline const bool Graph::isDirected() const
{
    return m_isDirected;
}

#endif /* GRAPH_H */
