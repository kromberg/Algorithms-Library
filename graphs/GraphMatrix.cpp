//
//  GraphMatrix.cpp
//  graphs
//
//  Created by Egor Kromberg on 14.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#include <stdio.h>
#include <list>
#include <iostream>
#include <map>

#include "General/BitsVector.h"
#include "GraphMatrix.h"


GraphMatrix::Utils::Edge::Edge(const uint32_t begin, const uint32_t end, const double cost) :
    m_begin(begin),
    m_end(end),
    m_cost(cost)
{}

GraphMatrix::Utils::Edge::Edge(const Edge& e) :
    m_begin(e.m_begin),
    m_end(e.m_end),
    m_cost(e.m_cost)
{}

GraphMatrix::Utils::Edge::Edge(Edge&& e)
{
    m_begin = e.m_begin;
    m_end = e.m_end;
    m_cost = e.m_cost;
    e.m_begin = 0;
    e.m_end = 0;
    e.m_cost = 0;
}

GraphMatrix::Utils::Edge& GraphMatrix::Utils::Edge::operator=(Edge&& e)
{
    if (this == &e)
    {
        return *this;
    }
    m_begin = e.m_begin;
    m_end = e.m_end;
    m_cost = e.m_cost;
    e.m_begin = 0;
    e.m_end = 0;
    e.m_cost = 0;
    return *this;
}

GraphMatrix::Utils::VertexCost::VertexCost():
    m_set(false),
    m_explored(false),
    m_begin(0),
    m_path()
{}

GraphMatrix::Utils::VertexCost::VertexCost(const uint32_t begin, const double cost):
    m_set(true),
    m_explored(false),
    m_begin(begin),
    m_path()
{
    m_path.m_length = cost;
}

GraphMatrix::Utils::VertexCost::VertexCost(VertexCost&& v)
{
    m_set = v.m_set;
    m_explored = v.m_explored;
    m_begin = v.m_begin;
    m_path = std::move(v.m_path);
    v.m_set = false;
    v.m_explored = false;
    v.m_begin = 0;
}

GraphMatrix::Utils::VertexCost& GraphMatrix::Utils::VertexCost::operator=(VertexCost&& v)
{
    if (this == &v)
    {
        return *this;
    }
    m_set = v.m_set;
    m_explored = v.m_explored;
    m_begin = v.m_begin;
    m_path = std::move(v.m_path);
    v.m_set = false;
    v.m_explored = false;
    v.m_begin = 0;
    return *this;
}

GraphMatrix::Utils::BellmanFord::MinEntity::MinEntity() :
    m_length(0),
    m_infinity(true),
    m_path(nullptr),
    m_vertex(0)
{}

GraphMatrix::Utils::BellmanFord::MatrixEntity::MatrixEntity() :
    m_infinity(true),
    m_path()
{}

std::ostream& operator << (std::ostream& out, const GraphMatrix::Utils::BellmanFord::MatrixEntity& matrixEntity)
{
    out << "[" << (matrixEntity.m_infinity ? "Infinity" : "Not infinity") << " : " << matrixEntity.m_path.m_length << "]";
    return out;
}

GraphMatrix::Utils::FloydWarshall::MatrixEntity::MatrixEntity() :
    m_infinity(true),
    m_length(0)
{}

std::ostream& operator << (std::ostream& out, const GraphMatrix::Utils::FloydWarshall::MatrixEntity& matrixEntity)
{
    out << "[" << (matrixEntity.m_infinity ? "Infinity" : "Not infinity") << " : " << matrixEntity.m_length << "]";
    return out;
}

GraphMatrix::LengthResult::LengthResult():
    m_infinity(true),
    m_length(0)
{}

std::ostream& operator << (std::ostream& out, const GraphMatrix::LengthResult& lengthResult)
{
    out << "[" << (lengthResult.m_infinity ? "Infinity" : "Not infinity") << " : " << lengthResult.m_length << "]";
    return out;
}

GraphMatrix::Path::Path() :
    m_length(0),
    m_vertices()
{}

GraphMatrix::Path::Path(const Path& path)
{
    m_length = path.m_length;
    m_vertices = path.m_vertices;
}

GraphMatrix::Path::Path(Path&& path)
{
    m_length = path.m_length;
    m_vertices = std::move(path.m_vertices);
    path.m_length = 0;
}

GraphMatrix::Path& GraphMatrix::Path::operator=(Path&& path)
{
    if (this == &path)
    {
        return *this;
    }
    m_length = path.m_length;
    m_vertices = std::move(path.m_vertices);
    path.m_length = 0;
    return *this;
}

std::ostream& operator << (std::ostream& out, const GraphMatrix::Path& path)
{
    out << "[Length: " << path.m_length << "; Path: ";
    bool first = true;
    uint32_t prev = 0;
    for (auto v : path.m_vertices)
    {
        if (!first)
        {
            if (v != prev)
            {
                out << " -> ";
                out << v + 1;
            }
        }
        else
        {
            out << v + 1;
            first = false;
        }
        prev = v;
    }
    out << "]";
    return out;
}

GraphMatrix::Edge::Edge() :
    m_set(false),
    m_cost(0)
{}

GraphMatrix::GraphMatrix():
    m_numVertices(0),
    m_numEdges(0),
    m_directed(false)
{}

GraphMatrix::~GraphMatrix()
{
    clear();
}

const bool GraphMatrix::isComplete() const
{
    for (uint32_t i = 0; i < m_numVertices; ++i)
    {
        for (uint32_t j = 0; j < m_numVertices; ++j)
        {
            if (!m_matrix[i][j].m_set)
            {
                return false;
            }
        }
    }
    return true;
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
        double cost = 0;
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

    return true;
}

bool GraphMatrix::Dijkstra(uint32_t startingVertex, uint32_t endVertex, double& length)
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

    std::vector<Utils::VertexCost> scores(m_numVertices);
    Utils::EdgesMultiset edgesMultiset(Utils::edgesCostsComp);

    uint32_t currVertex = startingVertex;
    scores[currVertex].m_explored = true;
    scores[currVertex].m_set = true;
    scores[currVertex].m_path.m_vertices.push_back(currVertex);
    scores[currVertex].m_path.m_length = 0;

    while (currVertex != endVertex)
    {
        for (auto crossingEdgeIt = edgesMultiset.begin(); crossingEdgeIt != edgesMultiset.end();)
        {
            if (scores[crossingEdgeIt->m_end].m_explored)
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
                !scores[i].m_explored)
            {
                if (!scores[i].m_set)
                {
                    scores[i].m_set = true;
                    scores[i].m_path.m_length = scores[currVertex].m_path.m_length + e.m_cost;
                    scores[i].m_begin = currVertex;
                    edgesMultiset.insert(Utils::Edge(scores[i].m_begin, i, scores[i].m_path.m_length));
                }
                else if (scores[i].m_path.m_length > scores[currVertex].m_path.m_length + e.m_cost)
                {
                    edgesMultiset.erase(Utils::Edge(scores[i].m_begin, i, scores[i].m_path.m_length));
                    scores[i].m_begin = currVertex;
                    scores[i].m_path.m_length = scores[currVertex].m_path.m_length + e.m_cost;
                    edgesMultiset.insert(Utils::Edge(scores[i].m_begin, i, scores[i].m_path.m_length));
                }
            }
        }

        if (edgesMultiset.empty())
        {
            break;
        }
        const Utils::Edge& e = *(edgesMultiset.begin());
        currVertex = e.m_end;
        scores[currVertex].m_explored = true;
        scores[currVertex].m_path.m_vertices = scores[e.m_begin].m_path.m_vertices;
        scores[currVertex].m_path.m_vertices.push_back(currVertex);
        edgesMultiset.erase(edgesMultiset.begin());
    }

    if (currVertex != endVertex)
    {
        return false;
    }

    //debug
    std::cout << "Scores: " << std::endl;
    for (auto& score : scores)
    {
        std::cout << (score.m_explored ? "[explored; " : "[not explored; ") <<
            (score.m_set ? " set: " : " not set: ") <<
            score.m_path << ']' << std::endl;
    }

    length = scores[endVertex].m_path.m_length;

    return true;
}

// SSSP Dijkstra
bool GraphMatrix::Dijkstra(uint32_t startingVertex, std::vector<Path>& paths)
{
    if (startingVertex < 1 || startingVertex > m_numVertices)
    {
        return false;
    }
    --startingVertex;

    std::vector<Utils::VertexCost> scores(m_numVertices);
    Utils::EdgesMultiset edgesMultiset(Utils::edgesCostsComp);

    uint32_t currVertex = startingVertex;
    scores[currVertex].m_explored = true;
    scores[currVertex].m_set = true;
    scores[currVertex].m_path.m_vertices.push_back(currVertex);
    scores[currVertex].m_path.m_length = 0;

    for (;;)
    {
        for (auto crossingEdgeIt = edgesMultiset.begin(); crossingEdgeIt != edgesMultiset.end();)
        {
            if (scores[crossingEdgeIt->m_end].m_explored)
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
                !scores[i].m_explored)
            {
                if (!scores[i].m_set)
                {
                    scores[i].m_set = true;
                    scores[i].m_path.m_length = scores[currVertex].m_path.m_length + e.m_cost;
                    scores[i].m_begin = currVertex;
                    edgesMultiset.insert(Utils::Edge(scores[i].m_begin, i, scores[i].m_path.m_length));
                }
                else if (scores[i].m_path.m_length > scores[currVertex].m_path.m_length + e.m_cost)
                {
                    edgesMultiset.erase(Utils::Edge(scores[i].m_begin, i, scores[i].m_path.m_length));
                    scores[i].m_begin = currVertex;
                    scores[i].m_path.m_length = scores[currVertex].m_path.m_length + e.m_cost;
                    edgesMultiset.insert(Utils::Edge(scores[i].m_begin, i, scores[i].m_path.m_length));
                }
            }
        }

        if (edgesMultiset.empty())
        {
            break;
        }
        const Utils::Edge& e = *(edgesMultiset.begin());
        currVertex = e.m_end;
        scores[currVertex].m_explored = true;
        scores[currVertex].m_path.m_vertices = scores[e.m_begin].m_path.m_vertices;
        scores[currVertex].m_path.m_vertices.push_back(currVertex);
        edgesMultiset.erase(edgesMultiset.begin());
    }

    //debug
    std::cout << "Scores: "  << std::endl;
    for (auto& score : scores)
    {
        std::cout << (score.m_explored ? "[explored; " : "[not explored; ") <<
            (score.m_set ? " set: " : " not set: ") <<
            score.m_path << ']' << std::endl;
    }

    for (auto& score : scores)
    {
        paths.push_back(std::move(score.m_path));
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
    struct Score
    {
        bool m_explored;
        Score() : m_explored(false)
        {}
    };
    std::vector<Score> scores(m_numVertices);
    scores[0].m_explored = true;

    while (vertices.size() != m_numVertices)
    {
        struct
        {
            bool m_set;
            double m_val;
            uint32_t m_begin;
            uint32_t m_end;
        } min{false, 0, 0, 0};
        for (auto v : vertices)
        {
            for (uint32_t i = 0; i < m_numVertices; ++i)
            {
                if (m_matrix[v][i].m_set && // real edge
                    (!scores[i].m_explored) && // end explored?
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
        scores[min.m_end].m_explored = true;
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

    std::vector<Utils::VertexCost> verticesCosts(m_numVertices);
    Utils::EdgesMultiset edgesMultiset(Utils::edgesCostsComp);

    uint32_t numExploredVertices = 1;
    uint32_t currVertex = 0;
    verticesCosts[currVertex].m_explored = true;
    verticesCosts[currVertex].m_set = true;
    verticesCosts[currVertex].m_path.m_length = 0;

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
                    verticesCosts[i].m_path.m_length = e.m_cost;
                    verticesCosts[i].m_begin = currVertex;
                    edgesMultiset.insert(Utils::Edge(currVertex, i, e.m_cost));
                }
                else if (verticesCosts[i].m_path.m_length > e.m_cost)
                {
                    edgesMultiset.erase(Utils::Edge(verticesCosts[i].m_begin, i, verticesCosts[i].m_path.m_length));
                    verticesCosts[i].m_begin = currVertex;
                    verticesCosts[i].m_path.m_length = e.m_cost;
                    edgesMultiset.insert(Utils::Edge(verticesCosts[i].m_begin, i, verticesCosts[i].m_path.m_length));
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
                verticesCosts[currVertex].m_path.m_length;
        ++mst.m_numEdges;
        ++numExploredVertices;
    }

    if (numExploredVertices != m_numVertices)
    {
        return false;
    }

    return true;
}

bool GraphMatrix::BellmandFord(uint32_t sourceVertex, std::vector<Path>& paths)
{
    if ((sourceVertex < 1) ||
        (sourceVertex > m_numVertices))
    {
        return false;
    }
    --sourceVertex;
    // TODO: optimiaze memory. we just need [2 x m_numVerticies] matrix
    Matrix<Utils::BellmanFord::MatrixEntity> matrix(m_numEdges + 1, m_numVertices);
    matrix[0][sourceVertex].m_infinity = false;
    matrix[0][sourceVertex].m_path.m_length = 0;

    Utils::BellmanFord::MinEntity minEntity;
    for (uint32_t i = 1; i < m_numEdges + 1; ++i)
    {
        for (uint32_t v = 0; v < m_numVertices; ++v)
        {
            for (uint32_t w = 0; w < m_numVertices; ++w)
            {
                if (m_matrix[w][v].m_set && !matrix[i - 1][w].m_infinity)
                {
                    if ((minEntity.m_infinity) || 
                        (matrix[i - 1][w].m_path.m_length + m_matrix[w][v].m_cost < minEntity.m_length))
                    {
                        minEntity.m_infinity = false;
                        minEntity.m_length = matrix[i - 1][w].m_path.m_length + m_matrix[w][v].m_cost;
                        minEntity.m_path = &matrix[i - 1][w].m_path;
                        minEntity.m_vertex = w;
                    }
                }
            }
            if (!matrix[i - 1][v].m_infinity)
            {
                if ((minEntity.m_infinity) ||
                    (matrix[i - 1][v].m_path.m_length < minEntity.m_length))
                {
                    minEntity.m_infinity = false;
                    minEntity.m_length = matrix[i - 1][v].m_path.m_length;
                    minEntity.m_path = &matrix[i - 1][v].m_path;
                    minEntity.m_vertex = v;
                }
            }
            // copy
            matrix[i][v].m_infinity = minEntity.m_infinity;
            if (minEntity.m_path)
            {
                matrix[i][v].m_path = *minEntity.m_path;
                matrix[i][v].m_path.m_vertices.push_back(minEntity.m_vertex);
                minEntity.m_path = nullptr;
                minEntity.m_vertex = 0;
            }
            matrix[i][v].m_path.m_length = minEntity.m_length;
            minEntity.m_infinity = true;
            minEntity.m_length = 0;
        }
    }
    
    for (uint32_t v = 0; v < m_numVertices; ++v)
    {
        if (matrix[m_numEdges][v].m_path.m_length < matrix[m_numEdges - 1][v].m_path.m_length)
        {
            // negative cycle was detected
            return false;
        }
    }

    for (uint32_t v = 0; v < m_numVertices; ++v)
    {
        matrix[m_numEdges - 1][v].m_path.m_vertices.push_back(v);
        paths.push_back(std::move(matrix[m_numEdges - 1][v].m_path));
    }

    return true;
}

bool GraphMatrix::FloydWarshall(Matrix<LengthResult>& length, Matrix<int64_t>& pathsMatrix)
{
    // TODO: size [2 x m_numVetices x m_numVetices] is enough
    std::vector<Matrix<Utils::FloydWarshall::MatrixEntity> > matrixLayers;
    matrixLayers.resize(m_numVertices);

    pathsMatrix.resize(m_numVertices, m_numVertices);
    pathsMatrix.fill(-1);

    for (auto& matrix : matrixLayers)
    {
        matrix.resize(m_numVertices, m_numVertices);
    }

    for (uint32_t i = 0; i < m_numVertices; ++i)
    {
        for (uint32_t j = 0; j < m_numVertices; ++j)
        {
            if (i == j)
            {
                matrixLayers[0][i][j].m_infinity = false;
                matrixLayers[0][i][j].m_length = 0;
            }
            else if (m_matrix[i][j].m_set)
            {
                matrixLayers[0][i][j].m_infinity = false;
                matrixLayers[0][i][j].m_length = m_matrix[i][j].m_cost;
                pathsMatrix[i][j] = j;
            }
        }
    }
    /* debug
    std::cout << "##############################################################" << std::endl;
    std::cout << matrixLayers[0] << std::endl;
    */

    Utils::FloydWarshall::MatrixEntity minEntity;
    for (uint32_t k = 1; k < m_numVertices; ++k)
    {
        for (uint32_t i = 0; i < m_numVertices; ++i)
        {
            for (uint32_t j = 0; j < m_numVertices; ++j)
            {
                minEntity.m_infinity = matrixLayers[k - 1][i][j].m_infinity;
                minEntity.m_length = matrixLayers[k - 1][i][j].m_length;

                if (!matrixLayers[k - 1][i][k].m_infinity &&
                    !matrixLayers[k - 1][k][j].m_infinity)
                {
                    if (minEntity.m_infinity ||
                        (minEntity.m_length > matrixLayers[k - 1][i][k].m_length + !matrixLayers[k - 1][k][j].m_length))
                    {
                        minEntity.m_infinity = false;
                        minEntity.m_length = matrixLayers[k - 1][i][k].m_length + matrixLayers[k - 1][k][j].m_length;
                        pathsMatrix[i][j] = k;
                    }
                }
                matrixLayers[k][i][j].m_infinity = minEntity.m_infinity;
                matrixLayers[k][i][j].m_length = minEntity.m_length;

                minEntity.m_infinity = true;
                minEntity.m_length = 0;
            }
        }

        /* debug
        std::cout << "##############################################################" << std::endl;
        std::cout << matrixLayers[k] << std::endl;
        */
    }

    length.resize(m_numVertices, m_numVertices);
    for (uint32_t i = 0; i < m_numVertices; ++i)
    {
        for (uint32_t j = 0; j < m_numVertices; ++j)
        {
            length[i][j].m_infinity = matrixLayers[m_numVertices - 1][i][j].m_infinity;
            length[i][j].m_length = matrixLayers[m_numVertices - 1][i][j].m_length;
        }
    }

    return true;
}

bool GraphMatrix::Johnson(Matrix<Path>& pathMatrix)
{
    GraphMatrix supportGraph = *this;
    ++ supportGraph.m_numVertices;
    supportGraph.m_matrix.resize(supportGraph.m_numVertices, supportGraph.m_numVertices);
    for (uint32_t i = 0; i < supportGraph.m_numVertices - 1; ++i)
    {
        supportGraph.m_matrix[supportGraph.m_numVertices - 1][i].m_set = true;
        supportGraph.m_matrix[supportGraph.m_numVertices - 1][i].m_cost = 0;

        supportGraph.m_matrix[i][supportGraph.m_numVertices - 1].m_set = false;
    }
    std::vector<Path> tmpPaths;
    if (!supportGraph.BellmandFord(supportGraph.m_numVertices - 1, tmpPaths))
    {
        return false;
    }

    -- supportGraph.m_numVertices;
    // re-calculate edges lengths
    for (uint32_t i = 0; i < supportGraph.m_numVertices; ++i)
    {
        for (uint32_t j = 0; j < supportGraph.m_numVertices; ++j)
        {
            if (supportGraph.m_matrix[i][j].m_set)
            {
                supportGraph.m_matrix[i][j].m_cost += (tmpPaths[i].m_length - tmpPaths[j].m_length);
            }
        }
    }
    supportGraph.m_matrix.resize(supportGraph.m_numVertices, supportGraph.m_numVertices);

    pathMatrix.resize(m_numVertices, m_numVertices);
    for (uint32_t i = 0; i < m_numVertices; ++i)
    {
        std::vector<Path> paths;
        supportGraph.Dijkstra(i + 1, paths);

        for (uint32_t j = 0; j < m_numVertices; ++j)
        {
            pathMatrix[i][j] = paths[j];
            pathMatrix[i][j].m_length -= (tmpPaths[i].m_length - tmpPaths[j].m_length);
        }
    }

    return true;
}

bool GraphMatrix::travelingSalesmanProblem(Path& path)
{
    if (isDirected() || !isComplete())
    {
        return false;
    }

    typedef uint64_t VerticesSubset;
    std::map<VerticesSubset, std::vector<double> > verticesSubsetToLength;

    // TODO: make it bits vector
    VerticesSubset verticesSubset = 0;
    VerticesSubset maxVerticesSubset = 0;

    auto getFirstVerticesSubset = [&verticesSubset, &maxVerticesSubset](const uint32_t m, const uint32_t numVertices) -> void {
        for (uint32_t i = 0; i < m; ++i)
        {
            //verticesSubset.set(i);
            verticesSubset |= (1 << i);
        }
        maxVerticesSubset = (verticesSubset << (numVertices - m));
        verticesSubset <<= 1;
        verticesSubset |= 1;
    };

    auto getNextVerticesSubset = [&verticesSubset, &maxVerticesSubset]() -> bool {
        verticesSubset >>= 1;
        // t gets verticesSubset's least significant 0 bits set to 1
        uint64_t t = verticesSubset | (verticesSubset - 1);
        // Next set to 1 the most significant bit to change, 
        // set to 0 the least significant ones, and add the necessary 1 bits.
        unsigned long idx = 0;
        verticesSubset = (t + 1) | (((~t & -~t) - 1) >> (_BitScanForward64(&idx, verticesSubset) + 1));

        if (verticesSubset > maxVerticesSubset)
        {
            return false;
        }
        verticesSubset <<= 1;
        verticesSubset |= 1;
        return true;
    };

    for (uint32_t m = 2; m <= m_numVertices; ++m)
    {
        getFirstVerticesSubset(m, m_numVertices);

        uint32_t numProcessed = 0;
        uint8_t pos = 2;
        do {
            if (!(verticesSubset & (1 << (pos - 1))))
            {
                pos += 1;
            }
            verticesSubsetToLength[verticesSubset ^ (1 << (pos - 1))]

        } while (numProcessed < m_numVertices);

        //verticesSubsetToLength[verticesSubset]
    }

    return true;
}