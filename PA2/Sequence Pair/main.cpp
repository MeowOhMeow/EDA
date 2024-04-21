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
#include "Algorithms/TopologicalSort.hpp"
#include "Algorithms/LongestPath.hpp"

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
    cout << "Macro names, widths, and heights:" << endl;
    for (Macro &macro : macros)
    {
        macroWidths.push_back(macro.getWidth());
        macroHeights.push_back(macro.getHeight());
        cout << macro.getName() << " " << macro.getWidth() << " " << macro.getHeight() << endl;
    }
    cout << endl;

    SequencePairGraph horizontalGraph(macroWidths);
    SequencePairGraph verticalGraph(macroHeights, true);

    // swap
    horizontalGraph.swapX(2, 4);
    horizontalGraph.swapX(2, 3);
    horizontalGraph.swapY(0, 3);
    horizontalGraph.swapY(0, 4);
    horizontalGraph.swapY(0, 5);
    horizontalGraph.swapY(0, 2);
    cout << "Horizontal Graph:" << endl;
    horizontalGraph.print();

    verticalGraph.swapY(5 - 2, 5 - 4);
    verticalGraph.swapY(5 - 2, 5 - 3);
    verticalGraph.swapX(0, 3);
    verticalGraph.swapX(0, 4);
    verticalGraph.swapX(0, 5);
    verticalGraph.swapX(0, 2);
    cout << "Vertical Graph:" << endl;
    verticalGraph.print();

    vector<int> seqX_horizontal = horizontalGraph.getSeqX();
    vector<int> seqY_horizontal = horizontalGraph.getSeqY();
    vector<int> seqX_vertical = verticalGraph.getSeqX();
    vector<int> seqY_vertical = verticalGraph.getSeqY();

    cout << "seqx_horizontal: ";
    for (int i = 0; i < seqX_horizontal.size(); i++)
    {
        cout << seqX_horizontal[i] << " ";
    }
    cout << endl;
    cout << "seqy_horizontal: ";
    for (int i = 0; i < seqY_horizontal.size(); i++)
    {
        cout << seqY_horizontal[i] << " ";
    }
    cout << endl;

    cout << "seqx_vertical: ";
    for (int i = 0; i < seqX_vertical.size(); i++)
    {
        cout << seqX_vertical[i] << " ";
    }
    cout << endl;
    cout << "seqy_vertical: ";
    for (int i = 0; i < seqY_vertical.size(); i++)
    {
        cout << seqY_vertical[i] << " ";
    }
    cout << endl;

    // print x and y coordinates
    cout << "horizontalGraph:" << endl;
    for (int i = 0; i < macros.size(); i++)
    {
        cout << "name: " << macros[i].getName() << " ";
        cout << "X: " << horizontalGraph.getVertexProperty(i).getValue()->getX() << " ";
        cout << "Y: " << horizontalGraph.getVertexProperty(i).getValue()->getY() << endl;
    }
    cout << endl;

    cout << "verticalGraph:" << endl;
    for (int i = 0; i < macros.size(); i++)
    {
        cout << "name: " << macros[i].getName() << " ";
        cout << "X: " << verticalGraph.getVertexProperty(i).getValue()->getX() << " ";
        cout << "Y: " << verticalGraph.getVertexProperty(i).getValue()->getY() << endl;
    }
    cout << endl;

    // topological sort
    vector<int> topologicalOrder1 = Topological<Coordinates<int> *, NoProperty>::sort(horizontalGraph);
    cout << "Topological Order:" << endl;
    for (int i = 0; i < topologicalOrder1.size(); i++)
    {
        cout << topologicalOrder1[i] << " ";
    }
    cout << endl;
    vector<int> topologicalOrder2 = Topological<Coordinates<int> *, NoProperty>::sort(verticalGraph);
    cout << "Topological Order:" << endl;
    for (int i = 0; i < topologicalOrder2.size(); i++)
    {
        cout << topologicalOrder2[i] << " ";
    }
    cout << endl;

    // longest path
    vector<float> longestPath1 = LongestPath<Coordinates<int> *, NoProperty>::find(horizontalGraph, topologicalOrder1);
    cout << "Longest Path:" << endl;
    for (int i = 0; i < longestPath1.size(); i++)
    {
        cout << longestPath1[i] << " ";
    }
    cout << endl;
    vector<float> longestPath2 = LongestPath<Coordinates<int> *, NoProperty>::find(verticalGraph, topologicalOrder2);
    cout << "Longest Path:" << endl;
    for (int i = 0; i < longestPath2.size(); i++)
    {
        cout << longestPath2[i] << " ";
    }
    cout << endl;

    return 0;
}
