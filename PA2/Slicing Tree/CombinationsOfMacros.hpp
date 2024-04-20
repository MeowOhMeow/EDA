#ifndef COMBINATIONSOFMACROS_HPP
#define COMBINATIONSOFMACROS_HPP

#include <iostream>
#include <vector>
#include <string>

#include "Constants.hpp"

using namespace std;
using namespace Constants;

class CombinationsOfMacros
{
private:
    string operatorChar = " ";
    vector<pair<int, int>> dimensions = {};
    vector<string> names = {};
    vector<pair<int, int>> combinationIndex = {};

    static void eliminateBadCombinations(CombinationsOfMacros &combinations)
    {
        // Create a vector to mark combinations that should be removed
        std::vector<bool> remove(combinations.dimensions.size(), false);

        for (size_t i = 0; i < combinations.dimensions.size(); i++)
        {
            for (size_t j = i + 1; j < combinations.dimensions.size(); j++)
            {
                // Check if combination j dominates combination i
                if (combinations.dimensions[i].first <= combinations.dimensions[j].first &&
                    combinations.dimensions[i].second <= combinations.dimensions[j].second)
                {
                    // Mark combination i for removal
                    remove[i] = true;
                    break; // Move to the next i since i is dominated by j
                }
                // Check if combination i dominates combination j
                else if (combinations.dimensions[i].first >= combinations.dimensions[j].first &&
                         combinations.dimensions[i].second >= combinations.dimensions[j].second)
                {
                    // Mark combination j for removal
                    remove[j] = true;
                }
            }
        }

        // Remove marked combinations in reverse order to avoid invalidating indices
        for (int i = remove.size() - 1; i >= 0; i--)
        {
            if (remove[i])
            {
                combinations.dimensions.erase(combinations.dimensions.begin() + i);
                combinations.combinationIndex.erase(combinations.combinationIndex.begin() + i);
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
        }
        else
        {
            dimensions.push_back({w, h});
            names.push_back(n);
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
        }
        else
        {
            dimensions.push_back({w, h});
            names.push_back(n);
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
                // string name = " ";
                int w = max(dimensions[i].first, other.dimensions[j].first);
                int h = dimensions[i].second + other.dimensions[j].second;
                result.dimensions.push_back({w, h});
                // result.names.push_back(name);
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
                // string name = " ";
                int w = dimensions[i].first + other.dimensions[j].first;
                int h = max(dimensions[i].second, other.dimensions[j].second);
                result.dimensions.push_back({w, h});
                // result.names.push_back(name);
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