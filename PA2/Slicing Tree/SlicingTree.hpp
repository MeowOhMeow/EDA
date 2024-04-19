#ifndef SLICINGTREE_HPP
#define SLICINGTREE_HPP

#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>
#include <string>

#include "BinaryTree.hpp"
#include "CombinationsOfMacros.hpp"

using namespace std;

int sizeOfBlocks = 0;

/*
 * SlicingTree class
 *
 */
class SlicingTree : public BinaryTree<CombinationsOfMacros>
{
private:
    const string HORIZONTAL = "+";
    const string VERTICAL = "*";

    using NodePtr = typename BinaryTree<CombinationsOfMacros>::Node *;

    /* 12v3v4v5...vN, v is verticalCombine -> 1*2)*3)*4)*5)*...)*N, all the numbers is a node
     * use recursive to build the tree
     */
    NodePtr build(vector<CombinationsOfMacros> &macros)
    {
        stack<NodePtr> stack;
        NodePtr node = new typename BinaryTree<CombinationsOfMacros>::Node(macros[0]);
        stack.push(node);
        for (int i = 1; i < macros.size(); i++)
        {
            NodePtr newNode = new typename BinaryTree<CombinationsOfMacros>::Node(macros[i]);
            NodePtr left = stack.top();
            stack.pop();

            NodePtr mulNode = new typename BinaryTree<CombinationsOfMacros>::Node(newNode->data * left->data);
            mulNode->left = left;
            mulNode->right = newNode;

            stack.push(mulNode);
        }

        while (stack.size() > 1)
        {
            NodePtr right = stack.top();
            stack.pop();
            NodePtr left = stack.top();
            stack.pop();

            NodePtr mulNode = new typename BinaryTree<CombinationsOfMacros>::Node(left->data * right->data);
            mulNode->left = left;
            mulNode->right = right;

            stack.push(mulNode);
        }

        return stack.top();
    }

    vector<string> getExpressions(NodePtr node, vector<string> &expression)
    {
        if (node != nullptr)
        {
            getExpressions(node->left, expression);
            getExpressions(node->right, expression);
            expression.push_back(node->data.toString());
        }

        return expression;
    }

protected:
    void insert(NodePtr &node, CombinationsOfMacros data) override
    {
        cout << "Method not implemented" << endl;
    }

public:
    SlicingTree() : BinaryTree<CombinationsOfMacros>() {}

    SlicingTree(vector<CombinationsOfMacros> &macros) : BinaryTree<CombinationsOfMacros>()
    {
        this->root = this->build(macros);
        sizeOfBlocks = macros.size();
    }

    SlicingTree(vector<string> &expression, unordered_map<string, pair<int, int>> &macrosMap) : BinaryTree<CombinationsOfMacros>()
    {
        stack<NodePtr> stack;
        for (int i = 0; i < expression.size(); i++)
        {
            if (expression[i] == VERTICAL)
            {
                NodePtr right = stack.top();
                stack.pop();
                NodePtr left = stack.top();
                stack.pop();

                NodePtr mulNode = new typename BinaryTree<CombinationsOfMacros>::Node(left->data * right->data);
                mulNode->left = left;
                mulNode->right = right;

                stack.push(mulNode);
            }
            else if (expression[i] == HORIZONTAL)
            {
                NodePtr right = stack.top();
                stack.pop();
                NodePtr left = stack.top();
                stack.pop();

                NodePtr addNode = new typename BinaryTree<CombinationsOfMacros>::Node(left->data + right->data);
                addNode->left = left;
                addNode->right = right;

                stack.push(addNode);
            }
            else
            {
                stack.push(new typename BinaryTree<CombinationsOfMacros>::Node(CombinationsOfMacros(expression[i], macrosMap[expression[i]])));
            }
        }

        this->root = stack.top();
    }

    CombinationsOfMacros getRootData()
    {
        return this->root->data;
    }

    vector<string> getExpressions()
    {
        vector<string> expression;
        return getExpressions(this->root, expression);
    }

    void reconstruct(string &filename, unordered_map<string, pair<int, int>> &macrosMap)
    {
        ofstream file(filename);
        // find index of the smallest area
        CombinationsOfMacros rootData = this->getRootData();
        vector<pair<int, int>> dimensions = rootData.getDimensions();
        int index = 0;
        int minArea = max(dimensions[0].first, dimensions[0].second);
        minArea *= minArea;
        for (size_t i = 1; i < dimensions.size(); i++)
        {
            int area = max(dimensions[i].first, dimensions[i].second);
            area *= area;
            if (area < minArea)
            {
                minArea = area;
                index = i;
            }
        }
        // reconstruct
        // reconstruct(this->root, file, macrosMap, index);
    }
};

#endif // SLICINGTREE_HPP