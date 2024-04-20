#include <iostream>

#include "Graph/Graph.hpp"

using namespace std;

int main()
{
    // Create a graph with 5 nodes
    Graph<int, int> graph(5);

    Vertex v1(1);
    Vertex v2(2);
    Vertex v3(3);
    Vertex v4(4);
    Vertex v5(5);

    // Add some edges
    graph.addBidirectedEdge(0, 1, 1, 2);
    graph.addBidirectedEdge(0, 2, 2, 3);
    graph.addBidirectedEdge(1, 2, 3, 4);
    graph.addBidirectedEdge(1, 3, 4, 5);
    graph.addBidirectedEdge(2, 4, 5, 6);
    graph.addBidirectedEdge(3, 4, 6, 7);

    // Print the adjacency list
    vector<map<int, float>> adjacencyList = graph.getAdjacencyList();
    for (size_t i = 0; i < adjacencyList.size(); ++i)
    {
        cout << i << ": ";
        for (auto it = adjacencyList[i].begin(); it != adjacencyList[i].end(); ++it)
        {
            cout << it->first << "(" << it->second << ") ";
        }
        cout << endl;
    }

    // Get out edges of node 1
    vector<pair<int, float>> outEdges = graph.getOutEdges(v1);
    cout << "Out edges of node 1: ";
    for (const auto &edge : outEdges)
    {
        cout << edge.first << "(" << edge.second << ") ";
    }
    cout << endl;

    // Get in edges of node 4
    vector<pair<int, float>> inEdges = graph.getInEdges(v4);
    cout << "In edges of node 4: ";
    for (const auto &edge : inEdges)
    {
        cout << edge.first << "(" << edge.second << ") ";
    }
    cout << endl;

    // Set vertex property of node 1
    graph.setVertexProperty(v1, VertexProperty<int>(10));
    cout << "Vertex property of node 1: " << graph.getVertexProperty(v1).getValue() << endl;

    // Set edge property of edge (1, 2)
    graph.setEdgeProperty(1, 2, EdgeProperty<int>(20));
    cout << "Edge property of edge (1, 2): " << graph.getEdgeProperty(1, 2).getValue() << endl;

    return 0;
}
