//
//  Graph.cpp
//  graphs
//
//  Created by Egor Kromberg on 13.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#include <random>
#include <stdio.h>
#include <queue>
#include <stack>

#include "Graph.h"

Graph::Edge::Edge():
    m_begin(-1),
    m_end(-1),
    m_cost(-1)
{}

Graph::Edge::Edge(const int32_t begin, const int32_t end, const int32_t cost) :
    m_begin(begin),
    m_end(end),
    m_cost(cost)
{}

Graph::Edge::Edge(const Edge& edge):
    m_begin(edge.m_begin),
    m_end(edge.m_end),
    m_cost(edge.m_cost)
{}

Graph::Edge& Graph::Edge::operator= (const Edge& edge)
{
    if (this == &edge)
    {
        return *this;
    }
    m_begin = edge.m_begin;
    m_end = edge.m_end;
    m_cost = edge.m_cost;
    return *this;
}

Graph::Edge::Edge(Edge&& edge) :
    m_begin(edge.m_begin),
    m_end(edge.m_end),
    m_cost(edge.m_cost)
{
    edge.m_begin = -1;
    edge.m_end = -1;
    edge.m_cost = -1;
}

Graph::Edge& Graph::Edge::operator= (Edge&& edge)
{
    if (this == &edge)
    {
        return *this;
    }
    m_begin = edge.m_begin;
    m_end = edge.m_end;
    m_cost = edge.m_cost;
    edge.m_begin = -1;
    edge.m_end = -1;
    edge.m_cost = -1;
    return *this;
}

Graph::Vertex::Vertex() :
    m_explored(false),
    m_edges()
{}

Graph::Vertex::Vertex(const Vertex& vertex):
    m_explored(vertex.m_explored),
    m_edges(vertex.m_edges)
{}

Graph::Vertex& Graph::Vertex::operator= (const Vertex& vertex)
{
    if (this == &vertex)
    {
        return *this;
    }
    m_explored = vertex.m_explored;
    m_edges = vertex.m_edges;
    return *this;
}

Graph::Vertex::Vertex(Vertex&& vertex):
    m_explored(vertex.m_explored)
{
    m_edges = std::move(vertex.m_edges);
    vertex.m_explored = false;
}
Graph::Vertex& Graph::Vertex::operator=(Vertex&& vertex)
{
    if (this == &vertex)
    {
        return *this;
    }
    m_explored = vertex.m_explored;
    m_edges = std::move(vertex.m_edges);
    vertex.m_explored = false;
    return *this;
}

Graph::Vertex::~Vertex()
{
    m_edges.clear();
}

Graph::Graph():
    m_isDirected(false)
{}

Graph::Graph(const Graph &graph):
    m_isDirected(graph.m_isDirected),
    m_vertices(graph.m_vertices),
    m_edges(graph.m_edges)
{
}

Graph& Graph::operator=(const Graph &graph)
{
    if (this == &graph)
    {
        return *this;
    }
    m_isDirected = graph.m_isDirected;
    m_vertices = graph.m_vertices;
    m_edges = graph.m_edges;
    return *this;
}

Graph::~Graph()
{
    clear();
}

void Graph::clear()
{
    m_vertices.clear();
    m_edges.clear();

    m_isDirected = false;
}

bool Graph::read(std::istream& in)
{
    clear();

    uint32_t numVertices = 0, numEdges = 0;
    in >> numVertices >> numEdges;
    m_vertices.resize(numVertices);
    for (uint32_t i = 0; i < numEdges; ++i)
    {
        // format
        // directed? begin end cost
        bool directed = false;
        int32_t begin = 0, end = 0;
        int32_t cost = 0;
        in >> directed >> begin >> end >> cost;

        Edge edge(begin - 1, end - 1, cost);
        m_edges.push_back(edge);
        m_vertices[begin - 1].m_edges.push_back(edge);

        if (!directed)
        {
            Edge reverseEdge(end - 1, begin - 1, cost);
            m_edges.push_back(reverseEdge);
            m_vertices[end - 1].m_edges.push_back(reverseEdge);
        }
        else
        {
            m_isDirected = true;
        }
    }
    return true;
}

bool Graph::write(std::ostream& out)
{
    out << "Num of vertices: " << m_vertices.size() << std::endl;
    uint32_t i = 1;
    for (auto& vertex : m_vertices)
    {
        out << i << " : " << vertex.m_explored << std::endl;
        ++i;
    }
    out << "Num of edges: " << m_edges.size() << std::endl;
    for (auto& edge : m_edges)
    {
        out << "Begin: " << edge.m_begin <<
               " End: " << edge.m_end <<
               " Cost: " << edge.m_cost << std::endl;
    }
    return true;
}

bool Graph::BFS(uint32_t startingVertex)
{
    if (startingVertex == 0 ||
        startingVertex > m_vertices.size())
    {
        return false;
    }
    --startingVertex;

    // set all vertices as unxplored
    for (auto& vertex: m_vertices)
    {
        vertex.m_explored = false;
    }
    
    std::queue<uint32_t> verticesQueue;
    m_vertices[startingVertex].m_explored = true;
    verticesQueue.push(startingVertex);
    
    while (!verticesQueue.empty())
    {
        uint32_t currentVertex = verticesQueue.front();
        verticesQueue.pop();
        
        for (auto& edge: m_vertices[currentVertex].m_edges)
        {
            if (!m_vertices[edge.m_end].m_explored)
            {
                m_vertices[edge.m_end].m_explored = true;
                verticesQueue.push(edge.m_end);
            }
        }
    }
    
    return true;
}

bool Graph::DFS(uint32_t startingVertex)
{
    if (startingVertex == 0 ||
        startingVertex > m_vertices.size())
    {
        return false;
    }
    --startingVertex;

    // set all vertices as unxplored
    for (auto& vertex: m_vertices)
    {
        vertex.m_explored = false;
    }
    
    std::stack<uint32_t> verticesStack;
    m_vertices[startingVertex].m_explored = true;
    verticesStack.push(startingVertex);
    
    while (!verticesStack.empty())
    {
        uint32_t currentVertex = verticesStack.top();
        verticesStack.pop();
        
        for (auto& edge : m_vertices[currentVertex].m_edges)
        {
            if (!m_vertices[edge.m_end].m_explored)
            {
                m_vertices[edge.m_end].m_explored = true;
                verticesStack.push(edge.m_end);
            }
        }
    }

    return true;
}

bool Graph::DFSv2(uint32_t startingVertex, std::vector<uint32_t>& labels, uint32_t &currentLabel)
{
    m_vertices[startingVertex].m_explored = true;
    for (auto& edge : m_vertices[startingVertex].m_edges)
    {
        if (!m_vertices[edge.m_end].m_explored)
        {
            DFSv2(edge.m_end, labels, currentLabel);
        }
        labels[startingVertex] = currentLabel;
        currentLabel--;
    }
    return true;
}

bool Graph::DFSLoop(std::vector<uint32_t> &labels)
{
    // set all vertices as unxplored
    for (auto& vertex : m_vertices)
    {
        vertex.m_explored = false;
    }
    labels.resize(m_vertices.size());
    std::fill(labels.begin(), labels.end(), 0);
    uint32_t numVertices = static_cast<uint32_t>(m_vertices.size());
    uint32_t currentLabel = numVertices;
    for (uint32_t vertexId = 0; vertexId < numVertices; ++vertexId)
    {
        if (!m_vertices[vertexId].m_explored)
        {
            DFSv2(vertexId, labels, currentLabel);
        }
    }
    return true;
}

bool Graph::doesVertexCoverExists(const uint32_t k)
{
    if (m_isDirected)
    {
        return false;
    }

    /* TODO: Algorithm:
    Choose arbitrary edge.
    1. Delete the first vertex of the edge
        1.1. Recursively calculate doesVertexCoverExists(k - 1)
        1.2. Success -> return
    2. Delete the second vertex of the edge
        2.1. Recursively calculate doesVertexCoverExists(k - 1)
        2.2. Success -> return
    3. Unsuccessful
    */
    return false;

#if 0
    if (0 == k)
    {
        if (m_vertices.size() > 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    // Seed with a real random value, if available
    std::random_device rd;
    // Choose a random mean between 0 and m_edges.size() - 1
    std::default_random_engine e1(rd());
    std::uniform_int_distribution<size_t> uniform_dist(0, m_edges.size() - 1);
    size_t edgeIdx = uniform_dist(e1);
    {
        // copy Graph
        Graph graph = *this;
        EdgesList::iterator edgeIt = m_edges.begin();
        std::advance(edgeIt, edgeIdx);
        VerticesList::iterator vertextIt = m_vertices.begin();
        std::advance(vertextIt, (*edgeIt)->m_begin);
        // the first vertex
        for (auto &edge : (*vertextIt)->m_edgesList)
        {
            edge.second->m_end->m_edgesList.erase((*vertextIt)->m_id);
        }
        m_vertices.erase(vertextIt);
        EdgesList::iterator edgeIt2;
        if (edgeIt == m_edges.begin())
        {
            edgeIt2 = edgeIt;
            ++edgeIt2;
        }
        else
        {
            edgeIt2 = edgeIt;
            --edgeIt2;
            if ((*edgeIt2)->m_begin != (*edgeIt)->m_end ||
                (*edgeIt2)->m_end != (*edgeIt)->m_begin)
            {
                edgeIt2 = edgeIt;
                ++edgeIt2;
            }
        }
        m_edges.erase(edgeIt);
        m_edges.erase(edgeIt2);
        if (graph.doesVertexCoverExists(k - 1))
        {
            return true;
        }
    }
    {
        // copy Graph
        Graph graph = *this;
        EdgesList::iterator edgeIt = m_edges.begin();
        std::advance(edgeIt, edgeIdx);
        VerticesList::iterator vertextIt = m_vertices.begin();
        std::advance(vertextIt, (*edgeIt)->m_end);
        // the second vertex
        for (auto &edge : (*vertextIt)->m_edgesList)
        {
            edge.second->m_end->m_edgesList.erase((*vertextIt)->m_id);
        }
        m_vertices.erase(vertextIt);

        EdgesList::iterator edgeIt2;
        if (edgeIt == m_edges.begin())
        {
            edgeIt2 = edgeIt;
            ++edgeIt2;
        }
        else
        {
            edgeIt2 = edgeIt;
            --edgeIt2;
            if ((*edgeIt2)->m_begin != (*edgeIt)->m_end ||
                (*edgeIt2)->m_end != (*edgeIt)->m_begin)
            {
                edgeIt2 = edgeIt;
                ++edgeIt2;
            }
        }
        m_edges.erase(edgeIt);
        m_edges.erase(edgeIt2);
        if (graph.doesVertexCoverExists(k - 1))
        {
            return true;
        }
    }
#endif 
    return false;
}