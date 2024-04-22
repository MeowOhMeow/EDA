#ifndef LONGESTPATH_HPP
#define LONGESTPATH_HPP

#include <iostream>
#include <vector>
#include <limits>

#include "../Graph/Graph.hpp"
#include "TopologicalSort.hpp"

using namespace std;

// Define a class for longest path
template <class VertexData, class EdgeData>
class LongestPath
{
private:
public:
    // Method to perform longest path
    static vector<float> find(Graph<VertexData, EdgeData> &graph)
    {
        vector<int> topologicalOrder = Topological<VertexData, EdgeData>::sort(graph);
        vector<float> distances(graph.size(), -numeric_limits<float>::infinity());
        distances[topologicalOrder[0]] = 0;

        for (size_t i = 0; i < topologicalOrder.size(); ++i)
        {
            int node = topologicalOrder[i];
            vector<pair<int, float>> inEdges = graph.getInEdges(Vertex(node));
            for (const auto &edge : inEdges)
            {
                distances[node] = max(distances[node], distances[edge.first] + graph.getEdgeWeight(edge.first, node));
            }
        }

        return distances;
    }

    // given topological order, find the longest path
    static vector<float> find(Graph<VertexData, EdgeData> &graph, vector<int> &topologicalOrder)
    {
        vector<float> distances(graph.size(), -numeric_limits<float>::infinity());
        distances[topologicalOrder[0]] = 0;

        for (size_t i = 0; i < topologicalOrder.size(); ++i)
        {
            int node = topologicalOrder[i];
            vector<pair<int, float>> inEdges = graph.getInEdges(Vertex(node));
            for (const auto &edge : inEdges)
            {
                distances[node] = max(distances[node], distances[edge.first] + graph.getEdgeWeight(edge.first, node));
            }
        }

        return distances;
    }
};

#endif // LONGESTPATH_HPP