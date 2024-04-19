#ifndef SIMULATEDANNEALING_HPP
#define SIMULATEDANNEALING_HPP

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <iomanip>

#include "FloorPlanningProcedure.hpp"
#include "SlicingTree.hpp"

using namespace std;

class SimulatedAnnealing
{
private:
    double temperature, coolingRate, absoluteTemperature;
    int iterations;
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
        logFile.open("sa.log");
        logFile << "temperature: " << temperature << endl;
        logFile << "coolingRate: " << coolingRate << endl;
        logFile << "absoluteTemperature: " << absoluteTemperature << endl;
        logFile << "iterations: " << iterations << endl;
    }

    ~SimulatedAnnealing()
    {
        logFile.close();
    }

    vector<string> run(SlicingTree &tree, FloorPlanningProcedure &procedure, unordered_map<string, pair<int, int>> &macrosMap)
    {
        procedure.setN(sizeOfBlocks);
        procedure.setK(iterations);
        procedure.setTemperature(temperature);
        procedure.setCoolingRate(coolingRate);

        vector<string> bestExpressions = tree.getExpressions();
        vector<string> currentExpressions = tree.getExpressions();
        float bestCost = procedure.evaluateState(bestExpressions, macrosMap);

        logFile << "Expression size: " << bestExpressions.size() << endl;
        logFile << setw(10) << "Time" << setw(10) << "Steps" << setw(20) << "Cost" << endl;

        int steps = 0;

        do
        {
            procedure.initialize();
            while (procedure.run)
            {
                // make a random modification to the current tree (state)
                vector<string> newExpressions = procedure.makeRandomModification(currentExpressions);

                float newCost = procedure.evaluateState(newExpressions, macrosMap);

                if (newCost <= bestCost)
                {
                    bestCost = newCost;
                    bestExpressions = newExpressions;
                    currentExpressions = newExpressions;
                    procedure.accept();
                }
                else
                {
                    float acceptanceProbability = exp((bestCost - newCost) / procedure.getTemperature());
                    if (acceptanceProbability > procedure.getRandomNumber(0.0f, 1.0f))
                    {
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