1. turn input file into graph
2. Calculate distances between every pair of nodes.
3. Construct a weighted graph G with nodes as vertices and distances as edge weights.
4. Add virtual nodes and corresponding edges to the graph to allow for flexibility in routing.
5. Find the minimum spanning tree T of G using Prim's or Kruskal's algorithm.
6. (Optional) Optimize the spanning tree if necessary.
7. Output the minimum spanning tree T as the optimal connection of nodes with the minimum total distance.
8. Use Dijkstra's algorithm to find the shortest path between every pair of nodes in T.
