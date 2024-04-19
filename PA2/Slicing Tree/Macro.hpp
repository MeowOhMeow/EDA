#ifndef MACRO_HPP
#define MACRO_HPP

#include <iostream>
#include <string>

using namespace std;

class Macro
{
private:
    string name;
    int width, height;
    int x, y;

public:
    Macro(string name, int width, int height) : name(name), width(width), height(height), x(0), y(0) {}

    

    // overload the << operator to print the macro
    friend ostream &operator<<(ostream &os, const Macro &macro)
    {
        os << macro.name << " " << macro.width << " " << macro.height << " " << macro.x << " " << macro.y << macro.x + macro.width << " " << macro.y + macro.height;
        return os;
    }
};

#endif // MACRO_HPP
