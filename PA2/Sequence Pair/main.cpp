#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

#include "Macro.hpp"
#include "Scheduler.hpp"
#include "SimulatedAnnealing.hpp"

using namespace std;

vector<string> split(string &str, char delim)
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

    // get current date and time
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    string filename = "logs/" + ss.str() + ".log";
    ofstream logFile;
    logFile.open(filename);
    logFile << "Start time: " << put_time(localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << endl;
    logFile << "Input file: " << argv[1] << endl;
    logFile << "Output file: " << argv[2] << endl;
    logFile.close();

    vector<Macro> macros = getMacros(argv[1]);

    Scheduler scheduler(macros);

    double avgSize = 0;
    for (auto &macro : macros)
    {
        avgSize += max(macro.getWidth(), macro.getHeight());
    }
    SimulatedAnnealing sa(avgSize, 0.85, 1, 7, filename);
    sa.run(scheduler);

    in_time_t = chrono::system_clock::to_time_t(chrono::system_clock::now());
    logFile.open(filename, ios::app);
    logFile << "End time: " << put_time(localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << endl;

    scheduler.saveFloorplan(argv[2]);
    cout << "Output saved to " << argv[2] << endl;
    cout << "See " << sa.getLogFilename() << " for more details." << endl;
    return 0;
}
