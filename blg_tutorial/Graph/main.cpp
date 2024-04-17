// boost includes
#include <boost/graph/adjacency_list.hpp>

// stl includes
#include <iostream>
#include <stack>

// custom includes
#include "Wrapper.hpp"

// namespace
using namespace boost;
using namespace std;
using namespace graph_namespace;

Wrapper buildTestGraph()
{
    // Graph initialization
    const int num_nodes = 9;
    Edge edges[] = {Edge(0, 1), Edge(0, 3), Edge(1, 2), Edge(1, 4), Edge(2, 5), Edge(3, 4), Edge(4, 5), Edge(3, 6), Edge(6, 7), Edge(7, 8), Edge(5, 8), Edge(4, 7)};
    int weights[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int num_edges = sizeof(edges) / sizeof(Edge);

    return Wrapper(edges, edges + num_edges, weights, num_nodes);
}

int main()
{
    Wrapper wrapper = buildTestGraph();

    // Define start and target vertices
    Vertex s = 0;
    Vertex t = 8;

    // Run Dijkstra's algorithm
    wrapper.runDijkstra(s, t);

    // Get maps
    DistanceMap distance_map = wrapper.getDistanceMap();
    PredecessorMap predecessor_map = wrapper.getPredecessorMap();

    // Reconstruct path
    std::stack<Vertex> path = reconstructPath(s, t, predecessor_map);

    // Print info
    printPath(path);
    std::cout << "Total cost: " << distance_map[t] << std::endl;

    // Run A* algorithm
    wrapper.runAStar(s, t);

    // Get maps
    distance_map = wrapper.getDistanceMap();
    predecessor_map = wrapper.getPredecessorMap();

    // Reconstruct path
    path = reconstructPath(s, t, predecessor_map);

    // Print info
    printPath(path);
    std::cout << "Total cost: " << distance_map[t] << std::endl;

    return 0;
}
