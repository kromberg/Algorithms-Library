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

void BellmandFordFunc()
{
    // insert code here...
    std::ifstream fin("graph.in");

    GraphMatrix graphMatrix;
    if (!graphMatrix.read(fin))
    {
        std::cout << "Cannot read graph from file" << std::endl;
        exit(2);
    }

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
    // insert code here...
    std::ifstream fin("graph.in");

    GraphMatrix graphMatrix;
    if (!graphMatrix.read(fin))
    {
        std::cout << "Cannot read graph from file" << std::endl;
        exit(2);
    }

    graphMatrix.write(std::cout);

    Matrix<GraphMatrix::LengthResult> length;
    Matrix<int64_t> path;
    graphMatrix.FloydWarshall(length, path);
    std::cout << length << std::endl;
    std::cout << path << std::endl;

    std::cin.get();
}

int main(int argc, const char * argv[])
{
    FloydWarshallFunc();
#if 0
    // insert code here...
    std::cout << "Creating graph.\n";
    std::ifstream fin("graph.in");

    GraphMatrix graphMatrix;
    if (!graphMatrix.read(fin))
    {
        std::cout << "Cannot read graph from file" << std::endl;
        exit(2);
    }

    GraphMatrix mst;
    graphMatrix.MSTPrim(mst);
    std::ofstream foutMst1("graph.mst1.out");
    mst.write(foutMst1);
#endif
#if 0
    double length = 0;
    graphMatrix.Dijkstra(1, 5, length);
    std::ofstream foutDijkstra1("graph.dijkstra1.out");
    graphMatrix.write(foutDijkstra1);
#endif
#if 0
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
    std::ofstream foutBfs1("graph.bfs1.out");
    std::ofstream foutDfs1("graph.dfs1.out");

    graph.BFS(vertex);
    graph.write(foutBfs1);
    graph.DFS(vertex);
    graph.write(foutDfs1);
    
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
    std::ofstream foutBfs2("graph.bfs2.out");
    std::ofstream foutDfs2("graph.dfs2.out");
    graph.BFS(vertex);
    graph.write(foutBfs2);
    graph.DFS(vertex);
    graph.write(foutDfs2);
#endif
    
    return 0;
}
