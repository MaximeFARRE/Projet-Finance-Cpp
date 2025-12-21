#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

// Because this class is a template, its implementation is written in the .h file
template <class T>
class BinaryTree {
private:
    int _depth;
    vector< vector<T> > _tree;

public:
    BinaryTree() : _depth(0) {}

    void setDepth(int depth) {
        if (depth < 0) {
            throw invalid_argument("depth must be non-negative");
        }

        _depth = depth;
        _tree.clear();
        _tree.resize(_depth + 1);

        for (int n = 0; n <= _depth; ++n) {
            _tree[n].resize(n + 1);
        }
    }

    int getDepth() const {
        return _depth;
    }

    void setNode(int n, int i, const T& value) {
        _tree[n][i] = value;
    }

    T getNode(int n, int i) const {
        return _tree[n][i];
    }

    void display() const {
        for (int n = 0; n <= _depth; ++n) {
            for (int i = 0; i <= n; ++i) {
                cout << _tree[n][i] << " ";
            }
            cout << endl;
        }
    }
};
