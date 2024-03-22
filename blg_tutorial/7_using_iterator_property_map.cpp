#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;

int main()
{
    // 定義一個簡單的無向圖
    typedef adjacency_list<vecS, vecS, undirectedS> Graph;
    Graph g;

    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(1, 2, g);

    // 為每個頂點準備標籤存儲空間
    std::vector<int> labels(num_vertices(g));

    // 使用迭代器屬性映射來關聯頂點和標籤
    typedef iterator_property_map<std::vector<int>::iterator, property_map<Graph, vertex_index_t>::type, int , int&> LabelMap;
    LabelMap label_map(labels.begin(), get(vertex_index, g));
    // 解釋:
    // 第二個模板參數(property_map<Graph, vertex_index_t>::type) : 這是索引映射的類型，它將vector中的位置映射到圖的頂點索引。
    // 這裡使用get(vertex_index, g) 來獲取一個到頂點索引的映射，它基於圖g的內部頂點索引。這個映射確保每個頂點的索引可以被用作訪問對應labels向量中標籤的鍵
    // ps.
    // property_map<Graph, vertex_index_t>::type indexMap =  get(vertex_index, g);
    // int v_index = indexMap[some_vertex_discritor];

    // 賦值操作
    graph_traits<Graph>::vertex_iterator vi, vi_end;
    int label_value = 0;
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi, ++label_value)
    {
        label_map[*vi] = label_value;
    }

    // 讀取操作
    for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    {
        std::cout << "Vertex " << *vi << " has label " << label_map[*vi] << std::endl;
    }

    return 0;
}
