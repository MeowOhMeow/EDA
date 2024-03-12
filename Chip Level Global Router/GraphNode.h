#ifndef GRAPHNODE_H
#define GRAPHNODE_H

class GraphNode
{
public:
    int id;
    float weight;

    GraphNode(int i, float w) : id(i), weight(w) {}
};

#endif // GRAPHNODE_H