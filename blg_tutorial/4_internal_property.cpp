#include <iostream>
#include <string>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;


namespace boost
{
    enum vertex_my_property_t { vertex_my_property = 112 };
    BOOST_INSTALL_PROPERTY(vertex, my_property);
}
int main()
{

    // 定義圖結構，包括自定義和內置的屬性
    typedef property<vertex_name_t, std::string> VertexPropertyName;                     // 頂點名字屬性，內置屬性
    typedef property<vertex_my_property_t, int, VertexPropertyName> VertexProperty;      // 自定義頂點屬性，附加在名字屬性之上

    typedef property<edge_index_t, int> EdgeProperty;                                    // 邊的索引屬性，內置屬性

    typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty> Graph; // 圖類型定義

    Graph g(3); // 創建一個包含3個頂點的無向圖

    // 獲取頂點描述符
    graph_traits<Graph>::vertex_descriptor v0 = vertex(0, g);
    graph_traits<Graph>::vertex_descriptor v1 = vertex(1, g);
    graph_traits<Graph>::vertex_descriptor v2 = vertex(2, g);

    graph_traits<Graph>::edge_descriptor e0 = add_edge(v0, v1, g).first;

    // 獲取名字屬性映射
    property_map<Graph, vertex_name_t>::type name_map = get(vertex_name, g);

    // 使用put函數設置頂點名字
    put(name_map, v0, "123");       // put(property_map, key, value) - 設置v0的名字為"123"
    name_map[v1] = "456";           // 直接通過屬性映射設置v1的名字為"456"，這是property_map提供的另一種設置方式
    put(vertex_name, g, v2, "789"); // put(property tag, graph, key, value) - 設置v2的名字為"789"

    // 使用get函數獲取頂點名字
    std::cout << "vertex v0 name: " << name_map[v0] << std::endl;            // 直接通過屬性映射獲取v0的名字
    std::cout << "vertex v1 name: " << name_map[v1] << std::endl;            // 直接通過屬性映射獲取v1的名字
    std::cout << "vertex v1 name: " << get(name_map, v1) << std::endl;       // get(property_map, key) - 通過屬性映射和鍵獲取v1的名字
    std::cout << "vertex v1 name: " << get(vertex_name, g, v1) << std::endl; // get(property tag, graph, key) - 通過屬性標籤、圖和鍵獲取v1的名字
    std::cout << "vertex v2 name: " << get(vertex_name, g, v2) << std::endl; // 同上，獲取v2的名字

    // 自定義的internal property
    property_map<Graph, vertex_my_property_t>::type my_property_map = get(vertex_my_property, g);
    put(my_property_map, v0, 111);
    std::cout << "vertex v0 my property: " << get(my_property_map, v0) << std::endl;

    put(edge_index, g, e0, 0);
    std::cout << "edge e0 edge index: " << get(edge_index, g, e0) << std::endl;
}