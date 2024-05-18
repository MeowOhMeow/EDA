#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <ilcplex/ilocplex.h>

using namespace std;

ILOSTLBEGIN

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

vector<pair<int, int>> readPoints(const string &filename)
{
    ifstream file(filename);
    string line;
    int horizontalSpan, verticalSpan, nums;
    getline(file, line);
    vector<string> tokens = split(line, ' ');
    horizontalSpan = stoi(tokens[1]);
    verticalSpan = stoi(tokens[2]);
    getline(file, line);
    tokens = split(line, ' ');
    nums = stoi(tokens[2]);
    vector<pair<int, int>> points = vector<pair<int, int>>(nums);
    for (int i = 0; i < nums; i++)
    {
        getline(file, line);
        tokens = split(line, ' ');
        int id = stoi(tokens[0]);
        int x = stoi(tokens[1]);
        int y = stoi(tokens[2]);
        points[id] = make_pair(x, y);
    }
    file.close();
    return points;
}

vector<vector<int>> find_neighbor_points(const vector<pair<int, int>> &points, int range_x = 2, int range_y = 2)
{
    vector<vector<int>> neighbor_points(points.size());

    for (size_t i = 0; i < points.size(); ++i)
    {
        for (size_t j = 0; j < points.size(); ++j)
        {
            if (i != j)
            {
                int x1 = points[i].first;
                int y1 = points[i].second;
                int x2 = points[j].first;
                int y2 = points[j].second;
                if (abs(x1 - x2) <= range_x && abs(y1 - y2) <= range_y)
                {
                    neighbor_points[i].push_back(j);
                }
            }
        }
    }

    return neighbor_points;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <inputfile> <outputfile>" << endl;
        return 1;
    }

    vector<pair<int, int>> pointsCoordinates = readPoints(argv[1]);
    vector<vector<int>> neighbors = find_neighbor_points(pointsCoordinates);

    // for (size_t i = 0; i < points.size(); ++i)
    // {
    //     cout << "Neighbors of point (" << points[i].first << ", " << points[i].second << "): ";
    //     for (const auto &neighbor_idx : neighbors[i])
    //     {
    //         cout << neighbor_idx << "(" << points[neighbor_idx].first << ", " << points[neighbor_idx].second << ")" << " ";
    //     }
    //     cout << endl;
    // }

    IloEnv env;
    try
    {
        IloModel model(env);
        IloCplex cplex(model);

        // There are 3 colors for each point to choose from
        IloNumVarArray colorBit0(env, pointsCoordinates.size(), 0, 1, ILOBOOL);
        IloNumVarArray colorBit1(env, pointsCoordinates.size(), 0, 1, ILOBOOL);

        // Objective function
        IloExpr obj(env);
        for (size_t i = 0; i < pointsCoordinates.size(); ++i)
        {
            // constraint: there is only 3 colors can be chosen
            model.add(colorBit0[i] + colorBit1[i] <= 1);
            // constraint: colorBits are binary
            model.add(colorBit0[i] == 0 || colorBit1[i] == 0 || colorBit0[i] == 1 || colorBit1[i] == 1);

            // for all points try to avoid the same color as neighbors
            for (const auto &neighbor_idx : neighbors[i])
            {
                if (neighbor_idx < i)
                {
                    // constraint: if two points are neighbors, they cannot have the same color
                    model.add(colorBit0[i] + colorBit0[neighbor_idx] <= 1 + (colorBit0[i] == colorBit0[neighbor_idx]));
                    model.add(colorBit1[i] + colorBit1[neighbor_idx] <= 1 + (colorBit1[i] == colorBit1[neighbor_idx]));
                    model.add((1 - colorBit0[i]) + (1 - colorBit0[neighbor_idx]) <= 1 + (colorBit0[i] == colorBit0[neighbor_idx]));
                    model.add((1 - colorBit1[i]) + (1 - colorBit1[neighbor_idx]) <= 1 + (colorBit1[i] == colorBit1[neighbor_idx]));

                    obj += (colorBit0[i] == colorBit0[neighbor_idx]) && (colorBit1[i] == colorBit1[neighbor_idx]);
                }
            }
        }
        model.add(IloMinimize(env, obj));

        // Solve the problem
        cplex.solve();

        // Print solution
        ofstream output(argv[2]);
        if (cplex.getStatus() == IloAlgorithm::Optimal)
        {
            cout << "Objective Value: " << cplex.getObjValue() << endl;
            output << cplex.getObjValue() << endl;
            for (size_t i = 0; i < pointsCoordinates.size(); ++i)
            {
                bool colorBit0Value = cplex.getValue(colorBit0[i]);
                bool colorBit1Value = cplex.getValue(colorBit1[i]);
                int color = colorBit0Value ? 0 : colorBit1Value ? 1
                                                                : 2;
                cout << "Point " << i << " color: " << color << endl;
                output << i << " " << color << endl;
            }
        }
        else
        {
            cout << "No solution found" << endl;
        }
        output.close();
    }
    catch (IloException &ex)
    {
        cerr << "Error: " << ex << endl;
    }

    return 0;
}