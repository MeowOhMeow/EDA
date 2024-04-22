#ifndef EDGEPROPERTY_HPP
#define EDGEPROPERTY_HPP

// Define a structure for edge properties
template <class EdgeData>
class EdgeProperty
{
private:
    EdgeData value;

public:
    EdgeProperty() {}
    EdgeProperty(EdgeData val) : value(val) {}

    EdgeData getValue() const
    {
        return value;
    }
};

#endif // EDGEPROPERTY_HPP