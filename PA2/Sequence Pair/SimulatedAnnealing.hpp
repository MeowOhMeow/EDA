#ifndef SIMULATEDANNEALING_HPP
#define SIMULATEDANNEALING_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>

#include "Scheduler.hpp"

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
        int iterations = 7,
        string filename = "log.txt")
    {
        this->temperature = temperature;
        this->coolingRate = coolingRate;
        this->absoluteTemperature = absoluteTemperature;
        this->iterations = iterations;
        this->filename = filename;
        logFile.open(filename, ios::app);
    }

    ~SimulatedAnnealing()
    {
        logFile.close();
    }

    string getLogFilename()
    {
        return filename;
    }

    void run(Scheduler &scheduler)
    {
        logFile << "temperature: " << temperature << endl;
        logFile << "coolingRate: " << coolingRate << endl;
        logFile << "absoluteTemperature: " << absoluteTemperature << endl;
        logFile << "iterations: " << iterations << endl;

        scheduler.setTemperature(temperature);
        scheduler.setCoolingRate(coolingRate);
        int totalIterations = log2(absoluteTemperature / temperature) / log2(coolingRate);
        int stepPerIteration = scheduler.getStepPerIteration();
        int currentIteration = 0;

        float bestCost = scheduler.evaluateState();
        float currentCost = bestCost;
        logFile << "Initial cost: " << bestCost << endl;
        logFile << setw(10) << "Time" << setw(10) << "Steps" << setw(20) << "Cost" << endl;

        int steps = 0;

        do
        {
            scheduler.initialize();
            while (scheduler.canContinue())
            {
                // make a random modification to the current tree (state)
                scheduler.makeRandomModification();

                float newCost = scheduler.evaluateState();

                if (newCost <= bestCost)
                {
                    bestCost = newCost;
                    currentCost = newCost;
                    scheduler.accept();
                }
                else
                {
                    float acceptanceProbability = exp((currentCost - newCost) / scheduler.getTemperature());
                    if (acceptanceProbability > scheduler.getRandomNumber(0.0f, 1.0f))
                    {
                        currentCost = newCost;
                        scheduler.uphill();
                    }
                    else
                    {
                        scheduler.reject();
                    }
                }

                steps++;
            }
            currentIteration++;
            int completeSteps = min(100 * currentIteration / totalIterations, 100);
            int ETA = max(max(totalIterations - currentIteration, 0) * stepPerIteration, 0) * (scheduler.getElapsed() / float(steps + 1));
            cout << "Progress: " << completeSteps << "% [" << string(completeSteps / 2, '=') << string(50 - completeSteps / 2, ' ') << "] ETA: " << ETA << "s   " << "\r" << flush;

            logFile << setw(10) << scheduler.getElapsed() << setw(10) << steps << setw(20) << bestCost << endl;
        } while (scheduler.isImproving() && !scheduler.hasTimeExpired() && scheduler.getTemperature() > absoluteTemperature);

        if (scheduler.hasTimeExpired())
        {
            logFile << "Time expired" << endl;
        }
        else if (!scheduler.isImproving())
        {
            logFile << "No improvement" << endl;
        }
        else
        {
            logFile << "Temperature too low, temperature: " << scheduler.getTemperature() << endl;
        }
        cout << endl;
    }
};

#endif // SIMULATEDANNEALING_HPP