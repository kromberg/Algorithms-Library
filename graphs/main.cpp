//
//  main.cpp
//  graphs
//
//  Created by Egor Kromberg on 13.12.15.
//  Copyright © 2015 Egor Kromberg. All rights reserved.
//

#include <cstdint>
#include <iostream>

#include "Graph.h"
#include "GraphMatrix.h"

int main(int argc, const char * argv[])
{
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

#if 0
    int32_t length = 0;
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
