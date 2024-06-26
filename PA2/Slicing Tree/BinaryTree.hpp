#ifndef BINARTTREE_HPP
#define BINARYTREE_HPP

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

template <typename VertexData>
class BinaryTree
{
protected:
    struct Node
    {
        VertexData data;
        Node *left;
        Node *right;

        Node(VertexData data = VertexData(), Node *left = nullptr, Node *right = nullptr) : data(data), left(left), right(right) {}
    };

    Node *root;

    virtual void insert(Node *&node, VertexData data) = 0;

    void print(Node *node)
    {
        if (node != nullptr)
        {
            print(node->left);
            cout << node->data << " ";
            print(node->right);
        }
    }

    void printStructure(Node *node, int level)
    {
        if (node != nullptr)
        {
            printStructure(node->left, level + 1);
            cout << setw(3 * level) << " " << node->data << endl;
            printStructure(node->right, level + 1);
        }
    }

    void destroy(Node *node)
    {
        if (node != nullptr)
        {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    void transplant(Node *u, Node *v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }

        if (v != nullptr)
        {
            v->parent = u->parent;
        }
    }

    void inOrder(Node *node, vector<VertexData> &v)
    {
        if (node != nullptr)
        {
            inOrder(node->left, v);
            v.push_back(node->data);
            inOrder(node->right, v);
        }
    }

    void preOrder(Node *node, vector<VertexData> &v)
    {
        if (node != nullptr)
        {
            v.push_back(node->data);
            preOrder(node->left, v);
            preOrder(node->right, v);
        }
    }

    void postOrder(Node *node, vector<VertexData> &v)
    {
        if (node != nullptr)
        {
            postOrder(node->left, v);
            postOrder(node->right, v);
            v.push_back(node->data);
        }
    }

public:
    BinaryTree()
    {
        root = nullptr;
    }

    ~BinaryTree()
    {
        destroy(root);
    }

    void insert(VertexData data)
    {
        insert(root, data);
    }

    void print()
    {
        print(root);
        cout << endl;
    }

    void printStructure()
    {
        printStructure(root, 0);
    }

    vector<VertexData> inOrder()
    {
        vector<VertexData> v;
        inOrder(root, v);
        return v;
    }

    vector<VertexData> preOrder()
    {
        vector<VertexData> v;
        preOrder(root, v);
        return v;
    }

    vector<VertexData> postOrder()
    {
        vector<VertexData> v;
        postOrder(root, v);
        return v;
    }
};

#endif // BINARYTREE_HPP
