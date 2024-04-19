#ifndef FLOORPLANNINGPROCEDURE_HPP
#define FLOORPLANNINGPROCEDURE_HPP

#include <iostream>
#include <random>
#include <unordered_map>
#include <chrono>
#include <iomanip>

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

    int n, k;
    int movesCount = 0;
    int uphillCount = 0;
    int rejectCount = 0;

    // Swap two adjacent operands.
    vector<string> move1(vector<string> &expressions)
    {
        int expressionsSize = expressions.size();
        int nodeIndex = getRandomNumber(0, expressionsSize - 2);

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
        int nodeIndex1 = getRandomNumber(0, expressionsSize - 1);
        int nodeIndex2 = getRandomNumber(0, expressionsSize - 1);
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
        int nodeIndex = getRandomNumber(1, expressionsSize - 3);
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
            if ((expressions[nodeIndex] == HORIZONTAL || expressions[nodeIndex] == VERTICAL) && (expressions[nodeIndex + 1] != HORIZONTAL && expressions[nodeIndex + 1] != VERTICAL))
            {
                break;
            }
            else if ((expressions[nodeIndex] != HORIZONTAL && expressions[nodeIndex] != VERTICAL) && (expressions[nodeIndex + 1] == HORIZONTAL || expressions[nodeIndex + 1] == VERTICAL))
            {
                if (2 * operatorCount < nodeIndex)

                    break;
            }
            // Move to the next position
            nodeIndex = (nodeIndex + 1) % (expressionsSize - 3);
            if (nodeIndex == 0)
            {
                nodeIndex = 1;
                operatorCount = expressions[2] == HORIZONTAL || expressions[2] == VERTICAL ? 1 : 0;
            }
            else if (expressions[nodeIndex + 1] == HORIZONTAL || expressions[nodeIndex + 1] == VERTICAL)
            {
                operatorCount++;
            }
        }

        swap(expressions[nodeIndex], expressions[nodeIndex + 1]);

        return expressions;
    }

public:
    bool improving = true;
    bool run = true;
    int warmUpCount = 0;

    double temperature = 1;
    double coolingRate = 0.95;

    FloorPlanningProcedure()
    {
        end = chrono::high_resolution_clock::now() + chrono::minutes(10);
    }

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
        if (movesCount > 2 * n * k)
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

    inline double evaluateState(vector<string> &expressions, unordered_map<string, pair<int, int>> &macrosMap)
    {
        SlicingTree tree(expressions, macrosMap);
        CombinationsOfMacros rootData = tree.getRootData();
        vector<pair<int, int>> dimensions = rootData.getDimensions();
        int minArea = INT_MAX;
        for (size_t i = 0; i < dimensions.size(); i++)
        {
            int area = max(dimensions[i].first, dimensions[i].second);
            if (area < minArea)
            {
                minArea = area;
            }
        }
        return minArea;
    }

    inline bool isImproving()
    {
        warmUpCount++;
        if (warmUpCount < n)
        {
            int completion = (warmUpCount * 100) / (n);
            cout << "Warmming up..." << setw(4) << completion << "% [" << string(completion / 2, '=') << string(50 - completion / 2, ' ') << "]\r";
            return true;
        }
        temperature *= coolingRate;
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
        return chrono::high_resolution_clock::now() >= end;
    }

    inline void setN(int n)
    {
        this->n = n;
    }

    inline void setK(int k)
    {
        this->k = k;
    }

    inline void accept()
    {
    }

    inline void setTemperature(double temperature)
    {
        this->temperature = temperature;
    }

    inline void setCoolingRate(double coolingRate)
    {
        this->coolingRate = coolingRate;
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

    // in seconds
    inline int getRemainingTime()
    {
        return chrono::duration_cast<chrono::seconds>(end - chrono::high_resolution_clock::now()).count();
    }

    inline double getTemperature()
    {
        return temperature;
    }
};

#endif // FLOORPLANNINGPROCEDURE_HPP