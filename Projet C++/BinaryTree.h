#pragma once
#include <vector>
#include <iostream>

template <class T>
class BinaryTree {
private:
    int _depth;
    std::vector< std::vector<T> > _tree;

public:
    BinaryTree() : _depth(0) {}

    void setDepth(int depth) {
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
                std::cout << _tree[n][i] << " ";
            }
            std::cout << std::endl;
        }
    }
};
