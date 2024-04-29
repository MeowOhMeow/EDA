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
            vector<pair<int, float>> inEdges = graph.getInEdges(node);
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
            vector<pair<int, float>> inEdges = graph.getInEdges(node);
            for (const auto &edge : inEdges)
            {
                distances[node] = max(distances[node], distances[edge.first] + graph.getEdgeWeight(edge.first, node));
            }
        }

        return distances;
    }

    static pair<vector<float>, vector<int>> findLongestPath(Graph<VertexData, EdgeData> &graph)
    {
        vector<int> topologicalOrder = Topological<VertexData, EdgeData>::sort(graph);
        vector<float> distances(graph.size(), -numeric_limits<float>::infinity());
        vector<int> predecessors(graph.size(), -1); // To store predecessors

        distances[topologicalOrder[0]] = 0;

        for (size_t i = 0; i < topologicalOrder.size(); ++i)
        {
            int node = topologicalOrder[i];
            vector<pair<int, float>> inEdges = graph.getInEdges(node);
            for (const auto &edge : inEdges)
            {
                float newDistance = distances[edge.first] + graph.getEdgeWeight(edge.first, node);
                if (newDistance > distances[node])
                {
                    distances[node] = newDistance;
                    predecessors[node] = edge.first; // Update predecessor
                }
            }
        }

        // Now, reconstruct the path from source to target using predecessors
        vector<int> reversedPath;
        int current = topologicalOrder.back(); // Start from the target
        while (current != -1)                  // Until we reach the source (predecessor is -1)
        {
            reversedPath.push_back(current);
            current = predecessors[current];
        }
        // reverse(path.begin(), path.end()); // Reverse to get the path from source to target

        return make_pair(distances, reversedPath);
    }
};

#endif // LONGESTPATH_HPP