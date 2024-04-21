#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <iostream>

using namespace std;

template <class T>
class Coordinates
{
private:
    int x;
    int y;
    T value;

public:
    Coordinates() {}
    Coordinates(int x, int y, T value) : x(x), y(y), value(value) {}

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    T getValue() const
    {
        return value;
    }

    void setX(int x)
    {
        this->x = x;
    }

    void setY(int y)
    {
        this->y = y;
    }

    void setValue(T value)
    {
        this->value = value;
    }
};

#endif // COORDINATES_HPP