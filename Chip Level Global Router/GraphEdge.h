#ifndef GraphEdge_H
#define GraphEdge_H

#define HORISONTAL 0
#define VERTICAL 1
#define NO_DIRECTION -1

class GraphEdge
{
public:
    int id;
    float cost;
    int dir = NO_DIRECTION;

    GraphEdge(int i, float w) : id(i), cost(w) {}
};

#endif // GraphEdge_H