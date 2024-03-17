#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "utility.h"

class Generator
{
private:
    string prefix, suffix;
    string infile, outfile;
    int xbound, ybound;
    int xtics, ytics;
    int macroCount;

    void init();
    void loadPrefixNSuffix();

public:
    Generator(string input, string output);
    void generate();
};

#endif // GENERATOR_H