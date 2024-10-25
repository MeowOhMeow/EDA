#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <limits>

#include "VertexProperty.hpp"
#include "EdgeProperty.hpp"
#include "Vertex.hpp"
#include "NoProperty.hpp"

using namespace std;

// Define a class for the graph
template <class VertexData, class EdgeData>
class Graph
{
private:
    EdgeProperty<EdgeData> emptyEdgeProperty = EdgeProperty<EdgeData>();
    VertexProperty<VertexData> emptyVertexProperty = VertexProperty<VertexData>();

    vector<map<int, float>> outEdgesList, inEdgesList;
    vector<VertexProperty<VertexData>> vertexPropertiesMap;
    vector<unordered_map<int, EdgeProperty<EdgeData>>> edgePropertiesMap;

public:
    // Constructor
    Graph(int numNodes)
        : outEdgesList(numNodes),
          inEdgesList(numNodes),
          edgePropertiesMap(numNodes)
    {
        for (int i = 0; i < numNodes; ++i)
        {
            vertexPropertiesMap.push_back(emptyVertexProperty);
        }
    }

    // Method to add a bidirected edge
    void addBidirectedEdge(int source, int target, float weight1, float weight2)
    {
        addDirectedEdge(source, target, weight1);
        addDirectedEdge(target, source, weight2);
    }

    void addBidirectedEdge(const Vertex &source, const Vertex &target, float weight1, float weight2)
    {
        addBidirectedEdge(source.getId(), target.getId(), weight1, weight2);
    }

    // Method to add an undirected edge
    void addUndirectedEdge(int source, int target, float weight)
    {
        addBidirectedEdge(source, target, weight, weight);
    }

    void addUndirectedEdge(const Vertex &source, const Vertex &target, float weight)
    {
        addUndirectedEdge(source.getId(), target.getId(), weight);
    }

    // Method to add a directed edge
    void addDirectedEdge(int source, int target, float weight)
    {
        outEdgesList[source][target] = weight;
        inEdgesList[target][source] = weight;
        edgePropertiesMap[source][target] = emptyEdgeProperty;
    }

    void addDirectedEdge(const Vertex &source, const Vertex &target, float weight)
    {
        addDirectedEdge(source.getId(), target.getId(), weight);
    }

    // Method to get edge property
    EdgeProperty<EdgeData> &getEdgeProperty(int source, int target)
    {
        auto it = edgePropertiesMap[source].find(target);
        if (it != edgePropertiesMap[source].end())
        {
            return it->second;
        }
        return emptyEdgeProperty;
    }

    EdgeProperty<EdgeData> &getEdgeProperty(const Vertex &source, const Vertex &target)
    {
        return getEdgeProperty(source.getId(), target.getId());
    }

    // Method to get edge weight
    float getEdgeWeight(int source, int target) const
    {
        auto it = outEdgesList[source].find(target);
        if (it != outEdgesList[source].end())
        {
            return it->second;
        }
        return numeric_limits<float>::infinity();
    }

    float getEdgeWeight(const Vertex &source, const Vertex &target) const
    {
        return getEdgeWeight(source.getId(), target.getId());
    }

    // Method to get neighbors of a vertex
    vector<int> getNeighbors(int vertex) const
    {
        vector<int> neighbors;
        for (const auto &neighbor : outEdgesList[vertex])
        {
            neighbors.push_back(neighbor.first);
        }
        return neighbors;
    }

    vector<int> getNeighbors(const Vertex &vertex) const
    {
        return getNeighbors(vertex.getId());
    }

    // Method to get vertex property
    VertexProperty<VertexData> &getVertexProperty(int vertex)
    {
        return vertexPropertiesMap[vertex];
    }

    VertexProperty<VertexData> &getVertexProperty(const Vertex &vertex)
    {
        return getVertexProperty(vertex.getId());
    }

    // Method to set edge property
    void setEdgeProperty(int source, int target, const EdgeProperty<EdgeData> &property)
    {
        edgePropertiesMap[source][target] = property;
    }

    void setEdgeProperty(const Vertex &source, const Vertex &target, const EdgeProperty<EdgeData> &property)
    {
        setEdgeProperty(source.getId(), target.getId(), property);
    }

    // Method to set edge weight
    void setEdgeWeight(int source, int target, float weight)
    {
        outEdgesList[source][target] = weight;
        inEdgesList[target][source] = weight;
    }

    void setEdgeWeight(const Vertex &source, const Vertex &target, float weight)
    {
        setEdgeWeight(source.getId(), target.getId(), weight);
    }

    // Method to set vertex property
    void setVertexProperty(int vertex, const VertexProperty<VertexData> &property)
    {
        vertexPropertiesMap[vertex] = property;
    }

    void setVertexProperty(const Vertex &vertex, const VertexProperty<VertexData> &property)
    {
        setVertexProperty(vertex.getId(), property);
    }

    // get out edges
    vector<pair<int, float>> getOutEdges(int vertex) const
    {
        vector<pair<int, float>> outEdges;
        for (const auto &edge : outEdgesList[vertex])
        {
            outEdges.emplace_back(edge.first, edge.second);
        }
        return outEdges;
    }

    vector<pair<int, float>> getOutEdges(const Vertex &vertex) const
    {
        return getOutEdges(vertex.getId());
    }

    // get in edges
    vector<pair<int, float>> getInEdges(int vertex) const
    {
        vector<pair<int, float>> inEdges;
        for (const auto &edge : inEdgesList[vertex])
        {
            inEdges.emplace_back(edge.first, edge.second);
        }
        return inEdges;
    }

    vector<pair<int, float>> getInEdges(const Vertex &vertex) const
    {
        return getInEdges(vertex.getId());
    }

    vector<map<int, float>> getAdjacencyList() const
    {
        return outEdgesList;
    }

    void clearEdges(int vertex)
    {
        // Clear outgoing edges and update incoming edges
        for (const auto &edge : outEdgesList[vertex])
        {
            inEdgesList[edge.first].erase(vertex);
        }
        outEdgesList[vertex].clear();

        // Clear incoming edges and update outgoing edges
        for (const auto &edge : inEdgesList[vertex])
        {
            outEdgesList[edge.first].erase(vertex);
        }
        inEdgesList[vertex].clear();
    }

    void clearEdges(const Vertex &vertex)
    {
        clearEdges(vertex.getId());
    }

    // Method to get size
    int size() const
    {
        return outEdgesList.size();
    }

    // Method to print the graph
    void print() const
    {
        for (int i = 0; i < outEdgesList.size(); i++)
        {
            cout << i << " -> ";
            for (const auto &edge : outEdgesList[i])
            {
                cout << edge.first << "(" << edge.second << ") ";
            }
            cout << endl;
        }
    }
};

#endif // GRAPH_HPP
