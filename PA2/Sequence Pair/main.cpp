#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include "Macro.hpp"
#include "SEQPairGraph.hpp"
#include "Coordinates.hpp"

using namespace std;

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

int main()
{
    vector<Macro> macros = getMacros("../testcases/floorplan_6.txt");
    vector<int> macroWidths, macroHeights;
    for (Macro &macro : macros)
    {
        macroWidths.push_back(macro.getWidth());
        macroHeights.push_back(macro.getHeight());
    }

    SequencePairGraph horizontalGraph(macroWidths);

    cout << "Vertex Properties: " << endl;
    for (int i = 0; i < horizontalGraph.size(); i++)
    {
        Coordinates<int> *coord = horizontalGraph.getVertexProperty(i).getValue();
        cout << "Vertex " << i << ": " << coord->getX() << " " << coord->getY() << " Width: " << *coord->getValue() << endl;
        coord->setValue(new int(10));
        coord->setX(100);
    }

    cout << "Vertex Properties after modification: " << endl;
    for (int i = 0; i < horizontalGraph.size(); i++)
    {
        Coordinates<int> *coord = horizontalGraph.getVertexProperty(i).getValue();
        cout << "Vertex " << i << ": " << coord->getX() << " " << coord->getY() << " Width: " << *coord->getValue() << endl;
    }

    return 0;
}
