//
//  GraphMatrix.h
//  graphs
//
//  Created by Egor Kromberg on 14.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#ifndef GRAPH_MATRIX_H
#define GRAPH_MATRIX_H

#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <cstdint>

#include "General/Matrix.h"

class GraphMatrix
{
public:
    struct LengthResult
    {
        double m_length;
        bool m_infinity;
        LengthResult();
    };
    friend std::ostream& operator << (std::ostream& out, const LengthResult& lengthResult);

    struct Path
    {
        double m_length;
        std::list<uint32_t> m_vertices;

        Path();
        Path(const Path& path);
        Path(Path&& path);
        Path& operator=(Path&& path);
    };
    friend std::ostream& operator << (std::ostream& out, const Path& path);

    // forward declaration
    class Utils;

    struct Edge
    {
        bool m_set;
        double m_cost;
        Edge();
    };

private:
    uint32_t m_numVertices;
    uint32_t m_numEdges;
    // matrix of edges
    // rows    - out vertex
    // columns - in vertex

    Matrix<Edge> m_matrix;
    bool m_directed;

public:
    GraphMatrix();
    ~GraphMatrix();

    const bool isDirected() const;

    void clear();

    bool read(std::istream& in);
    bool write(std::ostream& out);

    bool Dijkstra(uint32_t startingVertex, uint32_t endVertex, double& length);
    // minimmum spanning tree
    bool MSTPrimStraightforward(GraphMatrix &mst);
    bool MSTPrim(GraphMatrix &mst);

    // single source shortest path
    bool BellmandFord(uint32_t sourceVertex, std::vector<Path>& paths);
    bool Dijkstra(uint32_t startingVertex, std::vector<Path>& paths);

    // all pairs shortest path
    bool FloydWarshall(Matrix<LengthResult>& length, Matrix<int64_t>& pathsMatrix);
    bool Johnson(Matrix<Path>& pathMatrix);
};

inline const bool GraphMatrix::isDirected() const
{
    return m_directed;
}

class GraphMatrix::Utils
{
public:
    struct Edge
    {
        uint32_t m_begin;
        uint32_t m_end;
        double m_cost;
        Edge(const uint32_t begin, const uint32_t end, const double cost);
        Edge(const Edge& e);
        Edge(Edge&& e);
        Edge& operator=(Edge&& e);
    };
    static bool edgesCostsComp(const Edge& e1, const Edge& e2) { return e1.m_cost < e2.m_cost; }
    typedef std::multiset<Edge, bool(*)(const Edge&, const Edge&)> EdgesMultiset;

    struct VertexCost
    {
        bool m_set;
        bool m_explored;
        uint32_t m_begin;
        GraphMatrix::Path m_path;
        VertexCost();
        VertexCost(const uint32_t begin, const double cost);
        VertexCost(VertexCost&& v);
        VertexCost& operator=(VertexCost&& v);
    };

    class BellmanFord
    {
    public:
        struct MinEntity
        {
            double m_length;
            bool m_infinity;
            GraphMatrix::Path* m_path;
            uint32_t m_vertex;
            MinEntity();
        };
        struct MatrixEntity
        {
            GraphMatrix::Path m_path;
            bool m_infinity;
            MatrixEntity();
        };
        friend std::ostream& operator << (std::ostream& out, const MatrixEntity& matrixEntity);
    };

    class FloydWarshall
    {
    public:
        struct MatrixEntity
        {
            double m_length;
            bool m_infinity;
            MatrixEntity();
        };
        friend std::ostream& operator << (std::ostream& out, const MatrixEntity& matrixEntity);
    };
};

#endif // GRAPH_MATRIX_H