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

    void run(Scheduler &scheduler)
    {
        scheduler.setTemperature(temperature);
        scheduler.setCoolingRate(coolingRate);

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

            logFile << setw(10) << scheduler.getRemainingTime() << setw(10) << steps << setw(20) << bestCost << endl;
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