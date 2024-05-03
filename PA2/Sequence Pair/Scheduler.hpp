#ifndef SCHEUDLER_HPP
#define SCHEUDLER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <random>

#include "Macro.hpp"
#include "SEQPairGraph.hpp"
#include "Coordinates.hpp"
#include "Algorithms/TopologicalSort.hpp"
#include "Algorithms/LongestPath.hpp"

using namespace std;

const int NUM_MOVES = 4;
enum Moves
{
    M1,
    M2,
    M3,
    M4
};

class Scheduler
{
private:
    chrono::high_resolution_clock::time_point start, end;

    SequencePairGraph *horizontalGraph, *verticalGraph;

    int numNodes, k;
    int movesCount = 0;
    int uphillCount = 0;
    int rejectCount = 0;

    bool improving = true;
    bool run = true;

    double temperature = 1;
    double coolingRate = 0.95;

    Moves previousMove;
    pair<int, int> previousIndices;
    vector<int> previousLongestPathH, previousLongestPathV;

    vector<Macro> macros;
    float minAspectRatio, maxAspectRatio;
    vector<vector<pair<int, int>>> macroDimensions;
    vector<int> macroDimensionsIndex;

    // move 1: swapX
    inline void move1(int v1, int v2)
    {
        horizontalGraph->swapX(v1, v2);
        verticalGraph->swapY(v1, v2);
        previousMove = M1;
        previousIndices = {v1, v2};
    }

    // move 2: swapY
    inline void move2(int v1, int v2)
    {
        horizontalGraph->swapY(v1, v2);
        verticalGraph->swapX(v1, v2);
        previousMove = M2;
        previousIndices = {v1, v2};
    }

    // move 3: change dimensions
    inline void move3(int v, int aspectIndex = -1)
    {
        int originalIndex = macroDimensionsIndex[v];
        if (aspectIndex == -1)
        {
            aspectIndex = (originalIndex + 1) % macroDimensions[v].size();
            macroDimensionsIndex[v] = aspectIndex;
        }
        else
        {
            macroDimensionsIndex[v] = aspectIndex;
        }
        int w = macroDimensions[v][aspectIndex].first;
        int h = macroDimensions[v][aspectIndex].second;
        horizontalGraph->getVertexProperty(v).getValue()->setValue(w);
        verticalGraph->getVertexProperty(v).getValue()->setValue(h);
        horizontalGraph->updateEdges(v);
        verticalGraph->updateEdges(v);
        previousMove = M3;
        previousIndices = {v, originalIndex};
    }

    // move 4: swap both
    inline void move4(int v1, int v2)
    {
        horizontalGraph->swapBoth(v1, v2);
        verticalGraph->swapBoth(v1, v2);
        previousMove = M4;
        previousIndices = {v1, v2};
    }

    vector<pair<int, int>> findIntegerDimensions(int area, float minAspectRatio, float maxAspectRatio)
    {
        vector<pair<int, int>> combinations;

        // width <= height
        for (int width = 1; width * width <= area; ++width)
        {
            if (area % width == 0)
            {
                int height = area / width;
                int minHeight = static_cast<int>(width * minAspectRatio);
                int maxHeight = static_cast<int>(width * maxAspectRatio);

                // Check if height is within the valid range
                if (minHeight <= height && height <= maxHeight)
                {
                    combinations.push_back(make_pair(width, height));
                }
            }
        }
        // width > height, simply swap width and height
        int size = static_cast<int>(combinations.size());
        for (int i = 0; i < size; ++i)
        {
            if (combinations[i].first != combinations[i].second)
            {
                combinations.push_back(make_pair(combinations[i].second, combinations[i].first));
            }
        }

        return combinations;
    }

public:
    Scheduler(vector<Macro> &macros, float minAspectRatio, float maxAspectRatio, int k = 7, int timeLimit = 10) : k(k), macros(macros), minAspectRatio(minAspectRatio), maxAspectRatio(maxAspectRatio)
    {
        start = chrono::high_resolution_clock::now();
        end = start + chrono::minutes(timeLimit);

        numNodes = macros.size();
        macroDimensions.resize(numNodes);
        macroDimensionsIndex.resize(numNodes, 0);

        vector<int> macroWidths, macroHeights;
        for (int i = 0; i < numNodes; i++)
        {
            int area = macros[i].getWidth() * macros[i].getHeight();
            macroDimensions[i] = findIntegerDimensions(area, minAspectRatio, maxAspectRatio);
            if (macroDimensions[i].empty())
            {
                cerr << "No valid dimensions found for macro " << macros[i].getName() << endl;
                exit(1);
            }
            int idx = getRandomNumber(0, macroDimensions[i].size() - 1);
            macroWidths.push_back(macroDimensions[i][idx].first);
            macroHeights.push_back(macroDimensions[i][idx].second);
            macroDimensionsIndex[i] = idx;
        }

        horizontalGraph = new SequencePairGraph(macroWidths);
        verticalGraph = new SequencePairGraph(macroHeights, true);
    }

    ~Scheduler()
    {
        delete horizontalGraph;
        delete verticalGraph;
    }

    inline void initialize()
    {
        run = true;
        movesCount = 0;
        rejectCount = 0;
        uphillCount = 0;
    }

    inline void setTemperature(double temperature)
    {
        this->temperature = temperature;
    }

    inline void setCoolingRate(double coolingRate)
    {
        this->coolingRate = coolingRate;
    }

    // event handlers
    inline void makeRandomModification()
    {
        movesCount++;
        if (movesCount > 2 * numNodes * k)
        {
            run = false;
        }
        int v1 = previousLongestPathH[getRandomNumber(1, previousLongestPathH.size() - 2)];
        int v2 = previousLongestPathV[getRandomNumber(1, previousLongestPathV.size() - 2)];
        if (v1 == v2)
        {
            v2 = (v1 + 1) % numNodes;
        }
        // make a random modification to the current graph (state)
        int move = getRandomNumber(0, NUM_MOVES - 1);
        while (move == M3 && macroDimensions[v1].size() == 1)
        {
            move = getRandomNumber(0, NUM_MOVES - 1);
        }
        switch (move)
        {
        case M1:
            move1(v1, v2);
            break;
        case M2:
            move2(v1, v2);
            break;
        case M3:
            move3(getRandomNumber(0, numNodes - 1));
            break;
        case M4:
            move4(v1, v2);
            break;

        default:
            cerr << "Invalid move" << endl;
            break;
        }
    }

    inline double evaluateState()
    {
        pair<vector<float>, vector<int>> longestPathH = LongestPath<Coordinates<int> *, NoProperty>::findLongestPath(*horizontalGraph);
        pair<vector<float>, vector<int>> longestPathV = LongestPath<Coordinates<int> *, NoProperty>::findLongestPath(*verticalGraph);

        vector<float> costsH = longestPathH.first;
        vector<float> costsV = longestPathV.first;

        previousLongestPathH = longestPathH.second;
        previousLongestPathV = longestPathV.second;

        return max(costsH.back(), costsV.back());
    }

    inline void accept() {}

    inline void uphill()
    {
        uphillCount++;
        if (uphillCount > numNodes)
        {
            run = false;
        }
    }

    inline void reject()
    {
        rejectCount++;

        switch (previousMove)
        {
        case M1:
            move1(previousIndices.first, previousIndices.second);
            break;
        case M2:
            move2(previousIndices.first, previousIndices.second);
            break;
        case M3:
            move3(previousIndices.first, previousIndices.second);
            break;
        case M4:
            move4(previousIndices.first, previousIndices.second);
            break;

        default:
            break;
        }
    }

    // scheduling functions
    inline bool isImproving()
    {
        temperature *= coolingRate;
        // check if the current state is improving
        if (rejectCount / movesCount > 0.99)
        {
            improving = false;
        }
        return improving;
    }

    inline bool canContinue()
    {
        return run;
    }

    inline bool hasTimeExpired()
    {
        // check if there is still time left
        return chrono::high_resolution_clock::now() >= end;
    }

    // in seconds
    inline int getElapsed()
    {
        return chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start).count();
    }

    inline double getTemperature()
    {
        return temperature;
    }

    inline int getStepPerIteration()
    {
        return 2 * numNodes * k;
    }

    // random generator, range: [min, max]
    inline int getRandomNumber(int min, int max)
    {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

    inline float getRandomNumber(float min, float max)
    {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    void saveFloorplan(string filename)
    {
        ofstream file(filename);

        vector<int> topologicalOrderH = Topological<Coordinates<int> *, NoProperty>::sort(*horizontalGraph);
        vector<int> topologicalOrderV = Topological<Coordinates<int> *, NoProperty>::sort(*verticalGraph);
        vector<float> costsH = LongestPath<Coordinates<int> *, NoProperty>::find(*horizontalGraph, topologicalOrderH);
        vector<float> costsV = LongestPath<Coordinates<int> *, NoProperty>::find(*verticalGraph, topologicalOrderV);

        file << costsH.back() << " " << costsV.back() << endl;

        vector<int> xStarts(numNodes), yStarts(numNodes);
        for (int i = 1; i <= numNodes; i++)
        {
            xStarts[topologicalOrderH[i]] = costsH[topologicalOrderH[i]];
            yStarts[topologicalOrderV[i]] = costsV[topologicalOrderV[i]];
        }

        for (int i = 0; i < numNodes; i++)
        {
            int x = xStarts[i];
            int y = yStarts[i];
            int w = horizontalGraph->getVertexProperty(i).getValue()->getValue();
            int h = verticalGraph->getVertexProperty(i).getValue()->getValue();
            file << macros[i].getName() << " " << w << " " << h << " " << x << " " << y << " " << x + w << " " << y + h << endl;
        }

        file.close();
    }
};

#endif // SCHEUDLER_HPP