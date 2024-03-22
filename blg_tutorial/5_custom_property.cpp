#include <iostream>
#include <string>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

struct EdgeProperty{
    EdgeProperty(std::string name, int distance) : name(name), distance(distance){};
    std::string name;
    int distance;
};

int main()
{
    typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeProperty, no_property> Graph;
    Graph g(5);

    graph_traits<Graph>::vertex_descriptor v0 = vertex(0, g);
    graph_traits<Graph>::vertex_descriptor v1 = vertex(1, g);
    graph_traits<Graph>::vertex_descriptor v2 = vertex(2, g);

    // edge_property_type等同於EdgeProperty
    typedef Graph::edge_property_type EP;
    graph_traits<Graph>::edge_descriptor e0 = add_edge(v0, v1, EP("hello", 10), g).first;
    add_edge(v0, v2 ,EdgeProperty("A", 11), g);

    property_map<Graph, int EP::*>::type distance_map1 = get(&EP::distance, g);
    std::cout << distance_map1[e0] << std::endl;

    property_map<Graph, int EdgeProperty::*>::type distance_map2 = get(&EdgeProperty::distance, g);
    std::cout << distance_map2[e0] << std::endl;

    // 補充: 類別成員指針
    int EdgeProperty::*distance_ptr = &EdgeProperty::distance;
    EdgeProperty e_obj("D", 20);
    std::cout << e_obj.*distance_ptr << std::endl;
    std::cout << e_obj.distance << std::endl;
}