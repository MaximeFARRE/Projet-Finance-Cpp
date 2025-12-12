#include "BinaryTree.h"
#include <iostream>
#include <stdexcept>

template <class T>
BinaryTree<T>::BinaryTree() {
    _depth = 0;
}

template <class T>
int BinaryTree<T>::index(int n, int i) const {
    // index de la ligne n : n*(n+1)/2
    return (n * (n + 1)) / 2 + i;
}

template <class T>
void BinaryTree<T>::setDepth(int depth) {
    if (depth < 0) {
        throw std::invalid_argument("depth must be non-negative");
    }

    _depth = depth;

    // nombre total de noeuds: 1 + 2 + ... + (depth+1)
    int total = (_depth + 1) * (_depth + 2) / 2;

    _data.clear();
    _data.resize(total);
}

template <class T>
int BinaryTree<T>::getDepth() const {
    return _depth;
}

template <class T>
void BinaryTree<T>::setNode(int n, int i, const T& value) {
    _data[index(n, i)] = value;
}

template <class T>
T BinaryTree<T>::getNode(int n, int i) const {
    return _data[index(n, i)];
}

template <class T>
void BinaryTree<T>::display() const {
    // affichage "en arbre" simple (indentation)
    for (int n = 0; n <= _depth; n++) {
        // espaces au début pour centrer un peu
        for (int s = 0; s < (_depth - n); s++) {
            std::cout << "  ";
        }

        for (int i = 0; i <= n; i++) {
            std::cout << getNode(n, i);
            if (i < n) {
                std::cout << "   ";
            }
        }
        std::cout << std::endl;
    }
}

/*
  IMPORTANT :
  On force la génération du code pour les types qu'on utilise dans le projet.
  Dans votre CRRPricer, vous utilisez BinaryTree<double> et BinaryTree<bool>.
*/
template class BinaryTree<double>;
template class BinaryTree<bool>;
