//
//  main.cpp
//  graphs
//
//  Created by Egor Kromberg on 13.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#include <cstdint>
#include <iostream>
#include <cstdint>

#include "graphs/Graph.h"
#include "graphs/GraphMatrix.h"
#include "General/Knapsack.h"
#include "General/SequenceAlignment.h"
#include "General/OptimalBST.h"

void knapsackFunc()
{
    Knapsack knapsack;
    std::ifstream fin("knapsack.in");
    if (!knapsack.read(fin))
    {
        std::cout << "Cannot read Knapsack" << std::endl;
        exit(2);
    }
    uint32_t val = 0;
    knapsack.run(165, val);
    std::cout << val << std::endl;

    std::cin.get();
}

void sequenceAlignmentFunc()
{
    SequenceAlignment sequenceAlignment;
    std::ifstream fin("sequence_alignment.in");
    if (!sequenceAlignment.read(fin))
    {
        std::cout << "Cannot read SequenceAlignment" << std::endl;
        exit(2);
    }
    uint32_t nwScore = 0;
    std::string sequnces[2];
    sequenceAlignment.run(nwScore, sequnces);
    std::cout << nwScore << std::endl;
    std::cout << sequnces[0] << std::endl;
    std::cout << sequnces[1] << std::endl;

    std::cin.get();
}

void optimalBSTFunc()
{
    OptimalBST optimalBST;
    std::ifstream fin("optimal_bst.in");
    if (!optimalBST.read(fin))
    {
        std::cout << "Cannot read OptimalBST" << std::endl;
        exit(2);
    }
    double val = 0;
    optimalBST.run(val);
    std::cout << "Value : " << val << std::endl;
    optimalBST.write(std::cout);

    std::cin.get();
}

void readGraphMatrix(GraphMatrix &graphMatrix)
{
    std::ifstream fin("graph.in");

    if (!graphMatrix.read(fin))
    {
        std::cout << "Cannot read graph from file" << std::endl;
        exit(2);
    }
}

void BfsDfs()
{
    std::ifstream fin("graph.in");
    Graph graph;
    graph.read(fin);
    const VerticesList& verticesList = graph.getVertices();
    Vertex *vertex = nullptr;
    for (auto& verIt : verticesList)
    {
        if (verIt->m_id == 1)
        {
            vertex = verIt;
            break;
        }
    }
    if (!vertex)
    {
        std::cout << "Cannot find vertex" << std::endl;
        exit(2);
    }

    std::cout << "BFS from 1" << std::endl;
    graph.BFS(vertex);
    graph.write(std::cout);
    std::cout << "DFS from 1" << std::endl;
    graph.DFS(vertex);
    graph.write(std::cout);

    vertex = nullptr;
    for (auto& verIt : verticesList)
    {
        if (verIt->m_id == 6)
        {
            vertex = verIt;
            break;
        }
    }
    if (!vertex)
    {
        std::cout << "Cannot find vertex" << std::endl;
        exit(2);
    }
    std::cout << "BFS from 6" << std::endl;
    graph.BFS(vertex);
    graph.write(std::cout);
    std::cout << "DFS from 6" << std::endl;
    graph.DFS(vertex);
    graph.write(std::cout);

    std::cin.get();
}

void VertexCoverFunc()
{
    std::ifstream fin("graph.in");
    Graph graph;
    graph.read(fin);
    uint32_t k = 3;
    std::cout << "Vertex cover with k = " << k << (graph.doesVertexCoverExists(3) ? " exists" : " doesn't exist");

    std::cin.get();
}

void MSTPrimFunc()
{
    GraphMatrix graphMatrix;
    readGraphMatrix(graphMatrix);

    GraphMatrix mst;
    graphMatrix.MSTPrim(mst);
    mst.write(std::cout);
    std::cin.get();
}

void DijkstraFunc()
{
    GraphMatrix graphMatrix;
    readGraphMatrix(graphMatrix);

    double length = 0;
    graphMatrix.Dijkstra(1, 6, length);
    std::cout << "Length : " << length << std::endl;
    graphMatrix.write(std::cout);
    std::cin.get();
}


void BellmandFordFunc()
{
    GraphMatrix graphMatrix;
    readGraphMatrix(graphMatrix);

    graphMatrix.write(std::cout);

    std::vector<GraphMatrix::Path> paths;
    graphMatrix.BellmandFord(2, paths);
    for (auto& path : paths)
    {
        std::cout << path << std::endl;
    }

    std::cin.get();
}

void FloydWarshallFunc()
{
    GraphMatrix graphMatrix;
    readGraphMatrix(graphMatrix);

    graphMatrix.write(std::cout);

    Matrix<GraphMatrix::LengthResult> length;
    Matrix<int64_t> path;
    graphMatrix.FloydWarshall(length, path);
    std::cout << length << std::endl;
    std::cout << path << std::endl;

    std::cin.get();
}

void JohnsonFunc()
{
    GraphMatrix graphMatrix;
    readGraphMatrix(graphMatrix);
    graphMatrix.write(std::cout);

    Matrix<GraphMatrix::Path> pathsMatrix;
    graphMatrix.Johnson(pathsMatrix);
    std::cout << pathsMatrix << std::endl;

    std::cin.get();
}

int main(int argc, const char * argv[])
{
    VertexCoverFunc();
    
    return 0;
}
