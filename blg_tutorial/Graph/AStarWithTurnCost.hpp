#ifndef A_STAR_WITH_TURN_COST_HPP
#define A_STAR_WITH_TURN_COST_HPP

// boost includes
#include <boost/graph/adjacency_list.hpp>

// stl includes
#include <queue>
#include <tuple>

// custom includes
#include "GraphProperty.hpp"

// namespace
using namespace graph_namespace;
using namespace std;

template <class Graph, class Vertex>
int getHeuristic(const Graph &G, Vertex v, Vertex t)
{
    int result = get(vertex_index, G, t) - get(vertex_index, G, v);
    return abs(result);
}

template <class Graph, class Vertex, class DistanceMap, class DirectionMap, class PredecessorMap>
void AStarWithTurnCost(const Graph &graph, Vertex s, Vertex t, DistanceMap &distance, DirectionMap &direction, PredecessorMap &predecessor, int turnCost)
{
    // Initialize priority queue
    typedef std::pair<int, Vertex> WeightVertexPair;
    std::priority_queue<WeightVertexPair, std::vector<WeightVertexPair>, std::greater<WeightVertexPair>> pq;

    // Initialize distance from start vertex
    distance[s] = 0;
    pq.push(std::make_pair(distance[s] + getHeuristic(graph, s, t), s));

    // Dijkstra's algorithm
    while (!pq.empty())
    {
        // Get vertex with smallest distance
        Vertex curVertex = pq.top().second;
        if (curVertex == t)
        {
            break;
        }
        pq.pop();

        // Get current direction
        Direction curDir = direction[curVertex];

        // Iterate over all outgoing edges from current vertex
        typename graph_traits<Graph>::out_edge_iterator edgeIter, edgeIterEnd;
        for (tie(edgeIter, edgeIterEnd) = out_edges(curVertex, graph); edgeIter != edgeIterEnd; ++edgeIter)
        {
            // Get next vertex
            Vertex nextVertex = target(*edgeIter, graph);
            // Get weight of edge from current vertex to next vertex
            int weight = get(edge_weight, graph, *edgeIter);
            Direction nextDir = (nextVertex == curVertex + 1 || nextVertex == curVertex - 1) ? HORIZONTAL : VERTICAL;
            if (curDir != nextDir && curDir != NONE)
            {
                weight += turnCost;
            }
            weight += getHeuristic(graph, nextVertex, t);
            // Relaxation step
            if (distance[nextVertex] > distance[curVertex] + weight)
            {
                distance[nextVertex] = distance[curVertex] + weight;
                direction[nextVertex] = nextDir;
                predecessor[nextVertex] = curVertex;
                pq.push(std::make_pair(distance[nextVertex], nextVertex));
            }
        }
    }
}

#endif // A_STAR_WITH_TURN_COST_HPP
