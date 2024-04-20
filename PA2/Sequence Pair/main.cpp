#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include "Macro.hpp"

using namespace std;

struct KeyValuePair
{
    int key;
    int x;
    int y;
    Macro *value;

    KeyValuePair(int x, int y, Macro *value) : key(x + y), x(x), y(y), value(value) {}
};

bool compare(KeyValuePair &a, KeyValuePair &b)
{
    return a.key < b.key;
}

vector<string> split(string str, char delim)
{
    vector<string> res;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim))
    {
        res.push_back(token);
    }
    return res;
}

vector<Macro> getMacros(string filename)
{
    vector<Macro> macros;
    ifstream file(filename);
    string line;
    int nums;
    getline(file, line);
    vector<string> tokens = split(line, ' ');
    nums = stoi(tokens[1]);
    getline(file, line);
    getline(file, line);
    for (int i = 0; i < nums; i++)
    {
        getline(file, line);
        vector<string> tokens = split(line, ' ');
        string name = tokens[0];
        int w = stoi(tokens[1]);
        int h = stoi(tokens[2]);
        macros.push_back(Macro(name, w, h));
    }
    file.close();

    return macros;
}

vector<pair<int, int>> getCoordinates(vector<int> &seq1, vector<int> &seq2)
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

int main()
{
    vector<Macro> macros = getMacros("../testcases/floorplan_6.txt");
    cout << "Macros:" << endl;
    for (const auto &macro : macros)
    {
        macro.print();
    }
    cout << endl;

    vector<int> seq1 = {0, 1, 3, 4, 2, 5};
    vector<int> seq2 = {2, 1, 5, 0, 3, 4};
    vector<pair<int, int>> coordinates = getCoordinates(seq1, seq2);
    vector<KeyValuePair> pairs;
    for (size_t i = 0; i < coordinates.size(); i++)
    {
        cout << "x: " << coordinates[i].first << " y: " << coordinates[i].second << endl;
        pairs.push_back(KeyValuePair(coordinates[i].first, coordinates[i].second, &macros[seq1[i]]));
    }

    sort(pairs.begin(), pairs.end(), compare);

    cout << "Coordinates:" << endl;
    for (const auto &pair : pairs)
    {
        cout << pair.value->getName() << ", x: " << pair.x << ", y: " << pair.y << endl;
    }

    return 0;
}
