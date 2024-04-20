#ifndef VERTEX_HPP
#define VERTEX_HPP

// Define a structure for vertices
class Vertex
{
private:
    int id;

public:
    Vertex(int id) : id(id) {}

    int getId() const
    {
        return id;
    }
};

#endif // VERTEX_HPP