#include <iostream>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

template<class Graph>
void print_vertex_and_edge(Graph& g){
    typename graph_traits<Graph>::vertex_iterator vi, vi_end;
    typename graph_traits<Graph>::edge_iterator ei, ei_end;

    typename property_map<Graph, vertex_index_t>::type v_index = get(vertex_index, g);

    std::cout << "vertex: ";
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; vi++)
    {
        std::cout << v_index[*vi] << " ";
    }
    std::cout << std::endl;

    std::cout << "edges: ";
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    {
        printf("(%d, %d) ", source(*ei, g), target(*ei, g));
    }
    std::cout << std::endl;
}

int main()
{
    const int V = 3;
    typedef adjacency_list<vecS, vecS, undirectedS> Graph;

    Graph g(V); // 多少edge

    // 取得vertex discriptor
    graph_traits<Graph>::vertex_descriptor v0 = vertex(0, g);
    graph_traits<Graph>::vertex_descriptor v1 = vertex(1, g);
    graph_traits<Graph>::vertex_descriptor v2 = vertex(2, g);

    // 新增 edge
    add_edge(v0, v1, g);
    add_edge(v0, v2, g);
    add_edge(v2, v1, g);
    add_edge(v2, v1, g); // edge用vecS、listS可以是multigraph，但setS不行

    print_vertex_and_edge(g);

    // 新增vertex
    graph_traits<Graph>::vertex_descriptor v3 = add_vertex(g);

    print_vertex_and_edge(g);

    // 刪除edge
    graph_traits<Graph>::edge_descriptor e0 = edge(v0, v1, g).first; // edge 回傳 std::pair< graph_traits<Graph>::edge_descriptor, bool > ，bool 代表edge存不存在
    remove_edge(e0, g);

    print_vertex_and_edge(g);

    // 刪除vertex
    clear_vertex(v0, g);
    remove_vertex(v0, g);

    print_vertex_and_edge(g);
}
