#pragma once
#include <vector>

template <class T>
class BinaryTree {
private:
    int _depth;
    vector< vector<T> > _tree;

public:
    BinaryTree();
    void setDepth(int depth);
    void setNode(int n, int i, const T& value);
    T getNode(int n, int i) const;

    // prints all nodes level by level
    void display() const;
};
#include "BinaryTree.cpp"
