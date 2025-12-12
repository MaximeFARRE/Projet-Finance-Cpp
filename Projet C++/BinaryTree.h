#pragma once
#include <vector>

template <class T>
class BinaryTree {
private:
    int _depth;
    std::vector<T> _data;   // stockage 1D contigu

    int index(int n, int i) const;  // convertit (n,i) -> index dans _data

public:
    BinaryTree();

    void setDepth(int depth);
    int getDepth() const;

    void setNode(int n, int i, const T& value);
    T getNode(int n, int i) const;

    void display() const;
};
