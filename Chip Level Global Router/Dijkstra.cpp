#include "Dijkstra.h"

void Dijkstra::init_single_source(int s)
{
    for (int i = 0; i < vertices; i++)
    {
        cost[i] = INF;
        parent[i] = NIL;
    }
    cost[s] = 0;
}

void Dijkstra::dijkstra(int pos)
{
    init_single_source(pos);
    Q.push(make_pair(cost[pos], pos));

    while (!Q.empty())
    {
        int u = Q.top().second;
        Q.pop();

        for (size_t i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i].id;
            float w = adj[u][i].cost;
            // if there is a turn, the cost should be increased

            if (cost[v] > cost[u] + w)
            {
                cost[v] = cost[u] + w;
                parent[v] = u;
                Q.push(make_pair(cost[v], v));
            }
        }
    }
}
