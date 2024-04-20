#ifndef SIMULATEDANNEALING_HPP
#define SIMULATEDANNEALING_HPP

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <chrono>
#include <string>
#include <vector>
#include <sstream>

#include "FloorPlanningScheduler.hpp"
#include "SlicingTree.hpp"

using namespace std;

class SimulatedAnnealing
{
private:
    double temperature, coolingRate, absoluteTemperature;
    int iterations;
    string filename;
    ofstream logFile;

public:
    SimulatedAnnealing(
        double temperature = 1,
        double coolingRate = 0.95,
        double absoluteTemperature = 0.01,
        int iterations = 7)
    {
        this->temperature = temperature;
        this->coolingRate = coolingRate;
        this->absoluteTemperature = absoluteTemperature;
        this->iterations = iterations;
        // get current date and time
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%H-%M-%S");
        filename = "logs/" + ss.str() + ".log";
        logFile.open(filename);
        logFile << "temperature: " << temperature << endl;
        logFile << "coolingRate: " << coolingRate << endl;
        logFile << "absoluteTemperature: " << absoluteTemperature << endl;
        logFile << "iterations: " << iterations << endl;
    }

    ~SimulatedAnnealing()
    {
        logFile.close();
    }

    string getLogFilename()
    {
        return filename;
    }

    vector<string> run(SlicingTree &tree, FloorPlanningScheduler &procedure, unordered_map<string, pair<int, int>> &macrosMap)
    {
        procedure.setN(tree.getSize());
        procedure.setK(iterations);
        procedure.setTemperature(temperature);
        procedure.setCoolingRate(coolingRate);

        vector<string> bestExpressions = tree.getExpressions();
        vector<string> currentExpressions = tree.getExpressions();
        float bestCost = procedure.evaluateState(bestExpressions, macrosMap);
        float currentCost = bestCost;
        logFile << "Initial cost: " << bestCost << endl;
        logFile << "Expression size: " << bestExpressions.size() << endl;
        logFile << setw(10) << "Time" << setw(10) << "Steps" << setw(20) << "Cost" << endl;

        int steps = 0;

        do
        {
            procedure.initialize();
            while (procedure.canContinue())
            {
                // make a random modification to the current tree (state)
                vector<string> newExpressions = procedure.makeRandomModification(currentExpressions);

                float newCost = procedure.evaluateState(newExpressions, macrosMap);

                if (newCost <= bestCost)
                {
                    bestCost = newCost;
                    currentCost = newCost;
                    bestExpressions = newExpressions;
                    currentExpressions = newExpressions;
                    procedure.accept();
                }
                else
                {
                    float acceptanceProbability = exp((currentCost - newCost) / procedure.getTemperature());
                    if (acceptanceProbability > procedure.getRandomNumber(0.0f, 1.0f))
                    {
                        currentCost = newCost;
                        currentExpressions = newExpressions;
                        procedure.uphill();
                    }
                    else
                    {
                        procedure.reject();
                    }
                }

                steps++;
            }

            logFile << setw(10) << procedure.getRemainingTime() << setw(10) << steps << setw(20) << bestCost << endl;
        } while (procedure.isImproving() && !procedure.hasTimeExpired() && procedure.getTemperature() > absoluteTemperature);

        if (procedure.hasTimeExpired())
        {
            logFile << "Time expired" << endl;
        }
        else if (!procedure.isImproving())
        {
            logFile << "No improvement" << endl;
        }
        else
        {
            logFile << "Temperature too low, temperature: " << procedure.getTemperature() << endl;
        }
        cout << endl;

        return bestExpressions;
    }
};

#endif // SIMULATEDANNEALING_HPP