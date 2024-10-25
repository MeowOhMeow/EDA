#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

template <class Data>
class Rectangle
{
private:
    int w;
    int h;
    Data value;

public:
    Rectangle() {}
    Rectangle(int w, int h, Data value) : w(w), h(h), value(value) {}

    int getX() const
    {
        return w;
    }

    int getY() const
    {
        return h;
    }

    Data getValue() const
    {
        return value;
    }

    void setW(int w)
    {
        this->w = w;
    }

    void setH(int h)
    {
        this->h = h;
    }

    void setValue(Data value)
    {
        this->value = value;
    }
};

#endif // RECTANGLE_HPP