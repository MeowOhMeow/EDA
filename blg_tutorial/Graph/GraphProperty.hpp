#ifndef GRAPH_PROPERTY_HPP
#define GRAPH_PROPERTY_HPP

// boost includes
#include <boost/graph/adjacency_list.hpp>

enum Direction
{
    NONE,
    HORIZONTAL,
    VERTICAL
};

// Installing property for vertex direction
namespace boost
{
    enum vertex_direction_t
    {
        vertex_direction
    };
    BOOST_INSTALL_PROPERTY(vertex, direction);
}

namespace graph_namespace
{
    using namespace boost;

    // Graph typedefs
    typedef property<edge_weight_t, int> EdgeProperty;
    typedef property<vertex_direction_t, int> VertexProperty;
    typedef adjacency_list<listS, vecS, directedS, VertexProperty, EdgeProperty> Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef std::pair<int, int> Edge;

    // Property map typedefs
    typedef iterator_property_map<std::vector<int>::iterator, property_map<Graph, vertex_index_t>::const_type, int, int &> DistanceMap;
    typedef iterator_property_map<std::vector<Direction>::iterator, property_map<Graph, vertex_index_t>::const_type, int, Direction &> DirectionMap;
    typedef iterator_property_map<std::vector<Vertex>::iterator, property_map<Graph, vertex_index_t>::const_type, Vertex, Vertex &> PredecessorMap;
}

#endif // GRAPH_PROPERTY_HPP