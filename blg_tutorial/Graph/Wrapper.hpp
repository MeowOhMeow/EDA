#ifndef WRAPPER_HPP
#define WRAPPER_HPP

// boost includes
#include <boost/graph/adjacency_list.hpp>

// stl includes
#include <limits>
#include <vector>
#include <stack>

// custom includes
#include "DijkstraWithTurnCost.hpp"
#include "AStarWithTurnCost.hpp"
#include "GraphProperty.hpp"

// namespace
using namespace graph_namespace;
using namespace std;

class Wrapper
{
public:
    Wrapper(graph_namespace::Edge *edges, graph_namespace::Edge *edges_end, int *weights, int num_nodes, int turnCost = 1)
    {
        this->turnCost = turnCost;

        G = Graph(edges, edges_end, weights, num_nodes);
        // Dijkstra's algorithm data structures
        distance = vector<int>(num_vertices(G), (std::numeric_limits<int>::max)());
        direction = vector<Direction>(num_vertices(G), NONE);
        predecessor = vector<Vertex>(num_vertices(G), graph_traits<Graph>::null_vertex());


        // Property maps initialization
        distance_map = DistanceMap(distance.begin(), get(vertex_index, G));
        direction_map = DirectionMap(direction.begin(), get(vertex_index, G));
        predecessor_map = PredecessorMap(predecessor.begin(), get(vertex_index, G));
    }

    graph_namespace::Graph getGraph()
    {
        return G;
    }

    void runDijkstra(Vertex s, Vertex t)
    {
        DijkstraWithTurnCost(G, s, t, distance_map, direction_map, predecessor_map, turnCost);
    }

    void runAStar(Vertex s, Vertex t)
    {
        AStarWithTurnCost(G, s, t, distance_map, direction_map, predecessor_map, turnCost);
    }

    // getters
    DistanceMap getDistanceMap()
    {
        return distance_map;
    }

    PredecessorMap getPredecessorMap()
    {
        return predecessor_map;
    }

    DirectionMap getDirectionMap()
    {
        return direction_map;
    }

private:
    // Data members
    Graph G;

    // containers
    vector<int> distance;
    vector<Direction> direction;
    vector<Vertex> predecessor;

    // Property maps
    DistanceMap distance_map;
    DirectionMap direction_map;
    PredecessorMap predecessor_map;

    int turnCost;
};

// Reconstruction of path
template <class Vertex, class PredecessorMap>
std::stack<Vertex> reconstructPath(Vertex source, Vertex target, PredecessorMap &predecessor)
{
    std::stack<Vertex> path = std::stack<Vertex>();
    Vertex curVertex = target;
    while (curVertex != source)
    {
        path.push(curVertex);
        curVertex = predecessor[curVertex];
    }
    path.push(source);
    return path;
}

// Print path
template <class Vertex>
void printPath(std::stack<Vertex> path)
{
    while (!path.empty())
    {
        std::cout << path.top() << " -> ";
        path.pop();
    }
    std::cout << std::endl;
}

#endif // WRAPPER_HPP