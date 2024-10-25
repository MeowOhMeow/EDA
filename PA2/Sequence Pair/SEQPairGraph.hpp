#ifndef SEQPAIR_GRAPH_HPP
#define SEQPAIR_GRAPH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <numeric>
#include <algorithm>

#include "Macro.hpp"
#include "Rectangle.hpp"
#include "Graph/Graph.hpp"

using namespace std;

// Define a class for the sequence pair graph
class SequencePairGraph : public Graph<Rectangle<int>, NoProperty>
{
private:
    int numNodes;

    inline void checkAndAddEdge(int v1, int v2)
    {
        int x1 = getVertexProperty(v1).getValue().getX();
        int y1 = getVertexProperty(v1).getValue().getY();
        int x2 = getVertexProperty(v2).getValue().getX();
        int y2 = getVertexProperty(v2).getValue().getY();

        if (x1 < x2 && y1 < y2)
        {
            addDirectedEdge(v1, v2, getVertexProperty(v1).getValue().getValue());
        }
    }

    void initEdges()
    {

        for (int v1 = 0; v1 < numNodes; v1++)
        {
            for (int v2 = 0; v2 < numNodes; v2++)
            {
                if (v1 != v2)
                {
                    checkAndAddEdge(v1, v2);
                }
            }
            // Add edges from source to all nodes
            addDirectedEdge(numNodes, v1, 0);
            // Add edges from all nodes to sink
            addDirectedEdge(v1, numNodes + 1, getVertexProperty(v1).getValue().getValue());
        }
    }

    inline void maintainEdges(int v1, int v2)
    {
        clearEdges(v1);
        clearEdges(v2);

        // only check for edges from others to v1 and v2, and from v1 and v2 to others
        for (int i = 0; i < numNodes; i++)
        {
            if (i != v1)
            {
                checkAndAddEdge(i, v1);
                checkAndAddEdge(v1, i);
            }
            if (i != v2)
            {
                checkAndAddEdge(i, v2);
                checkAndAddEdge(v2, i);
            }
        }
        // Add edges from source to v1 and v2
        addDirectedEdge(numNodes, v1, 0);
        addDirectedEdge(numNodes, v2, 0);
        // Add edges from v1 and v2 to sink
        addDirectedEdge(v1, numNodes + 1, getVertexProperty(v1).getValue().getValue());
        addDirectedEdge(v2, numNodes + 1, getVertexProperty(v2).getValue().getValue());
    }

public:
    SequencePairGraph() : Graph<Rectangle<int>, NoProperty>(0) {}
    SequencePairGraph(vector<int> &macroSizes, bool isVertical = false)
        : Graph<Rectangle<int>, NoProperty>(macroSizes.size() + 2)
    {
        numNodes = static_cast<int>(macroSizes.size());

        if (isVertical)
        {
            for (int i = 0; i < numNodes; i++)
            {
                setVertexProperty(i, Rectangle<int>(i, numNodes - 1 - i, macroSizes[i]));
            }
        }
        else
        {
            for (int i = 0; i < numNodes; i++)
            {
                setVertexProperty(i, Rectangle<int>(i, i, macroSizes[i]));
            }
        }
        setVertexProperty(numNodes, Rectangle<int>(-1, -1, 0));
        setVertexProperty(numNodes + 1, Rectangle<int>(numNodes, numNodes, 0));

        initEdges();
    }

    ~SequencePairGraph()
    {
    }

    void swapX(int v1, int v2)
    {
        int temp = getVertexProperty(v1).getValue().getX();
        getVertexProperty(v1).getValue().setW(getVertexProperty(v2).getValue().getX());
        getVertexProperty(v2).getValue().setW(temp);

        maintainEdges(v1, v2);
    }

    void swapY(int v1, int v2)
    {
        int temp = getVertexProperty(v1).getValue().getY();
        getVertexProperty(v1).getValue().setH(getVertexProperty(v2).getValue().getY());
        getVertexProperty(v2).getValue().setH(temp);

        maintainEdges(v1, v2);
    }

    /*
     * Swap the positions of two vertices in both seqX and seqY using their positions
     */
    void swapBoth(int v1, int v2)
    {
        int temp = getVertexProperty(v1).getValue().getX();
        getVertexProperty(v1).getValue().setW(getVertexProperty(v2).getValue().getX());
        getVertexProperty(v2).getValue().setW(temp);

        temp = getVertexProperty(v1).getValue().getY();
        getVertexProperty(v1).getValue().setH(getVertexProperty(v2).getValue().getY());
        getVertexProperty(v2).getValue().setH(temp);

        maintainEdges(v1, v2);
    }

    void updateEdges(int v1)
    {
        int value = getVertexProperty(v1).getValue().getValue();

        for (int v2 : getNeighbors(v1))
        {
            setEdgeWeight(v1, v2, value);
        }
    }
};

#endif // SEQPAIR_GRAPH_HPP