#include <iostream>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

int main()
{
    const int V = 3;
    typedef adjacency_list<vecS, vecS, undirectedS> Graph1;
    typedef adjacency_list<listS, vecS, undirectedS> Graph2;
    typedef adjacency_list<vecS, listS, undirectedS> Graph3;
    typedef adjacency_list<vecS, setS, undirectedS> Graph4;
    typedef adjacency_list<setS, setS, undirectedS> Graph5;

    Graph1 g(V); // 多少edge

    
}
