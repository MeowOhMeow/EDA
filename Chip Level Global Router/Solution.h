#ifndef SOLUTION_H
#define SOLUTION_H

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <limits>

#include "utility.h"
#include "debug.h"
#include "Dijkstra.h"

using namespace std;

class Solution : public Dijkstra
{
private:
    int rows, cols;
    int capacity;
    float alpha;
    string input_file, output_file;

    // paths
    vector<vector<int>> paths;

    void choose_path(int i, int s, int v);
    void update_adj(int pos);
    void load();
    void init();

public:
    Solution(string input_file, string output_file);

    void solve();
    void save_path();
};

extern int num_nets;
extern vector<pair<pair<int, int>, pair<int, int>>> nets;

#endif // SOLUTION_H
