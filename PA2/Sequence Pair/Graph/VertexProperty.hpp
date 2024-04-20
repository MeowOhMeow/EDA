#ifndef VERTEXPROPERTY_HPP
#define VERTEXPROPERTY_HPP

// Define a structure for vertex properties
template <class VertexData>
class VertexProperty
{
private:
    VertexData value;

public:
    VertexProperty() {}
    VertexProperty(VertexData val) : value(val) {}

    VertexData getValue() const
    {
        return value;
    }
};

#endif // VERTEXPROPERTY_HPP