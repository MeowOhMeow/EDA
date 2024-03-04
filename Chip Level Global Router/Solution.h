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

#include "debug.h"
#include "Dijkstra.h"

using namespace std;

class Solution : public Dijkstra
{
private:
    int rows, cols;
    int capacity;
    float alpha;

    // paths
    vector<vector<int>> paths;

    void choose_path(int i, int s, int v);
    void update_adj(int pos);

public:
    Solution() = default;
    Solution(int cols, int rows, int capacity);

    void solve();
    void save_path(string output_file);
};

extern int num_nets;
extern vector<pair<pair<int, int>, pair<int, int>>> nets;

#endif // SOLUTION_H
