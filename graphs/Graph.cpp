//
//  Graph.cpp
//  graphs
//
//  Created by Egor Kromberg on 13.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#include <stdio.h>

#include "Graph.h"


Graph::Graph()
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
        edge->m_begin->m_edgesList.push_back(edge);
        edge->m_cost = cost;
        m_edges.push_back(edge);

        if (!directed)
        {
            Edge* reverseEdge = new Edge;
            reverseEdge->m_begin = verticesVector[end - 1];
            reverseEdge->m_end = verticesVector[begin - 1];
            reverseEdge->m_begin->m_edgesList.push_back(reverseEdge);
            reverseEdge->m_cost = cost;
            m_edges.push_back(reverseEdge);
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
            if (!edge->m_end->m_explored)
            {
                edge->m_end->m_explored = true;
                verticesQueue.push_back(edge->m_end);
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
            if (!edge->m_end->m_explored)
            {
                edge->m_end->m_explored = true;
                verticesQueue.push_back(edge->m_end);
            }
        }
    }

    return true;
}
