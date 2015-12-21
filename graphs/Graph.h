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

// forward declarartion
struct Vertex;
struct Edge;

typedef std::list<Vertex*> VerticesList;
typedef std::vector<Vertex*> VerticesVector;
typedef std::list<Edge*> EdgesList;
typedef std::map<uint32_t, Edge*> EdgesMap;

struct EdgeScore
{
    Edge* m_edge;
    int32_t m_score;
    EdgeScore(Edge* edge, int32_t score):
        m_edge(edge),
        m_score(score)
    {}
    EdgeScore(EdgeScore&& es)
    {
        m_edge = es.m_edge;
        m_score = es.m_score;
        es.m_edge = nullptr;
        es.m_score = 0;
    }
};

struct Vertex
{
    EdgesMap m_edgesList;
    uint32_t m_id;
    bool m_explored;
    
    Vertex(const uint32_t id):
        m_edgesList(),
        m_id(id),
        m_explored(false)
    {}
    ~Vertex()
    {
        m_edgesList.clear();
    }
};

struct Edge
{
    Vertex* m_begin;
    Vertex* m_end;
    int32_t m_cost;
};

class Graph
{
private:
    VerticesList m_vertices;
    EdgesList m_edges;
    bool m_isDirected;

public:
    Graph();
    ~Graph();
    
    void clear();
    
    bool read(std::istream& in);
    bool write(std::ostream& out);
    
    const VerticesList& getVertices() const;
    const EdgesList& getEdges() const;
    const bool isDirected() const;
    
    bool BFS(Vertex* startingVertex);
    bool DFS(Vertex* startingVertex);
    bool Dijkstra(Vertex* startingVertex, Vertex* endVertex, int32_t& length);

    bool doesVertexCoverExists(const uint32_t k);
};

inline const VerticesList& Graph::getVertices() const
{
    return m_vertices;
}

inline const EdgesList& Graph::getEdges() const
{
    return m_edges;
}

inline const bool Graph::isDirected() const
{
    return m_isDirected;
}

#endif /* GRAPH_H */
