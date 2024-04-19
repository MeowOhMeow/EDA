#ifndef COMBINATIONSOFMACROS_HPP
#define COMBINATIONSOFMACROS_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// enum Direction
// {
//     ORIGINAL,
//     ROTATED
// };

class CombinationsOfMacros
{
private:
    const string HORIZONTAL = "+";
    const string VERTICAL = "*";

    string operatorChar = " ";
    vector<pair<int, int>> dimensions = {};
    vector<string> names = {};
    // vector<Direction> directions = {};
    vector<pair<int, int>> combinationIndex = {};

    int offsetX = 0;
    int offsetY = 0;

    static void eliminateBadCombinations(CombinationsOfMacros &combinations)
    {
        for (size_t i = 0; i < combinations.dimensions.size(); i++)
        {
            for (size_t j = 0; j < combinations.dimensions.size(); j++)
            {
                if (i != j && combinations.dimensions[i].first <= combinations.dimensions[j].first && combinations.dimensions[i].second <= combinations.dimensions[j].second)
                {
                    combinations.dimensions.erase(combinations.dimensions.begin() + j);
                    combinations.names.erase(combinations.names.begin() + j);
                    // combinations.directions.erase(combinations.directions.begin() + j);
                    combinations.combinationIndex.erase(combinations.combinationIndex.begin() + j);
                    j--;
                }
            }
        }
    }

public:
    CombinationsOfMacros()
    {
    }

    CombinationsOfMacros(string n, int w, int h)
    {
        if (w != h)
        {
            dimensions.push_back({w, h});
            dimensions.push_back({h, w});
            names.push_back(n);
            names.push_back(n);
            // directions.push_back(ORIGINAL);
            // directions.push_back(ROTATED);
        }
        else
        {
            dimensions.push_back({w, h});
            names.push_back(n);
            // directions.push_back(ORIGINAL);
        }
    }

    CombinationsOfMacros(string n, pair<int, int> d)
    {
        int w = d.first;
        int h = d.second;
        if (w != h)
        {
            dimensions.push_back({w, h});
            dimensions.push_back({h, w});
            names.push_back(n);
            names.push_back(n);
            // directions.push_back(ORIGINAL);
            // directions.push_back(ROTATED);
        }
        else
        {
            dimensions.push_back({w, h});
            names.push_back(n);
            // directions.push_back(ORIGINAL);
        }
    }

    string getOperatorChar() const
    {
        return operatorChar;
    }

    vector<pair<int, int>> getDimensions() const
    {
        return dimensions;
    }

    pair<int, int> getDimensions(int index) const
    {
        return dimensions[index];
    }

    // vector<Direction> getDirections() const
    // {
    //     return directions;
    // }

    // Direction getDirection(int index) const
    // {
    //     return directions[index];
    // }

    vector<string> getNames() const
    {
        return names;
    }

    string getName(int index) const
    {
        return names[index];
    }

    vector<pair<int, int>> getCombinationIndex() const
    {
        return combinationIndex;
    }

    pair<int, int> getCombinationIndex(int index) const
    {
        return combinationIndex[index];
    }

    string toString() const
    {
        if (operatorChar == " ")
        {
            return names[0];
        }
        else
        {
            return operatorChar;
        }
    }

    int getSize() const
    {
        return dimensions.size();
    }

    // overload the + operator for horizontal slices combinations
    CombinationsOfMacros
    operator+(const CombinationsOfMacros &other) const
    {
        CombinationsOfMacros result;
        result.operatorChar = HORIZONTAL;
        for (size_t i = 0; i < dimensions.size(); i++)
        {
            for (size_t j = 0; j < other.dimensions.size(); j++)
            {
                string name = "(" + names[i] + HORIZONTAL + other.names[j] + ")";
                int w = max(dimensions[i].first, other.dimensions[j].first);
                int h = dimensions[i].second + other.dimensions[j].second;
                result.dimensions.push_back({w, h});
                result.names.push_back(name);
                result.combinationIndex.push_back({i, j});
            }
        }
        // eliminate bad combinations
        eliminateBadCombinations(result);
        return result;
    }

    // overload the * operator for vertical slices combinations
    CombinationsOfMacros operator*(const CombinationsOfMacros &other) const
    {
        CombinationsOfMacros result;
        result.operatorChar = VERTICAL;
        for (size_t i = 0; i < dimensions.size(); i++)
        {
            for (size_t j = 0; j < other.dimensions.size(); j++)
            {
                string name = "(" + names[i] + VERTICAL + other.names[j] + ")";
                int w = dimensions[i].first + other.dimensions[j].first;
                int h = max(dimensions[i].second, other.dimensions[j].second);
                result.dimensions.push_back({w, h});
                result.names.push_back(name);
                result.combinationIndex.push_back({i, j});
            }
        }
        // eliminate bad combinations
        eliminateBadCombinations(result);
        return result;
    }

    // overload the << operator
    friend ostream &operator<<(ostream &os, const CombinationsOfMacros &combinations)
    {
        for (size_t i = 0; i < combinations.dimensions.size(); i++)
        {
            os << "(" << combinations.dimensions[i].first << "," << combinations.dimensions[i].second << ")";
        }
        return os;
    }
};

#endif // COMBINATIONSOFMACROS_HPP