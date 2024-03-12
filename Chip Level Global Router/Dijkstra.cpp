#include "Dijkstra.h"

void Dijkstra::init_single_source(int s)
{
    for (int i = 0; i < vertices; i++)
    {
        d[i] = INF;
        parent[i] = NIL;
    }
    d[s] = 0;
}

void Dijkstra::dijkstra(int pos)
{
    init_single_source(pos);
    Q.push(make_pair(d[pos], pos));

    while (!Q.empty())
    {
        int u = Q.top().second;
        Q.pop();

        for (size_t i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i].id;
            float w = adj[u][i].weight;

            if (d[v] > d[u] + w)
            {
                d[v] = d[u] + w;
                parent[v] = u;
                Q.push(make_pair(d[v], v));
            }
        }
    }
}
