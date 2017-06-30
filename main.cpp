#include "graph.hpp"

#include <iostream>

using namespace std;

int main()
{
    Graph graph;
    graph.loadFromFile("./dependencyGraph.txt");
    graph.toDotFile("graph.dot");

    return 0;
}
