//
//  Graph.cpp
//  graphs
//
//  Created by Egor Kromberg on 13.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#include <random>
#include <stdio.h>

#include "Graph.h"


Graph::Graph():
    m_isDirected(false)
{}

Graph::~Graph()
{
    clear();
}

void Graph::clear()
{
    while (!m_vertices.empty())
    {
        Vertex *vertex = m_vertices.front();
        m_vertices.pop_front();
        delete vertex;
    }
    
    while (!m_edges.empty())
    {
        Edge *edge = m_edges.front();
        m_edges.pop_front();
        delete edge;
    }

    m_isDirected = false;
}

bool Graph::read(std::istream& in)
{
    clear();

    uint32_t numVertices = 0, numEdges = 0;
    in >> numVertices >> numEdges;
    VerticesVector verticesVector;
    verticesVector.reserve(numVertices);
    for (uint32_t i = 0; i < numVertices; ++i)
    {
        Vertex* vertex = new Vertex(i + 1);
        m_vertices.push_back(vertex);
        verticesVector.push_back(vertex);
    }
    for (uint32_t i = 0; i < numEdges; ++i)
    {
        // format
        // directed? begin end cost
        bool directed = false;
        uint32_t begin = 0, end = 0;
        int32_t cost = 0;
        in >> directed >> begin >> end >> cost;

        Edge* edge = new Edge;
        edge->m_begin = verticesVector[begin - 1];
        edge->m_end = verticesVector[end - 1];
        edge->m_begin->m_edgesList.insert(std::make_pair(end - 1, edge));
        edge->m_cost = cost;
        m_edges.push_back(edge);

        if (!directed)
        {
            Edge* reverseEdge = new Edge;
            reverseEdge->m_begin = verticesVector[end - 1];
            reverseEdge->m_end = verticesVector[begin - 1];
            reverseEdge->m_begin->m_edgesList.insert(std::make_pair(begin - 1, edge));
            reverseEdge->m_cost = cost;
            m_edges.push_back(reverseEdge);
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
    for (auto& vertex : m_vertices)
    {
        out << vertex->m_id << " : " << vertex->m_explored << std::endl;
    }
    out << "Num of edges: " << m_edges.size() << std::endl;
    for (auto& edge : m_edges)
    {
        out << "Begin: " << edge->m_begin->m_id <<
               " End: " << edge->m_end->m_id <<
               " Cost: " << edge->m_cost << std::endl;
    }
    return true;
}

bool Graph::BFS(Vertex* startingVertex)
{
    if (!startingVertex)
    {
        return false;
    }
    // set all vertices as unxplored
    for (auto& vertex: m_vertices)
    {
        vertex->m_explored = false;
    }
    
    VerticesList verticesQueue;
    startingVertex->m_explored = true;
    verticesQueue.push_back(startingVertex);
    
    while (!verticesQueue.empty())
    {
        Vertex* currentVertex = verticesQueue.front();
        verticesQueue.pop_front();
        
        for (auto& edge: currentVertex->m_edgesList)
        {
            if (!edge.second->m_end->m_explored)
            {
                edge.second->m_end->m_explored = true;
                verticesQueue.push_back(edge.second->m_end);
            }
        }
    }
    
    return true;
}

bool Graph::DFS(Vertex* startingVertex)
{
    if (!startingVertex)
    {
        return false;
    }
    // set all vertices as unxplored
    for (auto& vertex: m_vertices)
    {
        vertex->m_explored = false;
    }
    
    VerticesList verticesQueue;
    startingVertex->m_explored = true;
    verticesQueue.push_back(startingVertex);
    
    while (!verticesQueue.empty())
    {
        Vertex* currentVertex = verticesQueue.back();
        verticesQueue.pop_back();
        
        for (auto& edge: currentVertex->m_edgesList)
        {
            if (!edge.second->m_end->m_explored)
            {
                edge.second->m_end->m_explored = true;
                verticesQueue.push_back(edge.second->m_end);
            }
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