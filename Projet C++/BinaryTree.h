#pragma once

#include <vector>
#include <iostream>
#include <stdexcept>

// Classe template représentant un arbre binaire (comme dans le modèle CRR)
template <typename T>
class BinaryTree {
private:
    int _depth;                          // profondeur N de l'arbre
    std::vector<std::vector<T>> _tree;   // _tree[n][i] contient la valeur au temps n, noeud i

public:
    // Constructeur par défaut : arbre vide
    BinaryTree()
        : _depth(0)
    {
    }

    // Getter sur la profondeur
    int getDepth() const {
        return _depth;
    }

    // Initialise la profondeur N et redimensionne _tree
    // n va de 0 à N, et pour chaque n, i va de 0 à n
    void setDepth(int depth) {
        if (depth < 0) {
            throw std::invalid_argument("Depth must be non-negative");
        }

        _depth = depth;

        // On doit avoir N+1 niveaux : 0, 1, ..., N
        _tree.clear();
        _tree.resize(_depth + 1);

        for (int n = 0; n <= _depth; ++n) {
            // à l'instant n, il y a n+1 noeuds : i = 0, ..., n
            _tree[n].resize(n + 1);
        }
    }

    // Fixe la valeur en (n, i)
    void setNode(int n, int i, const T& value) {
        if (n < 0 || n > _depth || i < 0 || i > n) {
            throw std::out_of_range("Invalid indices (n, i) in setNode");
        }
        _tree[n][i] = value;
    }

    // Récupère la valeur en (n, i)
    T getNode(int n, int i) const {
        if (n < 0 || n > _depth || i < 0 || i > n) {
            throw std::out_of_range("Invalid indices (n, i) in getNode");
        }
        return _tree[n][i];
    }

    // Affiche l'arbre niveau par niveau
    void display() const {
        std::cout << "BinaryTree (depth = " << _depth << ")\n";
        for (int n = 0; n <= _depth; ++n) {
            std::cout << "n = " << n << " : ";
            for (int i = 0; i <= n; ++i) {
                std::cout << _tree[n][i] << " ";
            }
            std::cout << "\n";
        }
    }
};
