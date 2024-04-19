#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "SlicingTree.hpp"
#include "CombinationsOfMacros.hpp"
#include "SimulatedAnnealing.hpp"
#include "FloorPlanningProcedure.hpp"

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

SlicingTree createTree(string filename, unordered_map<string, pair<int, int>> &macrosMap)
{
    vector<CombinationsOfMacros> macros;
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
        macros.push_back(CombinationsOfMacros(name, w, h));
        macrosMap[name] = {w, h};
    }
    file.close();

    SlicingTree tree(macros);

    return tree;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <inputfile> <outputfile>" << endl;
        // return 1;
    }
    // create a tree from the input file
    unordered_map<string, pair<int, int>> macrosMap;
    SlicingTree tree = createTree("..\\testcases\\floorplan_6.txt", macrosMap);

    // print the tree
    tree.printStructure();
    vector<string> expression = tree.getExpressions();
    cout << "Expression: ";
    for (string s : expression)
    {
        cout << s;
    }
    cout << endl;

    SlicingTree tree2(expression, macrosMap);
    expression = tree2.getExpressions();
    cout << "Expression: ";
    for (string s : expression)
    {
        cout << s;
    }
    cout << endl;

    // create a procedure for sa
    FloorPlanningProcedure procedure;
    // create a simulated annealing object
    SimulatedAnnealing sa(10, 0.9, 0.1, 7);
    // run the simulated annealing algorithm
    vector<string> bestExpressions = sa.run(tree, procedure, macrosMap);
    cout << "Best expressions: ";
    for (string s : bestExpressions)
    {
        cout << s;
    }
    cout << endl;
    // create a tree from the best expressions
    SlicingTree bestTree(bestExpressions, macrosMap);
    cout << "Best tree: " << endl;
    // print the best tree
    bestTree.printStructure();
    // print the best tree's area
    cout << "Area: " << procedure.evaluateState(bestExpressions, macrosMap) << endl;
    return 0;
}