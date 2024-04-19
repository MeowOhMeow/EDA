#ifndef SIMULATEDANNEALING_HPP
#define SIMULATEDANNEALING_HPP

#include <iostream>
#include <unordered_map>

#include "FloorPlanningProcedure.hpp"
#include "SlicingTree.hpp"

using namespace std;

class SimulatedAnnealing
{
private:
    float temperature, coolingRate, absoluteTemperature;
    int iterations;

public:
    SimulatedAnnealing(
        float temperature = 1,
        float coolingRate = 0.95,
        float absoluteTemperature = 0.01,
        int iterations = 7)
    {
        this->temperature = temperature;
        this->coolingRate = coolingRate;
        this->absoluteTemperature = absoluteTemperature;
        this->iterations = iterations;
    }

    vector<string> run(SlicingTree &tree, FloorPlanningProcedure &procedure, unordered_map<string, pair<int, int>> &macrosMap)
    {
        procedure.setN(sizeOfBlocks);
        procedure.setK(iterations);

        vector<string> bestExpressions = tree.getExpressions();
        vector<string> currentExpressions = tree.getExpressions();
        float bestCost = procedure.evaluateState(bestExpressions, macrosMap);

        do
        {
            procedure.initialize();
            while (procedure.run)
            {
                // make a random modification to the current tree (state)
                vector<string> newExpressions = procedure.makeRandomModification(currentExpressions);

                float newCost = procedure.evaluateState(newExpressions, macrosMap);
                for (auto &exp : newExpressions)
                {
                    cout << exp << " ";
                }
                cout << endl;

                if (newCost <= bestCost)
                {
                    bestCost = newCost;
                    bestExpressions = newExpressions;
                    currentExpressions = newExpressions;
                    procedure.accept();
                }
                else
                {
                    float acceptanceProbability = exp((bestCost - newCost) / temperature);
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
            }

            temperature *= coolingRate;
            cout << "isImproving: " << procedure.isImproving() << endl;
            cout << "hasTimeExpired: " << procedure.hasTimeExpired() << endl;
            cout << "temperature > absoluteTemperature: " << (temperature > absoluteTemperature) << endl;
        } while (procedure.isImproving() && !procedure.hasTimeExpired() && temperature > absoluteTemperature);

        return bestExpressions;
    }
};

#endif // SIMULATEDANNEALING_HPP