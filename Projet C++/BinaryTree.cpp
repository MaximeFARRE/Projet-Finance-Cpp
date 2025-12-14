#include "BinaryTree.h"
#include <iostream>
#include <stdexcept>
using namespace std;

template <class T>
BinaryTree<T>::BinaryTree() {
    _depth = 0;
    _tree.resize(1);
    _tree[0].resize(1);
}

template <class T>
void BinaryTree<T>::setDepth(int depth) {
    if (depth < 0) {
        throw invalid_argument("depth must be non-negative");
    }
    _depth = depth;
    // resize : _tree[n] contains n+1 nodes
    _tree.clear();
    _tree.resize(_depth + 1);

    for (int n = 0; n <= _depth; n++) {
        _tree[n].resize(n + 1);
    }
}

template <class T>
void BinaryTree<T>::setNode(int n, int i, const T& value) {
    // check indices
    if (n < 0 || n > _depth || i < 0 || i > n) {
        throw out_of_range("index out of range");
    }
    _tree[n][i] = value;
}

template <class T>
T BinaryTree<T>::getNode(int n, int i) const {
    // check indices
    if (n < 0 || n > _depth || i < 0 || i > n) {
        throw out_of_range("index out of range");
    }
    return _tree[n][i];
}

template <class T>
void BinaryTree<T>::display() const {
    // level-order print
    for (int n = 0; n <= _depth; n++) {
        for (int s = 0; s < (_depth - n); s++) {
            cout << "  ";
        }
        for (int i = 0; i <= n; i++) {
            cout << _tree[n][i];
            if (i < n) {
                cout << "   ";
            }
        }
        cout << endl;
    }
}