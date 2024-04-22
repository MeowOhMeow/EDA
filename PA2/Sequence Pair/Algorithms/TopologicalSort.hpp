#ifndef TOPOLOGICALSORT_HPP
#define TOPOLOGICALSORT_HPP

#include <iostream>
#include <vector>

#include "../Graph/Graph.hpp"

using namespace std;

// Define a class for topological sort
template <class VertexData, class EdgeData>
class Topological
{
private:
    // helper function to perform topological sort
    static void topologicalSortUtil(Graph<VertexData, EdgeData> &graph, int node, vector<bool> &visited, vector<int> &topologicalOrder)
    {
        visited[node] = true;

        vector<pair<int, float>> outEdges = graph.getOutEdges(Vertex(node));
        for (const auto &edge : outEdges)
        {
            if (!visited[edge.first])
            {
                topologicalSortUtil(graph, edge.first, visited, topologicalOrder);
            }
        }

        topologicalOrder.push_back(node);
    }

public:
    // Method to perform topological sort
    static vector<int> sort(Graph<VertexData, EdgeData> &graph)
    {
        vector<int> topologicalOrder;
        vector<bool> visited(graph.size(), false);

        for (int i = 0; i < graph.size(); ++i)
        {
            if (!visited[i])
            {
                topologicalSortUtil(graph, i, visited, topologicalOrder);
            }
        }

        reverse(topologicalOrder.begin(), topologicalOrder.end());

        return topologicalOrder;
    }
};

#endif // TOPOLOGICALSORT_HPP