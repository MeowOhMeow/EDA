#ifndef COORDINATES_HPP
#define COORDINATES_HPP

template <class Data>
class Coordinates
{
private:
    int x;
    int y;
    Data value;

public:
    Coordinates() {}
    Coordinates(int x, int y, Data value) : x(x), y(y), value(value) {}

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    Data getValue() const
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

    void setValue(Data value)
    {
        this->value = value;
    }
};

#endif // COORDINATES_HPP