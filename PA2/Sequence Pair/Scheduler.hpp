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

    // move 1: swapX
    inline void move1(int v1, int v2)
    {
        horizontalGraph->swapX(v1, v2);
        verticalGraph->swapY(v1, v2);
        previousMove = M1;
        previousIndices = {v1, v2};
    }

    inline void move2(int v1, int v2)
    {
        horizontalGraph->swapY(v1, v2);
        verticalGraph->swapX(v1, v2);
        previousMove = M2;
        previousIndices = {v1, v2};
    }

    inline void move3(int v)
    {
        int width = horizontalGraph->getVertexProperty(v).getValue()->getValue();
        int height = verticalGraph->getVertexProperty(v).getValue()->getValue();
        horizontalGraph->getVertexProperty(v).getValue()->setValue(height);
        verticalGraph->getVertexProperty(v).getValue()->setValue(width);
        horizontalGraph->updateValue(v);
        verticalGraph->updateValue(v);
        previousMove = M3;
        previousIndices = {v, -1};
    }

    inline void move4(int v1, int v2)
    {
        horizontalGraph->swapBoth(v1, v2);
        verticalGraph->swapBoth(v1, v2);
        previousMove = M4;
        previousIndices = {v1, v2};
    }

public:
    Scheduler(vector<Macro> &macros, int k = 7, int timeLimit = 10) : k(k), macros(macros)
    {
        start = chrono::high_resolution_clock::now();
        end = start + chrono::minutes(timeLimit);

        numNodes = macros.size();

        vector<int> macroWidths, macroHeights;
        for (Macro &macro : macros)
        {
            macroWidths.push_back(macro.getWidth());
            macroHeights.push_back(macro.getHeight());
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
        // make a random modification to the current tree (state)
        switch (getRandomNumber(0, NUM_MOVES - 1))
        {
        case M1:
            move1(v1, v2);
            break;
        case M2:
            move2(v1, v2);
            break;
        case M3:
            move3(v1);
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
            move3(previousIndices.first);
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
        if (rejectCount / movesCount > 0.95)
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