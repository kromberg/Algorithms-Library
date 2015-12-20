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
        Edge() :
            m_set(false),
            m_cost(0)
        {}
    };

private:
    uint32_t m_numVertices;
    uint32_t m_numEdges;
    // matrix of edges
    // rows    - out vertex
    // columns - in vertex

    Matrix<Edge> m_matrix;
    bool m_directed;
    struct Score
    {
        bool m_explored;
        bool m_set;
        double m_value;
        Score():
            m_explored(false),
            m_set(false),
            m_value(0)
        {}
    };
    std::vector<Score> m_scores;

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
};

inline const bool GraphMatrix::isDirected() const
{
    return m_directed;
}

class GraphMatrix::Utils
{
public:
    class Dijkstra
    {
    public:
        struct End
        {
            uint32_t m_end;
            double m_score;
            End(const uint32_t end, const double score);
            End(const End& e);
            End(End&& e);
            End& operator=(End&& e);
        };
        static bool endsScoresComp(const End& e1, const End& e2) { return e1.m_score < e2.m_score; }
        typedef std::multiset<End, bool(*)(const End&, const End&)> EndsMultiset;
    };

    class Prim
    {
    public:
        struct VertexCost
        {
            bool m_set;
            bool m_explored;
            uint32_t m_begin;
            double m_cost;
            VertexCost();
            VertexCost(const uint32_t begin, const double cost);
            VertexCost(VertexCost&& v);
            VertexCost& operator=(VertexCost&& v);
        };

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
            ~MatrixEntity();
        };
        friend std::ostream& operator << (std::ostream& out, const MatrixEntity& matrixEntity);
    };
};

#endif // GRAPH_MATRIX_H