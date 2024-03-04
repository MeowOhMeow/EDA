#include "utility.h"

using namespace std;

Solution solution;
string input_file;
string output_file;

vector<string> split(string str, char delim)
{
    vector<string> res;
    string tmp;
    for (auto ch : str)
    {
        if (ch == delim)
        {
            res.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp.push_back(ch);
        }
    }
    res.push_back(tmp);
    return res;
}

void _load()
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

    solution = Solution(cols, rows, capacity);
}

void init(int argc, char *argv[])
{
    Assert(argc == 3, "Usage: %s <input file> <output file>", argv[0]);
    input_file = argv[1];
    output_file = argv[2];
    _load();
}

void sovle()
{
    solution.solve();
}

void save_path()
{
    solution.save_path(output_file);
}
