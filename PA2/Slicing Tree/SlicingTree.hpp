#ifndef SLICINGTREE_HPP
#define SLICINGTREE_HPP

#include <iostream>
#include <stack>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>

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

    NodePtr build(vector<CombinationsOfMacros> &macros)
    {
        stack<NodePtr> stack;
        for (size_t i = 0; i < macros.size(); i++)
        {
            stack.push(new typename BinaryTree<CombinationsOfMacros>::Node(macros[i]));
        }

        while (stack.size() > 1)
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

        return stack.top();
    }

    void getExpressions(NodePtr node, vector<string> &expression)
    {
        if (node != nullptr)
        {
            getExpressions(node->left, expression);
            getExpressions(node->right, expression);
            expression.push_back(node->data.toString());
        }
    }

    void reconstruct(NodePtr node, ofstream &fout, int index, pair<int, int> &offset)
    {
        if (node != nullptr)
        {
            CombinationsOfMacros data = node->data;
            if (data.getOperatorChar() == HORIZONTAL)
            {
                pair<int, int> srcs = data.getCombinationIndex(index);
                pair<int, int> dims = node->left->data.getDimensions(srcs.first);
                pair<int, int> offsetDown = {0 + offset.first, 0 + offset.second};
                pair<int, int> offsetUp = {0 + offset.first, dims.second + offset.second};
                reconstruct(node->left, fout, srcs.first, offsetDown);
                reconstruct(node->right, fout, srcs.second, offsetUp);
            }
            else if (data.getOperatorChar() == VERTICAL)
            {
                pair<int, int> srcs = data.getCombinationIndex(index);
                pair<int, int> dims = node->left->data.getDimensions(srcs.first);
                pair<int, int> offsetLeft = {0 + offset.first, 0 + offset.second};
                pair<int, int> offsetRight = {dims.first + offset.first, 0 + offset.second};

                reconstruct(node->left, fout, srcs.first, offsetLeft);
                reconstruct(node->right, fout, srcs.second, offsetRight);
            }
            else
            {
                pair<int, int> dims = data.getDimensions(index);
                // name width height x y x+w y+h
                fout << data.getName(index) << " " << dims.first << " " << dims.second << " " << offset.first << " " << offset.second << " " << dims.first + offset.first << " " << dims.second + offset.second << endl;
            }
        }
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
        for (size_t i = 0; i < expression.size(); i++)
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
        getExpressions(this->root, expression);
        return expression;
    }

    void saveToFile(string filename)
    {
        ofstream fout(filename);
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
        pair<int, int> chipSize = rootData.getDimensions(index);
        fout << chipSize.first << " " << chipSize.second << endl;
        // reconstruct
        pair<int, int> offset = {0, 0};
        reconstruct(this->root, fout, index, offset);
        fout.close();
    }
};

#endif // SLICINGTREE_HPP