#ifndef SEQPAIR_GRAPH_HPP
#define SEQPAIR_GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <set>
#include <limits>
#include <numeric>
#include <algorithm>

#include "Macro.hpp"
#include "Coordinates.hpp"
#include "Graph/Graph.hpp"

using namespace std;

// Define a class for the sequence pair graph
class SequencePairGraph : public Graph<Coordinates<int> *, NoProperty>
{
private:
    vector<int> seqX, seqY;
    vector<int> macroSizes;

    int numNodes;

    struct KeyValuePair
    {
        int key;
        int x;
        int y;
        int *value;

        KeyValuePair(int x, int y, int *value) : key(x + y), x(x), y(y), value(value) {}

        static bool compare(const KeyValuePair &a, const KeyValuePair &b)
        {
            return a.key < b.key;
        }
    };

    vector<pair<int, int>> getCoordinates()
    {
        vector<pair<int, int>> coordinates;
        unordered_map<int, int> seq2_map;
        // Create a mapping of elements in seq2 to their indices
        for (int i = 0; i < seqY.size(); i++)
        {
            seq2_map[seqY[i]] = i;
        }

        // Generate coordinates based on seq1 and seq2_map
        for (int i = 0; i < seqX.size(); i++)
        {
            if (seq2_map.find(seqX[i]) != seq2_map.end())
            {
                coordinates.push_back({i, seq2_map[seqX[i]]});
            }
        }

        return coordinates;
    }

    inline void checkAndAddEdge(int v1, int v2)
    {
        int x1 = getVertexProperty(v1).getValue()->getX();
        int y1 = getVertexProperty(v1).getValue()->getY();
        int x2 = getVertexProperty(v2).getValue()->getX();
        int y2 = getVertexProperty(v2).getValue()->getY();

        if (x1 < x2 && y1 < y2)
        {
            addDirectedEdge(v1, v2, getVertexProperty(v1).getValue()->getValue());
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
            addDirectedEdge(v1, numNodes + 1, getVertexProperty(v1).getValue()->getValue());
        }
    }

    void maintainEdges(int v1, int v2)
    {
        clearEdges(v1);
        clearEdges(v2);

        // only check for edges from others to v1 and v2, and from v1 and v2 to others
        for (int i = 0; i < numNodes; i++)
        {
            if (i != v1)
            {
                checkAndAddEdge(i, v1);
            }
            if (i != v2)
            {
                checkAndAddEdge(i, v2);
            }
        }
        // Add edges from source to v1 and v2
        addDirectedEdge(numNodes, v1, 0);
        addDirectedEdge(numNodes, v2, 0);
        // Add edges from v1 and v2 to sink
        addDirectedEdge(v1, numNodes + 1, getVertexProperty(v1).getValue()->getValue());
        addDirectedEdge(v2, numNodes + 1, getVertexProperty(v2).getValue()->getValue());
    }

public:
    // Constructor
    SequencePairGraph(vector<int> &macroSizes, bool isVertical = false)
        : Graph<Coordinates<int> *, NoProperty>(macroSizes.size() + 2), macroSizes(macroSizes)
    {
        numNodes = static_cast<int>(macroSizes.size());

        seqX.resize(numNodes);
        seqY.resize(numNodes);
        iota(seqX.begin(), seqX.end(), 0);
        iota(seqY.begin(), seqY.end(), 0);
        if (isVertical)
        {
            reverse(seqY.begin(), seqY.end());
        }

        vector<pair<int, int>> coordinates = getCoordinates();
        for (size_t i = 0; i < coordinates.size(); i++)
        {
            int x = coordinates[i].first;
            int y = coordinates[i].second;

            setVertexProperty(i, new Coordinates<int>(x, y, macroSizes[seqX[x]]));
        }
        setVertexProperty(numNodes, new Coordinates<int>(-1, -1, 0));
        setVertexProperty(numNodes + 1, new Coordinates<int>(numNodes, numNodes, 0));

        initEdges();
    }

    ~SequencePairGraph()
    {
        for (int i = 0; i < size(); i++)
        {
            delete getVertexProperty(i).getValue();
        }
    }

    void swapX(int pos1, int pos2)
    {
        int temp = getVertexProperty(seqX[pos1]).getValue()->getX();
        getVertexProperty(seqX[pos1]).getValue()->setX(getVertexProperty(seqX[pos2]).getValue()->getX());
        getVertexProperty(seqX[pos2]).getValue()->setX(temp);
        maintainEdges(seqX[pos1], seqX[pos2]);

        swap(seqX[pos1], seqX[pos2]);
    }

    void swapY(int pos1, int pos2)
    {
        int temp = getVertexProperty(seqY[pos1]).getValue()->getY();
        getVertexProperty(seqY[pos1]).getValue()->setY(getVertexProperty(seqY[pos2]).getValue()->getY());
        getVertexProperty(seqY[pos2]).getValue()->setY(temp);
        maintainEdges(seqY[pos1], seqY[pos2]);

        swap(seqY[pos1], seqY[pos2]);
    }

    void swapBoth(int v1, int v2)
    {
        cerr << "Not implemented" << endl;
    }

    vector<int> getSeqX() const
    {
        return seqX;
    }

    vector<int> getSeqY() const
    {
        return seqY;
    }
};

#endif // SEQPAIR_GRAPH_HPP