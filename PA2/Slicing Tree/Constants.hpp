#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace Constants
{
    const string HORIZONTAL = "+";
    const string VERTICAL = "*";

    const int NUM_MOVES = 3;

    enum Move
    {
        M1,
        M2,
        M3
    };
}

#endif // CONSTANTS_HPP