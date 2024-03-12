#include "Solution.h"

int num_nets;
vector<pair<pair<int, int>, pair<int, int>>> nets;

Solution::Solution(string input_file, string output_file)
{
    this->input_file = input_file;
    this->output_file = output_file;
    load();
    init();
}

void Solution::load()
{
    ifstream fin(input_file);
    Perror(fin, "Failed to open file %s", input_file.c_str());
    string line;
    // grid cols rows
    getline(fin, line);
    vector<string> tmp = split(line, ' ');
    Assert(tmp.size() == 3, "Invalid input file");
    int cols = stoi(tmp[1]);
    int rows = stoi(tmp[2]);
    // capacity #
    getline(fin, line);
    tmp = split(line, ' ');
    Assert(tmp.size() == 2, "Invalid input file");
    int capacity = stoi(tmp[1]);
    // num net #
    getline(fin, line);
    tmp = split(line, ' ');
    Assert(tmp.size() == 3, "Invalid input file");
    num_nets = stoi(tmp[2]);
    // nets
    for (int i = 0; i < num_nets; i++)
    {
        getline(fin, line);
        tmp = split(line, ' ');
        Assert(tmp.size() == 5, "Invalid input file");
        int x1 = stoi(tmp[1]);
        int y1 = stoi(tmp[2]);
        int x2 = stoi(tmp[3]);
        int y2 = stoi(tmp[4]);
        nets.push_back(make_pair(make_pair(x1, y1), make_pair(x2, y2)));
    }
    fin.close();

    this->rows = rows;
    this->cols = cols;
    this->capacity = capacity;
}

void Solution::init()
{
    vertices = rows * cols;
    adj.resize(vertices);
    parent.resize(vertices);
    d.resize(vertices);
    paths.resize(num_nets);

    alpha = pow(100, 1.0 / capacity);

    // build graph
    // horizontal
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < rows - 1; y++)
        {
            int pos = x + y * cols;
            // adj[pos].push_back(make_pair(pos + cols, 1));
            // adj[pos + cols].push_back(make_pair(pos, 1));
            adj[pos].push_back(GraphNode(pos + cols, 1));
            adj[pos + cols].push_back(GraphNode(pos, 1));
        }
    }
    // vertical
    for (int x = 0; x < cols - 1; x++)
    {
        for (int y = 0; y < rows; y++)
        {
            int pos = x + y * cols;
            // adj[pos].push_back(make_pair(pos + 1, 1));
            // adj[pos + 1].push_back(make_pair(pos, 1));
            adj[pos].push_back(GraphNode(pos + 1, 1));
            adj[pos + 1].push_back(GraphNode(pos, 1));
        }
    }
}

void Solution::update_adj(int pos)
{
    for (size_t i = 0; i < adj[pos].size(); i++)
    {
        if (adj[pos][i].id == parent[pos])
        {
            adj[pos][i].weight *= alpha;
            break;
        }
    }
    for (size_t i = 0; i < adj[parent[pos]].size(); i++)
    {
        if (adj[parent[pos]][i].id == pos)
        {
            adj[parent[pos]][i].weight *= alpha;
            break;
        }
    }
}

void Solution::choose_path(int i, int s, int v)
{
    paths[i].clear();
    int pos = v;
    paths[i].push_back(pos);
    while (pos != s)
    {
        update_adj(pos);
        pos = parent[pos];
        paths[i].push_back(pos);
    }
}

void Solution::solve()
{
    for (int i = 0; i < num_nets; i++)
    {
        int start_pos = nets[i].first.first + nets[i].first.second * cols;
        int end_pos = nets[i].second.first + nets[i].second.second * cols;
        dijkstra(start_pos);
        choose_path(i, start_pos, end_pos);
    }
}

void Solution::save_path()
{
    ofstream fout(output_file);
    for (int i = 0; i < num_nets; i++)
    {
        fout << i << " " << paths[i].size() - 1 << endl;
        for (int j = paths[i].size() - 1; j > 0; j--)
        {
            int x1 = paths[i][j] % cols;
            int y1 = paths[i][j] / cols;
            int x2 = paths[i][j - 1] % cols;
            int y2 = paths[i][j - 1] / cols;
            fout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
        }
    }
}
