#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

template <class PredecessorMap> // 在此範例，PredecessorMap的類型是 *vector<Vertex>
class my_record_predecessors : public dijkstra_visitor<>
{
public:
    my_record_predecessors(PredecessorMap p) : m_predecessor(p) {}

    template <class Edge, class Graph>
    void edge_relaxed(Edge e, Graph &g)
    {
        put(m_predecessor, target(e, g), source(e, g));
    }

protected:
    PredecessorMap m_predecessor;
};

template <class PredecessorMap> // 在此範例，PredecessorMap的類型是 *vector<Vertex>
my_record_predecessors<PredecessorMap> make_predecessor_recorder(PredecessorMap p)
{
    return my_record_predecessors<PredecessorMap>(p);
}

int main()
{
    typedef property<edge_weight_t, int> EdgeProperty;
    typedef adjacency_list<listS, vecS, directedS, no_property, EdgeProperty> Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef std::pair<int, int> E;

    const int num_nodes = 5;
    E edges[] = {E(0, 2), E(1, 1), E(1, 3), E(1, 4),
                 E(2, 1), E(2, 3), E(3, 4), E(4, 0), E(4, 1)};
    int weights[] = {1, 2, 1, 2, 7, 3, 1, 1, 1};
    int num_edges = sizeof(edges) / sizeof(E);

    Graph G(edges, edges + num_edges, weights, num_nodes);
    std::vector<int> d(num_vertices(G), (std::numeric_limits<int>::max)());
    Vertex s = *(vertices(G).first);
    std::vector<Vertex> p(num_vertices(G), graph_traits<Graph>::null_vertex());

    // 显式地指定distance_map和predecessor_map的类型
    typedef iterator_property_map<std::vector<int>::iterator, property_map<Graph, vertex_index_t>::const_type, int, int &> DistanceMap;
    typedef iterator_property_map<std::vector<Vertex>::iterator, property_map<Graph, vertex_index_t>::const_type, Vertex, Vertex &> PredecessorMap;

    
    DistanceMap my_distance_map(d.begin(), get(vertex_index, G));
    PredecessorMap predecessor_map(p.begin(), get(vertex_index, G));

    // 使用修改后的参数调用dijkstra_shortest_paths
    dijkstra_shortest_paths(G, s, distance_map(my_distance_map).visitor(make_predecessor_recorder(predecessor_map)));
    // dijkstra_shortest_paths(G, s, distance_map(my_distance_map)); // 不紀錄predecessor

    // 获取 vertex_index 的映射
    auto index = get(vertex_index, G);

    std::cout << "distances from start vertex:" << std::endl;
    graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (tie(vi, vi_end) = vertices(G); vi != vi_end; ++vi)
    {
        std::cout << "distance(" << index[*vi] << ") = " << d[*vi] << std::endl;
    }

    std::cout << "parents in the tree of shortest paths:" << std::endl;
    for (tie(vi, vi_end) = vertices(G); vi != vi_end; ++vi)
    {
        std::cout << "parent(" << index[*vi];
        if (p[*vi] == graph_traits<Graph>::null_vertex())
            std::cout << ") = no parent" << std::endl;
        else
            std::cout << ") = " << index[p[*vi]] << std::endl;
    }
    return 0;
}
