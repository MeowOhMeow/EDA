#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <limits>

#include "debug.h"

using namespace std;

#define NIL -1
#define INF numeric_limits<float>::max() / 2

class Dijkstra
{
protected:
    int vertices;

    // (adjacent, weight)
    vector<vector<pair<int, float>>> adj;
    vector<int> parent;
    vector<float> d;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;

    void init_single_source(int s);
    void dijkstra(int s);

public:
    Dijkstra() = default;
};

#endif // DIJKSTRA_H
