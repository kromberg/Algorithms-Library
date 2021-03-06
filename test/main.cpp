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
#include "hackerrank/SherlockMinimax.h"
#include "hackerrank/LongestIncrSubseq.h"
#include "hackerrank/AlmostSorted.h"
#include "hackerrank/Clique.h"
#include "hackerrank/SansaAndXOR.h"
#include "hackerrank/JackGoesToRapture.h"

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
    Knapsack::ItemsList resList;
    knapsack.run(val, resList);
    std::cout << val << std::endl;
    for (auto& item : resList)
    {
        std::cout << "< Weight : " << item.m_weight << " ; Value : " << item.m_value << " >" << std::endl;
    }

    std::cout << "#######################################" << std::endl;
    std::cout << "GREEDY" << std::endl;
    resList.clear();
    knapsack.runGreedy(val, resList);
    std::cout << val << std::endl;
    for (auto& item : resList)
    {
        std::cout << "< Weight : " << item.m_weight << " ; Value : " << item.m_value << " >" << std::endl;
    }

    std::cout << "#######################################" << std::endl;
    std::cout << "BY VALUE" << std::endl;
    resList.clear();
    knapsack.runByValue(val, resList);
    std::cout << val << std::endl;
    for (auto& item : resList)
    {
        std::cout << "< Weight : " << item.m_weight << " ; Value : " << item.m_value << " >" << std::endl;
    }

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

void readGraphMatrix(GraphMatrix &graphMatrix, const std::string graphFilename = "graph.in")
{
    std::ifstream fin(graphFilename.c_str());

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
    if (!graph.read(fin))
    {
        std::cout << "Cannot read graph from file" << std::endl;
        exit(2);
    }
    std::cout << "BFS from 1" << std::endl;
    graph.BFS(1);
    graph.write(std::cout);
    std::cout << "DFS from 1" << std::endl;
    graph.DFS(1);
    graph.write(std::cout);

    std::cout << "BFS from 6" << std::endl;
    graph.BFS(6);
    graph.write(std::cout);
    std::cout << "DFS from 6" << std::endl;
    graph.DFS(6);
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

void TspFunc()
{
    GraphMatrix graphMatrix;
    readGraphMatrix(graphMatrix, "graph.tsp.in");
    graphMatrix.write(std::cout);

    GraphMatrix::Path path;
    double length = 0;
    graphMatrix.travelingSalesmanProblem(length, path);
    std::cout << "Length : " << length << std::endl;

    std::cin.get();
}

void SherlockMinimaxFunc()
{
    SherlockMinimax sherlockMinimax;
    std::ifstream fin("sherlock_minimax.in");
    fin >> sherlockMinimax;
    int32_t res = 0;
    int32_t val = 0;
    sherlockMinimax.runAlgorithm(res, val);
    std::cout << "Result : " << res << " Value : " << val << std::endl;

    std::cin.get();
}

void LongestIncrSubseqFunc()
{
    LongestIncrSubseq longestIncrSubseq;
    std::ifstream fin("longest_increasing_subsequence.long.in");
    fin >> longestIncrSubseq;
    uint32_t res = 0;
    longestIncrSubseq.runOptimizedAlgorithm(res);
    std::cout << res << std::endl;

    std::cin.get();
}

void AlmostSortedFunc()
{
    AlmostSorted almostSorted;
    std::ifstream fin("almost_sorted.in");
    fin >> almostSorted;
    AlmostSorted::Anomaly anomaly;
    bool res = almostSorted.runAlgorithm(anomaly);
    if (!res)
    {
        std::cout << "no" << std::endl;
    }
    else
    {
        std::cout << "yes" << std::endl;
        if (!anomaly.m_what.empty())
        {
            std::cout << anomaly.m_what << ' ' << anomaly.m_indices.first + 1 << ' ' << anomaly.m_indices.second + 1<< std::endl;
        }
    }

    std::cin.get();
}

void CliqueFunc()
{
    Clique clique;
    std::ifstream fin("clique.in");
    uint32_t numTestcases = 0;
    fin >> numTestcases;
    while (numTestcases--)
    {
        fin >> clique;
        uint32_t res = 0;
        clique.runAlgorithm(res);
        std::cout << res << std::endl;
    }

    std::cin.get();
}

void SansaAndXORFunc()
{
    SansaAndXOR sansaAndXOR;
    std::ifstream fin("sansa_xor.in");
    uint32_t numTestcases = 0;
    fin >> numTestcases;
    while (numTestcases--)
    {
        fin >> sansaAndXOR;
        uint32_t res = 0;
        sansaAndXOR.runAlgorithm(res);
        std::cout << res << std::endl;
    }

    std::cin.get();
}

void JackGoesToRaptureFunc()
{
    JackGoesToRapture jackGoesToRapture;
    std::ifstream fin("jack_goes_to_rapture.long2.in");
    fin >> jackGoesToRapture;
    int32_t res = 0;
    jackGoesToRapture.runAlgorithm(res);
    std::cout << res << std::endl;
    
    std::cin.get();
}

int main(int argc, const char * argv[])
{
    BfsDfs();
    
    return 0;
}
