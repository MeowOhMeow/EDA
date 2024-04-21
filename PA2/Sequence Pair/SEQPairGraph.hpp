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
    vector<int> seq1, seq2;
    vector<int> macroSizes;

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
        for (int i = 0; i < seq2.size(); i++)
        {
            seq2_map[seq2[i]] = i;
        }

        // Generate coordinates based on seq1 and seq2_map
        for (int i = 0; i < seq1.size(); i++)
        {
            if (seq2_map.find(seq1[i]) != seq2_map.end())
            {
                coordinates.push_back({i, seq2_map[seq1[i]]});
            }
        }

        return coordinates;
    }

    // vector<KeyValuePair> getOrder(vector<int> &seq1, vector<int> &seq2)
    // {
    //     vector<pair<int, int>> coordinates = getCoordinates(seq1, seq2);
    //     vector<KeyValuePair> pairs;
    //     for (size_t i = 0; i < coordinates.size(); i++)
    //     {
    //         pairs.push_back(KeyValuePair(coordinates[i].first, coordinates[i].second, &macros[seq1[i]]));
    //     }

    //     sort(pairs.begin(), pairs.end(), KeyValuePair::compare);

    //     return pairs;
    // }

public:
    // Constructor
    SequencePairGraph(vector<int> &macroSizes)
        : Graph<Coordinates<int> *, NoProperty>(macroSizes.size() + 2), macroSizes(macroSizes)
    {
        int numNodes = static_cast<int>(macroSizes.size());

        seq1.resize(numNodes);
        seq2.resize(numNodes);
        iota(seq1.begin(), seq1.end(), 0);
        iota(seq2.begin(), seq2.end(), 0);

        vector<pair<int, int>> coordinates = getCoordinates();
        for (size_t i = 0; i < coordinates.size(); i++)
        {
            int x = coordinates[i].first;
            int y = coordinates[i].second;
            setVertexProperty(Vertex(seq1[x]), new Coordinates<int>(x, y, &macroSizes[seq1[x]]));
        }
        setVertexProperty(Vertex(numNodes), new Coordinates<int>(-1, -1, new int(0)));
        setVertexProperty(Vertex(numNodes + 1), new Coordinates<int>(numNodes, numNodes, new int(0)));
    }

    ~SequencePairGraph()
    {
        delete getVertexProperty(Vertex(size() - 2)).getValue()->getValue();
        delete getVertexProperty(Vertex(size() - 1)).getValue()->getValue();
        for (int i = 0; i < size(); i++)
        {
            delete getVertexProperty(Vertex(i)).getValue();
        }
    }
};

#endif // SEQPAIR_GRAPH_HPP