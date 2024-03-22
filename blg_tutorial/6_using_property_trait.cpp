#include <iostream>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

int main()
{
    const int V = 3;
    // 定義圖，並將vertex_name_t設為頂點屬性
    typedef property<vertex_name_t, std::string> VertexProperty;
    typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty> Graph;
    Graph g(V);

    // 獲取一個頂點名稱的屬性映射
    typedef property_map<Graph, vertex_name_t>::type NameMap;
    NameMap name_map = get(vertex_name, g);

    // 使用property_traits來獲取鍵和值的類型
    typedef property_traits<NameMap>::key_type keyType;     // 鍵的類型通常是頂點描述符
    typedef property_traits<NameMap>::value_type valueType; // 值的類型是你的屬性類型，這裡是std::string

    // 給幾個頂點設置名字
    put(name_map, vertex(0, g), "Vertex 0");
    put(name_map, vertex(1, g), "Vertex 1");
    put(name_map, vertex(2, g), "Vertex 2");

    // 使用property_traits來獲取和打印每個頂點的名字
    graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    {
        keyType v = *vi;                   // 使用property_traits中的key_type
        valueType name = get(name_map, v); // 使用property_traits中的value_type
        std::cout << "Vertex " << v << " name: " << name << std::endl;
    }

    // 等同
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    {
        graph_traits<Graph>::vertex_descriptor v = *vi;                   // 使用property_traits中的key_type
        std::string name = get(name_map, v); // 使用property_traits中的value_type
        std::cout << "Vertex " << v << " name: " << name << std::endl;
    }

    return 0;
}
