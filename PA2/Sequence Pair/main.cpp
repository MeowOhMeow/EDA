#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include "Macro.hpp"
#include "Scheduler.hpp"
#include "SimulatedAnnealing.hpp"

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

vector<Macro> getMacros(string filename)
{
    vector<Macro> macros;
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
        macros.push_back(Macro(name, w, h));
    }
    file.close();

    return macros;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <inputfile> <outputfile>" << endl;
        return 1;
    }

    vector<Macro> macros = getMacros(argv[1]);

    Scheduler scheduler(macros);

    SimulatedAnnealing sa(1e5, 0.85, 0.01, 7);
    sa.run(scheduler);

    scheduler.saveFloorplan(argv[2]);
    cout << "Output saved to " << argv[2] << endl;
    cout << "See " << sa.getLogFilename() << " for more details." << endl;
    return 0;
}
