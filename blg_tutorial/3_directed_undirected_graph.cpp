#include <iostream>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

template <class Graph>
void print_vertex_and_edge(Graph &g)
{
    typename graph_traits<Graph>::vertex_iterator vi, vi_end;
    typename graph_traits<Graph>::edge_iterator ei, ei_end;



    typename graph_traits<Graph>::vertex_descriptor v2 = vertex(2, g);

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
template <class Graph>
void print_out_edge(Graph &g, int index = 2)
{
    typename graph_traits<Graph>::vertex_iterator vi, vi_end;

    typename graph_traits<Graph>::vertex_descriptor v2 = vertex(index, g);

    typename property_map<Graph, vertex_index_t>::type v_index = get(vertex_index, g);

    

    typename graph_traits<Graph>::out_edge_iterator out_ei, out_ei_end;
    std::cout << "out edges: ";
    for (tie(out_ei, out_ei_end) = out_edges(v2, g); out_ei != out_ei_end; ++out_ei)
    {
        printf("(%d, %d) ", source(*out_ei, g), target(*out_ei, g));
    }
    std::cout << std::endl;
    


}

template <class Graph>
void print_in_edge(Graph &g, int index = 2)
{
    typename graph_traits<Graph>::vertex_iterator vi, vi_end;

    typename graph_traits<Graph>::vertex_descriptor v2 = vertex(index, g);

    typename property_map<Graph, vertex_index_t>::type v_index = get(vertex_index, g);

   
    typename graph_traits<Graph>::in_edge_iterator in_ei, in_ei_end;
    std::cout << "in edges: ";
    for (tie(in_ei, in_ei_end) = in_edges(v2, g); in_ei != in_ei_end; ++in_ei)
    {
        printf("(%d, %d) ", source(*in_ei, g), target(*in_ei, g));
    }
    std::cout << std::endl;
    
}

int main()
{
    const int V = 3;
    typedef adjacency_list<vecS, vecS, directedS> GraphDirected;
    typedef adjacency_list<vecS, vecS, undirectedS> GraphUndirected;
    typedef adjacency_list<vecS, vecS, bidirectionalS> GraphBidirected;
    // bidirectionalS跟directedS的差別為:
    // bidirectionalS允許有效地訪問每個頂點的入邊（in - edges）和出邊（out - edges）。 
    // 在有向圖中，訪問入邊通常需要更多的存儲空間，且許多算法不需要訪問入邊。但是在雙向圖中，這不是問題。

    GraphDirected gd(V);
    GraphUndirected gud(V);
    GraphBidirected gbid(V);

    // 用 vecS的vertex儲存方式可用 0、1、2... 來訪問，但listS不行
    add_edge(0, 1, gd);
    add_edge(0, 2, gd);
    add_edge(1, 2, gd);
    add_edge(2, 1, gd);
    // print_vertex_and_edge(gd);
    // print_out_edge(gd);
    // print_in_edge(gd); //建議用bidirectional，比較有效率

    add_edge(0, 1, gud);
    add_edge(0, 2, gud);
    add_edge(1, 2, gud);
    add_edge(2, 1, gud);
    print_vertex_and_edge(gud);
    print_out_edge(gud, 0);
    print_in_edge(gud, 0);
    print_out_edge(gud, 1);
    print_in_edge(gud, 1);
    print_out_edge(gud, 2);
    print_in_edge(gud, 2);

    add_edge(0, 1, gbid);
    add_edge(0, 2, gbid);
    add_edge(1, 2, gbid);
    add_edge(2, 1, gbid);
    // print_vertex_and_edge(gbid);
    // print_out_edge(gbid);
    // print_in_edge(gbid);
}