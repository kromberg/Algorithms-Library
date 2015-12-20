//
//  GraphMatrix.cpp
//  graphs
//
//  Created by Egor Kromberg on 14.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#include <stdio.h>
#include <list>

#include "GraphMatrix.h"

GraphMatrix::Utils::Dijkstra::End::End(const uint32_t end, const int32_t score) :
    m_end(end),
    m_score(score)
{}

GraphMatrix::Utils::Dijkstra::End::End(const End& e):
    m_end(e.m_end),
    m_score(e.m_score)
{}

GraphMatrix::Utils::Dijkstra::End::End(End&& e)
{
    m_end = e.m_end;
    m_score = e.m_score;
    e.m_end = 0;
    e.m_score = 0;
}

GraphMatrix::Utils::Prim::Edge::Edge(const uint32_t begin, const uint32_t end, const int32_t cost) :
    m_begin(begin),
    m_end(end),
    m_cost(cost)
{}

GraphMatrix::Utils::Prim::Edge::Edge(const Edge& e):
    m_begin(e.m_begin),
    m_end(e.m_end),
    m_cost(e.m_cost)
{}

GraphMatrix::Utils::Prim::Edge::Edge(Edge&& e)
{
    m_begin = e.m_begin;
    m_end = e.m_end;
    m_cost = e.m_cost;
    e.m_begin = 0;
    e.m_end = 0;
    e.m_cost = 0;
}

GraphMatrix::Utils::Prim::VertexCost::VertexCost():
    m_set(false),
    m_explored(false),
    m_begin(0),
    m_cost(0)
{}

GraphMatrix::Utils::Prim::VertexCost::VertexCost(const uint32_t begin, const int32_t cost):
    m_set(true),
    m_explored(false),
    m_begin(begin),
    m_cost(cost)
{}

GraphMatrix::Utils::Prim::VertexCost::VertexCost(VertexCost&& v)
{
    m_set = v.m_set;
    m_explored = v.m_explored;
    m_begin = v.m_begin;
    m_cost = v.m_cost;
    v.m_set = false;
    v.m_explored = false;
    v.m_begin = 0;
    v.m_cost = 0;
}

GraphMatrix::GraphMatrix():
    m_numVertices(0),
    m_numEdges(0),
    m_directed(false)
{}

GraphMatrix::~GraphMatrix()
{
    clear();
}

void GraphMatrix::clear()
{
    m_matrix.reset();
    m_numEdges = 0;
    m_numVertices = 0;
    m_directed = false;
}

bool GraphMatrix::read(std::istream& in)
{
    clear();

    in >> m_numVertices >> m_numEdges;
    m_matrix.resize(m_numVertices, m_numVertices);

    for (uint32_t i = 0; i < m_numEdges; ++i)
    {
        // format
        // directed? begin end cost
        bool directed = false;
        uint32_t begin = 0, end = 0;
        int32_t cost = 0;
        in >> directed >> begin >> end >> cost;
        --begin;
        --end;

        m_matrix[begin][end].m_set = true;
        m_matrix[begin][end].m_cost = cost;
        if (!directed)
        {
            m_matrix[end][begin].m_set = true;
            m_matrix[end][begin].m_cost = cost;
        }
        else
        {
            // set graph as directed
            m_directed = true;
        }
    }
    return true;
}

bool GraphMatrix::write(std::ostream& out)
{
    out << "Num of edges: " << m_numEdges << std::endl;
    out << "Num of vertices: " << m_numVertices << std::endl;
    for (uint32_t i = 0; i < m_numVertices; ++i)
    {
        for (uint32_t j = 0; j < m_numVertices; ++j)
        {
            if (m_matrix[i][j].m_set)
            {
                out << m_matrix[i][j].m_cost << ' ';
            }
            else
            {
                out << "[null]" << ' ';
            }
        }
        out << std::endl;
    }
    out << "Scores: "  << std::endl;
    for (auto& score : m_scores)
    {
        out << (score.m_explored ? "[explored]" : "[not explored]") <<
            (score.m_set ? "[set]:" : "[not set]:") <<
            score.m_value << std::endl;
    }

    return true;
}

bool GraphMatrix::Dijkstra(uint32_t startingVertex, uint32_t endVertex, int32_t& length)
{
    if (startingVertex < 1 || endVertex < 1 ||
        startingVertex > m_numVertices || endVertex > m_numVertices)
    {
        return false;
    }
    --startingVertex;
    --endVertex;

    length = 0;
    if (startingVertex == endVertex)
    {
        return true;
    }

    m_scores = std::move(std::vector<Score>(m_numVertices));
    Utils::Dijkstra::EndsMultiset endsMultiset(Utils::Dijkstra::endsScoresComp);

    uint32_t currVertex = startingVertex;
    m_scores[currVertex].m_explored = true;
    m_scores[currVertex].m_set = true;
    
    for (uint32_t i = 0; i < m_numVertices; ++i)
    {
        Edge& e = m_matrix[currVertex][i];
        if (e.m_set)
        {
            m_scores[i].m_set = true;
            m_scores[i].m_value = e.m_cost;
            endsMultiset.insert(Utils::Dijkstra::End(i, e.m_cost));
        }
    }

    currVertex = endsMultiset.begin()->m_end;
    endsMultiset.erase(endsMultiset.begin());
    m_scores[currVertex].m_explored = true;

    while (currVertex != endVertex)
    {
        for (auto crossingEdgeIt = endsMultiset.begin(); crossingEdgeIt != endsMultiset.end();)
        {
            if (m_scores[crossingEdgeIt->m_end].m_explored)
            {
                endsMultiset.erase(crossingEdgeIt++);
            }
            else
            {
                ++crossingEdgeIt;
            }
        }

        for (uint32_t i = 0; i < m_numVertices; ++i)
        {
            Edge& e = m_matrix[currVertex][i];
            if (e.m_set &&
                !m_scores[i].m_explored)
            {
                if (!m_scores[i].m_set)
                {
                    m_scores[i].m_set = true;
                    m_scores[i].m_value = e.m_cost;
                    endsMultiset.insert(Utils::Dijkstra::End(i, m_scores[i].m_value));
                }
                else if (m_scores[i].m_value > m_scores[currVertex].m_value + e.m_cost)
                {
                    endsMultiset.erase(Utils::Dijkstra::End(i, m_scores[i].m_value));
                    m_scores[i].m_value = m_scores[currVertex].m_value + e.m_cost;
                    endsMultiset.insert(Utils::Dijkstra::End(i, m_scores[i].m_value));
                }
            }
        }

        if (endsMultiset.empty())
        {
            break;
        }
        currVertex = endsMultiset.begin()->m_end;
        endsMultiset.erase(endsMultiset.begin());
        m_scores[currVertex].m_explored = true;
    }

    if (currVertex != endVertex)
    {
        return false;
    }

    return true;
}

bool GraphMatrix::MSTPrimStraightforward(GraphMatrix &mst)
{
    if (isDirected())
    {
        return false;
    }

    // clear mst
    mst.clear();
    // set mst initial values
    mst.m_numVertices = m_numVertices;
    mst.m_matrix.resize(m_matrix.getRowsCount(), m_matrix.getColsCount());

    std::list<uint32_t> vertices;
    vertices.push_back(0);
    m_scores = std::move(std::vector<Score>(m_numVertices));
    m_scores[0].m_explored = true;

    while (vertices.size() != m_numVertices)
    {
        struct
        {
            bool m_set;
            int32_t m_val;
            uint32_t m_begin;
            uint32_t m_end;
        } min{false, 0, 0, 0};
        for (auto v : vertices)
        {
            for (uint32_t i = 0; i < m_numVertices; ++i)
            {
                if (m_matrix[v][i].m_set && // real edge
                    (!m_scores[i].m_explored) && // end explored?
                    (!min.m_set || // min set?
                     min.m_val > m_matrix[v][i].m_cost)) // min condition
                {
                    // set new minimum
                    min.m_set = true;
                    min.m_val = m_matrix[v][i].m_cost;
                    min.m_begin = v;
                    min.m_end = i;
                }
            }
        }
        vertices.push_back(min.m_end);
        m_scores[min.m_end].m_explored = true;
        // add edge
        ++ mst.m_numEdges;
        mst.m_matrix[min.m_begin][min.m_end].m_set = mst.m_matrix[min.m_end][min.m_begin].m_set = true;
        mst.m_matrix[min.m_begin][min.m_end].m_cost = mst.m_matrix[min.m_end][min.m_begin].m_cost = min.m_val;
    } 

    return true;
}

bool GraphMatrix::MSTPrim(GraphMatrix &mst)
{
    if (isDirected())
    {
        return false;
    }

    // set new variables for MST graph
    mst.clear();

    mst.m_numVertices = m_numVertices;
    mst.m_matrix.resize(mst.m_numVertices, mst.m_numVertices);

    std::vector<Utils::Prim::VertexCost> verticesCosts(m_numVertices);
    Utils::Prim::EdgesMultiset edgesMultiset(Utils::Prim::edgesCostsComp);

    uint32_t numExploredVertices = 1;
    uint32_t currVertex = 0;
    verticesCosts[currVertex].m_explored = true;
    verticesCosts[currVertex].m_set = true;
    verticesCosts[currVertex].m_cost = 0;

    while (numExploredVertices != m_numVertices)
    {
        for (auto crossingEdgeIt = edgesMultiset.begin(); crossingEdgeIt != edgesMultiset.end();)
        {
            if (verticesCosts[crossingEdgeIt->m_end].m_explored)
            {
                edgesMultiset.erase(crossingEdgeIt++);
            }
            else
            {
                ++crossingEdgeIt;
            }
        }

        for (uint32_t i = 0; i < m_numVertices; ++i)
        {
            Edge& e = m_matrix[currVertex][i];
            if (e.m_set &&
                !verticesCosts[i].m_explored)
            {
                if (!verticesCosts[i].m_set)
                {
                    verticesCosts[i].m_set = true;
                    verticesCosts[i].m_cost = e.m_cost;
                    verticesCosts[i].m_begin = currVertex;
                    edgesMultiset.insert(Utils::Prim::Edge(currVertex, i, e.m_cost));
                }
                else if (verticesCosts[i].m_cost > e.m_cost)
                {
                    edgesMultiset.erase(Utils::Prim::Edge(verticesCosts[i].m_begin, i, verticesCosts[i].m_cost));
                    verticesCosts[i].m_begin = currVertex;
                    verticesCosts[i].m_cost = e.m_cost;
                    edgesMultiset.insert(Utils::Prim::Edge(verticesCosts[i].m_begin, i, verticesCosts[i].m_cost));
                }
            }
        }

        if (edgesMultiset.empty())
        {
            break;
        }
        currVertex = edgesMultiset.begin()->m_end;
        edgesMultiset.erase(edgesMultiset.begin());
        verticesCosts[currVertex].m_explored = true;
        mst.m_matrix[currVertex][verticesCosts[currVertex].m_begin].m_set =
            mst.m_matrix[verticesCosts[currVertex].m_begin][currVertex].m_set =
                true;
        mst.m_matrix[currVertex][verticesCosts[currVertex].m_begin].m_cost =
            mst.m_matrix[verticesCosts[currVertex].m_begin][currVertex].m_cost =
                verticesCosts[currVertex].m_cost;
        ++mst.m_numEdges;
        ++numExploredVertices;
    }

    if (numExploredVertices != m_numVertices)
    {
        return false;
    }

    return true;
}