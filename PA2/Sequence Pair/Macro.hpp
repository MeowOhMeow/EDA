#ifndef MACRO_HPP
#define MACRO_HPP

#include <iostream>

using namespace std;

class Macro
{
private:
    string name;
    int width;
    int height;

public:
    Macro() : name(""), width(0), height(0) {}
    Macro(string name, int width, int height) : name(name), width(width), height(height) {}

    string getName() const
    {
        return name;
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    void swapWidthHeight()
    {
        int temp = width;
        width = height;
        height = temp;
    }

    void print() const
    {
        cout << name << " " << width << " " << height << endl;
    }
};

#endif // MACRO_HPP
