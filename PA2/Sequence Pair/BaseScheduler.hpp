#ifndef BASE_SCHEDULER_HPP
#define BASE_SCHEDULER_HPP

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <functional>

using namespace std;

class BaseScheduler
{
protected:
    chrono::high_resolution_clock::time_point start, end;
    double temperature = 1;
    double coolingRate = 0.95;
    bool run = true;
    int k;
    int movesCount = 0;
    int uphillCount = 0;
    int rejectCount = 0;

    bool improving = true;

    int previousMove = -1;

    vector<double> moveProbabilities;
    vector<double> cdf;

public:
    BaseScheduler(double initialTemp = 1, double cooling = 0.95, int timeLimit = 10, int k = 7)
        : temperature(initialTemp), coolingRate(cooling), k(k)
    {
        start = chrono::high_resolution_clock::now();
        end = start + chrono::minutes(timeLimit);
    }

    virtual ~BaseScheduler() {}

    inline void setTemperature(double temperature) { this->temperature = temperature; }
    inline void setCoolingRate(double coolingRate) { this->coolingRate = coolingRate; }

    // random generator, range: [min, max]
    inline int getRandomNumber(int min, int max)
    {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

    inline float getRandomNumber(float min, float max)
    {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    inline double getRandomNumber(double min, double max)
    {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    // Time-based functions
    inline bool hasTimeExpired() { return chrono::high_resolution_clock::now() >= end; }
    inline int getElapsedTimeInSeconds() { return chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start).count(); }

    inline double getTemperature() { return temperature; }

    inline bool isImproving()
    {
        temperature *= coolingRate;
        if (rejectCount / movesCount > 0.95)
        {
            improving = false;
        }
        return improving;
    }

    // Check if the scheduler can continue running
    inline bool canContinue() { return run; }

    // Virtual methods to be defined by subclasses
    virtual void initialize() = 0;
    virtual void makeRandomModification() = 0;
    virtual double evaluateState() = 0;

    // Event handlers
    virtual void accept() = 0;
    virtual void reject() = 0;
    virtual void uphill() = 0;

    // Get the number of steps per iteration
    virtual int getStepsPerIteration() = 0;
};

#endif // BASE_SCHEDULER_HPP
