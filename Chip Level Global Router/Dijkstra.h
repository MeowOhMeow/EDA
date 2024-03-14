#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <limits>

#include "debug.h"
#include "GraphEdge.h"

using namespace std;

#define NIL -1
#define INF numeric_limits<float>::max() / 2

class Dijkstra
{
protected:
    int vertices;

    // (adjacent, cost)
    vector<vector<GraphEdge>> adj;
    vector<int> parent;
    vector<float> cost;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;

    void init_single_source(int s);
    void dijkstra(int s);

public:
    Dijkstra() = default;
};

#endif // DIJKSTRA_H
