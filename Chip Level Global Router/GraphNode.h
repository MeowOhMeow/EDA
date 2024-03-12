#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#define HORISONTAL 0
#define VERTICAL 1
#define NO_DIRECTION -1

class GraphNode
{
public:
    int id;
    float cost;
    int dir = NO_DIRECTION;

    GraphNode(int i, float w) : id(i), cost(w) {}
};

#endif // GRAPHNODE_H