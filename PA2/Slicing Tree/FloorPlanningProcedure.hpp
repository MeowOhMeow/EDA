#ifndef FLOORPLANNINGPROCEDURE_HPP
#define FLOORPLANNINGPROCEDURE_HPP

#include <iostream>
#include <random>
#include <unordered_map>
#include <chrono>

#include "SlicingTree.hpp"
#include "CombinationsOfMacros.hpp"

using namespace std;

const int M1 = 0;
const int M2 = 1;
const int M3 = 2;

const int NUM_MOVES = 3;

class FloorPlanningProcedure
{
private:
    const string HORIZONTAL = "+";
    const string VERTICAL = "*";

    std::chrono::high_resolution_clock::time_point end;

    int bestIndex, index;
    int n, k;
    int movesCount = 0;
    int uphillCount = 0;
    int rejectCount = 0;

    // Swap two adjacent operands.
    vector<string> move1(vector<string> &expressions)
    {
        int expressionsSize = expressions.size();
        int nodeIndex = FloorPlanningProcedure::getRandomNumber(0, expressionsSize - 2);

        // Find the index of the first non-operator expression after nodeIndex
        while (expressions[nodeIndex] == HORIZONTAL || expressions[nodeIndex] == VERTICAL)
        {
            nodeIndex = (nodeIndex + 1) % expressionsSize;
        }

        int nextNonOperatorIndex = (nodeIndex + 1) % expressionsSize;
        while (expressions[nextNonOperatorIndex] == HORIZONTAL || expressions[nextNonOperatorIndex] == VERTICAL)
        {
            nextNonOperatorIndex = (nextNonOperatorIndex + 1) % expressionsSize;
        }

        // Swap the expressions at nodeIndex and nextNonOperatorIndex
        swap(expressions[nodeIndex], expressions[nextNonOperatorIndex]);

        return expressions;
    }

    // Complement some chain (V = H, H = V).
    vector<string> move2(vector<string> &expressions)
    {
        int expressionsSize = expressions.size();
        int nodeIndex1 = FloorPlanningProcedure::getRandomNumber(0, expressionsSize - 1);
        int nodeIndex2 = FloorPlanningProcedure::getRandomNumber(0, expressionsSize - 1);
        if (nodeIndex1 > nodeIndex2)
        {
            swap(nodeIndex1, nodeIndex2);
        }

        // Find the range of operator expressions [nodeIndex1, nodeIndex2]
        for (int i = nodeIndex1; i <= nodeIndex2; ++i)
        {
            if (expressions[i] == HORIZONTAL)
            {
                expressions[i] = VERTICAL;
            }
            else if (expressions[i] == VERTICAL)
            {
                expressions[i] = HORIZONTAL;
            }
        }

        return expressions;
    }

    /*
    Swap two adjacent operators and operand.
    while not (done) do
        Select two adjacent operand ei and operator ei+1;
        if (ei-1 ≠ ei+1)  and (2 Ni+1 < i) then done ← TRUE;
    */
    vector<string> move3(vector<string> &expressions)
    {
        int expressionsSize = expressions.size();
        if (expressionsSize < 4)
        {
            return expressions;
        }
        int operatorCount = 0;

        // Count the number of operators before nodeIndex
        int nodeIndex = FloorPlanningProcedure::getRandomNumber(1, expressionsSize - 3);
        for (int i = 1; i <= nodeIndex + 1; ++i)
        {
            if (expressions[i] == HORIZONTAL || expressions[i] == VERTICAL)
            {
                operatorCount++;
            }
        }

        // Check conditions for adjacent operand and operator
        while (true)
        {
            if ((2 * operatorCount <= nodeIndex) 
            && (expressions[nodeIndex] == HORIZONTAL || expressions[nodeIndex] == VERTICAL) 
            && (expressions[nodeIndex + 1] != HORIZONTAL && expressions[nodeIndex + 1] != VERTICAL))
            {
                swap(expressions[nodeIndex], expressions[nodeIndex + 1]);
                break;
            }
            // Move to the next position
            nodeIndex = (nodeIndex + 1) % (expressionsSize - 3);
            if (nodeIndex == 0)
            {
                nodeIndex = 1;
                operatorCount = 0;
            }
            else if (expressions[nodeIndex] == HORIZONTAL || expressions[nodeIndex] == VERTICAL)
            {
                operatorCount++;
            }
        }

        return expressions;
    }

public:
    bool improving = true;
    bool run = true;

    FloorPlanningProcedure() {}
    ~FloorPlanningProcedure() {}

    inline void initialize()
    {
        run = true;
        movesCount = 0;
        rejectCount = 0;
        uphillCount = 0;
    }

    inline vector<string> makeRandomModification(vector<string> &expressions)
    {
        movesCount++;
        if (movesCount > n * 2)
        {
            run = false;
        }
        // make a random modification to the current tree (state)
        switch (getRandomNumber(0, NUM_MOVES - 1))
        {
        case M1:
            return move1(expressions);
            break;
        case M2:
            return move2(expressions);
            break;
        case M3:
            return move3(expressions);
            break;

        default:
            cerr << "Invalid move" << endl;
            return expressions;
            break;
        }
    }

    inline float evaluateState(vector<string> &expressions, unordered_map<string, pair<int, int>> &macrosMap)
    {
        SlicingTree tree(expressions, macrosMap);
        CombinationsOfMacros rootData = tree.getRootData();
        vector<pair<int, int>> dimensions = rootData.getDimensions();
        index = 0;
        int minArea = max(dimensions[0].first, dimensions[0].second);
        minArea *= minArea;
        for (size_t i = 1; i < dimensions.size(); i++)
        {
            int area = max(dimensions[i].first, dimensions[i].second);
            area *= area;
            if (area < minArea)
            {
                minArea = area;
                index = i;
            }
        }
        return minArea;
    }

    inline bool isImproving()
    {
        // check if the current state is improving
        if (rejectCount / movesCount > 0.95)
        {
            improving = false;
        }
        return improving;
    }

    inline bool hasTimeExpired()
    {
        // check if there is still time left
        return chrono::high_resolution_clock::now() < end;
    }

    inline void setN(int n)
    {
        n = n;
    }

    inline void setK(int k)
    {
        k = k;
    }

    inline void accept()
    {
        bestIndex = index;
    }

    inline void uphill()
    {
        uphillCount++;
        if (uphillCount > n)
        {
            run = false;
        }
    }

    inline void reject()
    {
        rejectCount++;
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
};

#endif // FLOORPLANNINGPROCEDURE_HPP
