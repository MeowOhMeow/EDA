#include "Generator.h"

using namespace std;

Generator::Generator(string filename)
{
    this->filename = filename;
    // initialize parameters
    init();

    // Read prefix and suffix from files
    loadPrefixNSuffix();
}

void Generator::init()
{
    ifstream fin;
    fin.open(filename);
    string line;

    // Read xtics and ytics
    getline(fin, line);
    vector<string> tokens = split(line, ' ');
    xbound = stoi(tokens[1]);
    ybound = stoi(tokens[2]);

    // Read macroCount
    getline(fin, line);
    tokens = split(line, ' ');
    macroCount = stoi(tokens[1]);

    fin.close();

    // calculate xtics and ytics
    xtics = xbound / 5;
    ytics = ybound / 5;

    cout << "xbound: " << xbound << endl;
    cout << "ybound: " << ybound << endl;
    cout << "xtics: " << xtics << endl;
    cout << "ytics: " << ytics << endl;
}

void Generator::loadPrefixNSuffix()
{
    ifstream fin;
    fin.open("prefix.txt");
    prefix = "";
    string line;
    while (getline(fin, line))
    {
        prefix += line + "\n";
    }
    fin.close();

    fin.open("suffix.txt");
    suffix = "";
    while (getline(fin, line))
    {
        suffix += line + "\n";
    }
    fin.close();
}

void Generator::generate()
{
    ifstream fin;
    fin.open(filename);
    string line;
    getline(fin, line);
    getline(fin, line);

    ofstream fout;
    fout.open("output.gp");

    fout << prefix;
    for (int i = 0; i < macroCount; i++)
    {

        getline(fin, line);
        vector<string> tokens = split(line, ' ');
        fout << "set object " + to_string(i + 1) + " rect from ";
        fout << tokens[1] + "," + tokens[2] + " to ";
        fout << tokens[3] + "," + tokens[4] + "\n";
        fout << "set label \"" + tokens[0] + "\" at ";
        fout << to_string((stoi(tokens[1]) + stoi(tokens[3])) / 2) + ",";
        fout << to_string((stoi(tokens[2]) + stoi(tokens[4])) / 2) + "center\n";
    }
    fout << "set xtics " + to_string(xtics) + "\n";
    fout << "set ytics " + to_string(ytics) + "\n";
    fout << "plot [0:" + to_string(xbound) + "][0:" + to_string(ybound) + "]0\n";
    fout << suffix;

    fin.close();
    fout.close();
}