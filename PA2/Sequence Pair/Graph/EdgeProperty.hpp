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

    // Overloading the operator!=
    bool notEquals(const EdgeProperty<EdgeData> &other) const
    {
        return value != other.value;
    }
};

#endif // EDGEPROPERTY_HPP