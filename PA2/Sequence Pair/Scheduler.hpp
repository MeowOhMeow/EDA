#ifndef SCHEUDLER_HPP
#define SCHEUDLER_HPP

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <functional>
#include <cassert>

#include "Macro.hpp"
#include "SEQPairGraph.hpp"
#include "Rectangle.hpp"
#include "Algorithms/TopologicalSort.hpp"
#include "Algorithms/LongestPath.hpp"
#include "BaseScheduler.hpp"

using namespace std;

class Scheduler : public BaseScheduler
{
private:
    SequencePairGraph *horizontalGraph = nullptr, *verticalGraph = nullptr;
    int numNodes;

    vector<Macro> macros;
    vector<vector<pair<int, int>>> macroDimensions;
    vector<int> macroDimensionsIndex;

    pair<int, int> previousIndices;
    vector<function<void(int, int)>> moves;

    int stepsPerIteration = 0;

    // Move functions encapsulated in std::function<>
    inline void move1(int v1, int v2)
    {
        horizontalGraph->swapX(v1, v2);
        verticalGraph->swapY(v1, v2);

        previousMove = 0;
        previousIndices = {v1, v2};
    }

    inline void move2(int v1, int v2)
    {
        horizontalGraph->swapY(v1, v2);
        verticalGraph->swapX(v1, v2);

        previousMove = 1;
        previousIndices = {v1, v2};
    }

    inline void move3(int v, int aspectIndex = -1)
    {
        int originalIndex = macroDimensionsIndex[v];
        int newAspectIndex = 0;
        if (aspectIndex == -1)
        {
            newAspectIndex = (originalIndex + 1) % macroDimensions[v].size();
            macroDimensionsIndex[v] = newAspectIndex;
        }
        else
        {
            macroDimensionsIndex[v] = aspectIndex;
            newAspectIndex = aspectIndex;
        }
        int w = macroDimensions[v][newAspectIndex].first;
        int h = macroDimensions[v][newAspectIndex].second;
        horizontalGraph->getVertexProperty(v).getValue().setValue(w);
        verticalGraph->getVertexProperty(v).getValue().setValue(h);
        horizontalGraph->updateEdges(v);
        verticalGraph->updateEdges(v);

        previousMove = 2;
        previousIndices = {v, originalIndex};
    }

    inline void move4(int v1, int v2)
    {
        horizontalGraph->swapBoth(v1, v2);
        verticalGraph->swapBoth(v1, v2);

        previousMove = 3;
        previousIndices = {v1, v2};
    }

    void initializeMoves()
    {
        // Define the moves using std::function<>. These moves can now be stored in a vector.
        moves.push_back([this](int v1, int v2)
                        { move1(v1, v2); });
        moves.push_back([this](int v1, int v2)
                        { move2(v1, v2); });
        moves.push_back([this](int v, int)
                        { move3(v, 0); });
        moves.push_back([this](int v1, int v2)
                        { move4(v1, v2); });

        // Initialize the CDF based on move probabilities
        cdf.resize(moves.size());
        partial_sum(moveProbabilities.begin(), moveProbabilities.end(), cdf.begin());
    }

    // Random node selection
    inline int getRandomNode()
    {
        return getRandomNumber(0, numNodes - 1);
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
    Scheduler(vector<Macro> &macros, float minAspectRatio, float maxAspectRatio, int k = 7, int timeLimit = 10)
        : BaseScheduler(1.0, 0.95, timeLimit, k), macros(macros)
    {
        numNodes = macros.size();
        macroDimensions.resize(numNodes);
        macroDimensionsIndex.resize(numNodes, 0);
        moveProbabilities = {0.25, 0.25, 0.25, 0.25};
        stepsPerIteration = 2 * k * numNodes;

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

        // Initialize the moves and CDF
        initializeMoves();
    }

    ~Scheduler()
    {
        delete horizontalGraph;
        delete verticalGraph;
    }

    // Randomly select a move based on the CDF
    void selectMove()
    {
        double r = getRandomNumber(0.0, 1.0);
        auto it = lower_bound(cdf.begin(), cdf.end(), r);
        int moveIndex = distance(cdf.begin(), it);

        int v1 = getRandomNode();
        int v2 = getRandomNode();
        while (v1 == v2)
        {
            v2 = getRandomNode();
        }
        if (moveIndex == 2)
        {
            moves[moveIndex](v1, -1);
        }
        else
        {
            moves[moveIndex](v1, v2);
        }
    }

    inline void initialize() override
    {
        run = true;
        movesCount = 0;
        rejectCount = 0;
        uphillCount = 0;
    }

    inline void makeRandomModification() override
    {
        selectMove();

        movesCount++;
        if (movesCount > stepsPerIteration)
        {
            run = false;
        }
    }

    inline double evaluateState() override
    {
        pair<vector<float>, vector<int>> longestPathH = LongestPath<Rectangle<int>, NoProperty>::findLongestPath(*horizontalGraph);
        pair<vector<float>, vector<int>> longestPathV = LongestPath<Rectangle<int>, NoProperty>::findLongestPath(*verticalGraph);

        return max(longestPathH.first.back(), longestPathV.first.back());
    }

    inline void accept() override {}

    inline void uphill() override
    {
        uphillCount++;
        if (uphillCount > numNodes)
        {
            run = false;
        }
    }

    inline void reject() override
    {
        rejectCount++;

        moves[previousMove](previousIndices.first, previousIndices.second);
    }

    inline int getStepsPerIteration() override
    {
        return stepsPerIteration;
    }

    void saveFloorplan(string filename)
    {
        ofstream file(filename);

        vector<int> topologicalOrderH = Topological<Rectangle<int>, NoProperty>::sort(*horizontalGraph);
        vector<int> topologicalOrderV = Topological<Rectangle<int>, NoProperty>::sort(*verticalGraph);
        vector<float> costsH = LongestPath<Rectangle<int>, NoProperty>::find(*horizontalGraph, topologicalOrderH);
        vector<float> costsV = LongestPath<Rectangle<int>, NoProperty>::find(*verticalGraph, topologicalOrderV);

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
            int w = horizontalGraph->getVertexProperty(i).getValue().getValue();
            int h = verticalGraph->getVertexProperty(i).getValue().getValue();
            file << macros[i].getName() << " " << w << " " << h << " " << x << " " << y << " " << x + w << " " << y + h << endl;
        }

        file.close();
    }
};

#endif // SCHEUDLER_HPP
