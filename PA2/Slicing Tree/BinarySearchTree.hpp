#ifndef BINARTSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP

#include <iostream>

#include "BinaryTree.hpp"

using namespace std;

template <typename T>
class BinarySearchTree : public BinaryTree<T>
{
protected:
    void insert(typename BinaryTree<T>::Node *&node, T data) override
    {
        if (node == nullptr)
        {
            node = new typename BinaryTree<T>::Node;
            node->data = data;
            node->left = nullptr;
            node->right = nullptr;
        }
        else if (data < node->data)
        {
            insert(node->left, data);
        }
        else
        {
            insert(node->right, data);
        }
    }

    void remove(typename BinaryTree<T>::Node *&node, T data)
    {
        if (node == nullptr)
        {
            return;
        }

        if (data < node->data)
        {
            remove(node->left, data);
        }
        else if (data > node->data)
        {
            remove(node->right, data);
        }
        else
        {
            if (node->left == nullptr && node->right == nullptr)
            {
                delete node;
                node = nullptr;
            }
            else if (node->left == nullptr)
            {
                typename BinaryTree<T>::Node *temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr)
            {
                typename BinaryTree<T>::Node *temp = node;
                node = node->left;
                delete temp;
            }
            else
            {
                typename BinaryTree<T>::Node *temp = node->right;
                while (temp->left != nullptr)
                {
                    temp = temp->left;
                }
                node->data = temp->data;
                remove(node->right, temp->data);
            }
        }
    }
public:
    BinarySearchTree() : BinaryTree<T>() {}

    void insert(T data)
    {
        insert(this->root, data);
    }

    void remove(T data)
    {
        remove(this->root, data);
    }

    void print()
    {
        BinaryTree<T>::print(this->root);
    }

    void printStructure()
    {
        BinaryTree<T>::printStructure(this->root, 0);
    }

    void inOrder(vector<T> &v)
    {
        BinaryTree<T>::inOrder(this->root, v);
    }

    void preOrder(vector<T> &v)
    {
        BinaryTree<T>::preOrder(this->root, v);
    }

    void postOrder(vector<T> &v)
    {
        BinaryTree<T>::postOrder(this->root, v);
    }
};

#endif // BINARYSEARCHTREE_HPP